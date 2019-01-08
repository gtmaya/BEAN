#include "peep.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

int Peep::temp = 0;

std::array<glm::vec3, 5> colours {glm::vec3(27, 153, 139), glm::vec3(255, 255, 220), glm::vec3(255, 253, 130), glm::vec3(255, 155, 113), glm::vec3(232, 72, 85)};

Peep::Peep() : m_position     ({1.f, 1.f}),
               m_shaderProps  (new ShaderProps({1.f, 1.f, 1.f},
                                              {1.f, 1.f, 1.f},
                                              1.f,
                                              0.1f,
                                              1.f,
                                              0.f,
                                              1.f,
                                              0)),
               m_velocity     ({0.f, 0.f}),
               m_direction    ({0.f, 0.f})
{
  m_speed = (float(std::rand()) / float(RAND_MAX)) * 0.1f + 0.1f;
  m_position.x = float(std::rand()) / float(RAND_MAX) * 254.f + 1.f;
  m_position.y = float(std::rand()) / float(RAND_MAX) * 254.f + 1.f;
  m_nearestTile.x = int(std::floor(m_position.x));
  m_nearestTile.y = int(std::floor(m_position.y));
  m_shaderProps->m_diffuseColour = colours[int(float(std::rand()) / float(RAND_MAX) * 5.f)] * 0.00392156862745f * (float(std::rand()) / float(RAND_MAX) * 0.5f + 0.5f);
  m_currentSection = m_nearestTile.x / 32 + 32 * (m_nearestTile.y / 32);
}

glm::vec2 Peep::getPosition() const
{
  return {m_position.x, m_position.y};
}

glm::ivec2 Peep::getGridPosition() const
{
  return {m_nearestTile.x, m_nearestTile.y};
}

ShaderProps* Peep::getShaderProps() const
{
  return m_shaderProps;
}

void Peep::update()
{
//  if (glm::length(m_direction) > 1.f) {std::cout<<"Direction length = "<<glm::length(m_direction)<<".  Normalising...\n";glm::normalize(m_direction);}
  m_hunger -= (float(std::rand()) / float(RAND_MAX)) * 0.004;
  m_hygiene -= (float(std::rand()) / float(RAND_MAX)) * 0.004;
  if (m_hunger < 0.f) {m_hunger = 0.f;}
  if (m_hygiene < 0.f) {m_hygiene = 0.f;}
//  std::cout<<"DIRECTION = "<<glm::to_string(m_direction)<<'\n';
//  std::cout<<"LEN DIR   = "<<glm::length(m_direction)<<'\n';
  m_velocity = 0.1f * m_direction;
  m_position += m_velocity;
  m_nearestTile.x = int(std::floor(m_position.x));
  m_nearestTile.y = int(std::floor(m_position.y));
//  std::cout<<"CURRENTLY AT "<<glm::to_string(m_position)<<'\n';
//  std::cout<<"CURRENT DEST "<<glm::to_string(m_currentDestinationTile)<<'\n';
//  std::cout<<"traversing junction = "<<m_traversingJunction<<'\n';
  if (!m_traversingJunction)
  {
    if (m_position.x >= m_currentDestinationTile.x + 0.45f && m_position.x <= m_currentDestinationTile.x + 0.55f &&
        m_position.y >= m_currentDestinationTile.y + 0.45f && m_position.y <= m_currentDestinationTile.y + 0.55f)
    {
      if (m_path.sections.size() > 1)
      {
        m_path.sections.pop_back();
        m_path.pairs.pop_back();
        m_oldSection = m_currentSection;
        m_currentGoalIndex = m_path.pairs.back()[1];
        m_currentSection = m_path.sections.back();
        m_currentDestinationTile = {m_currentGoalIndex % 8 + (8 * (m_currentSection % 32)), (m_currentGoalIndex / 8) + ((m_currentSection / 32) * 8)};
        m_traversingJunction = true;
        m_junctionTile = {m_path.pairs.back()[0] % 8 + (8 * (m_path.sections.back() % 32)), (m_path.pairs.back()[0] / 8) + ((m_path.sections.back() / 32) * 8)};
        m_containerDirty = true;
        m_vecContainerAddIDs.push_back(m_currentSection);
      }
      else
      {
        m_done = true;
        m_shaderProps->m_diffuseColour = glm::vec3(0.f, 1.f, 0.f);
      }
    }
  }
  else
  {
    glm::vec2 peepToTarget {m_junctionTile.x + 0.5f - m_position.x, m_junctionTile.y + 0.5f - m_position.y};
    peepToTarget = glm::normalize(peepToTarget);
    setDirection(peepToTarget);
    if (glm::length(m_direction) > 1.f) {m_direction = glm::normalize(m_direction);}
//    std::cout<<"Peep to Target = "<<glm::to_string(peepToTarget)<<'\n';
    if (m_position.x >= m_junctionTile.x + 0.45f && m_position.x <= m_junctionTile.x + 0.55f &&
        m_position.y >= m_junctionTile.y + 0.45f && m_position.y <= m_junctionTile.y + 0.55f)
    {
      m_vecContainerRemoveIDs.push_back(m_oldSection);
      m_traversingJunction = false;
      m_containerDirty = true;
    }
  }
//  if (m_position.x > 256.f || m_position.x < 0.f || m_position.y > 256.f || m_position.y < 0.f) {std::cout<<"POSITION OUT OF BOUNDS\n";}
//  if (glm::length(m_velocity) > 0.1f) {std::cout<<"SPEED OUT OF BOUNDS\n";}
//  if (glm::length(m_direction) > 1.f) {std::cout<<"DIRECTION OUT OF BOUNDS\n";}
  glm::vec2 delta = m_position - prevPos;
  //std::cout<<"prev to this frame delta "<<glm::to_string(delta)<<'\n';
//  if (glm::length(delta) > 0.2f) {std::cout<<"POSITION CHANGE TOO LARGE!\n";}
  prevPos = m_position;
}

bool Peep::needsPath() const
{
  return !m_hasPath;
}

void Peep::setPath(Path p, glm::ivec2 destTile)
{
//  std::cout<<"\n\n\n\n";
//  for (size_t i = 0; i < p.pairs.size(); i++)
//  {
//    std::cout<<p.pairs[i][0]<<", "<<p.pairs[i][1]<<'\n';
//    std::cout<<glm::to_string(glm::ivec2(p.pairs[i][1] % 8 + (8 * (p.sections[i] % 32)), (p.pairs[i][1] / 8) + ((p.sections[i] / 32) * 8)));
//    std::cout<<"\t";
//    std::cout<<glm::to_string(glm::ivec2(p.pairs[i][0] % 8 + (8 * (p.sections[i] % 32)), (p.pairs[i][0] / 8) + ((p.sections[i] / 32) * 8)));
//    std::cout<<'\n';
//  }

  m_path = p;
  m_hasPath = true;
  m_currentGoalIndex = p.pairs.back()[1];
  m_currentSection = p.sections.back();
  m_containerDirty = true;
  m_destinationIndex = p.pairs[0][1];
  m_destinationTile = destTile;
  m_currentDestinationTile = {m_currentGoalIndex % 8 + (8 * (m_currentSection % 32)), (m_currentGoalIndex / 8) + ((m_currentSection / 32) * 8)};
}

int Peep::getDestinationIndex() const
{
  return m_destinationIndex;
}

int Peep::getCurrentSection() const
{
  return m_currentSection;
}

int Peep::getLocalGoalIndex() const
{
  return m_currentGoalIndex;
}

void Peep::setDirection(glm::vec2 direction)
{
  m_direction += direction;
  if (glm::length(m_direction) > 1.f) {m_direction = glm::normalize(m_direction);}
}

glm::ivec2 Peep::getDestinationTile() const
{
  return m_destinationTile;
}

bool Peep::isTraversingJunction() const
{
  return m_traversingJunction;
}

bool Peep::isDone() const
{
  return m_done;
}

std::vector<int> Peep::getNewContainers() const
{
  return m_vecContainerAddIDs;
}

std::vector<int> Peep::getOldContainers() const
{
  return m_vecContainerRemoveIDs;
}

bool Peep::containerIsDirty() const
{
  return m_containerDirty;
}

void Peep::makeContainerClean()
{
  m_containerDirty = false;
  m_vecContainerAddIDs.clear();
  m_vecContainerRemoveIDs.clear();
}

int Peep::getNeediestNeed() const
{
  if (m_hunger < m_hygiene)
  {
    return 2;
  }
  else
  {
    return 3;
  }
}
