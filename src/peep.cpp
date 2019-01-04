#include "peep.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

int Peep::temp = 0;

Peep::Peep() : m_position({0.f, 0.f}),
               m_shaderProps (new ShaderProps({1.f, 1.f, 1.f},
                                              {1.f, 1.f, 1.f},
                                              1.f,
                                              0.1f,
                                              1.f,
                                              0.f,
                                              1.f,
                                              0)),
               m_velocity ({0.f, 0.f})
{
  m_position.x = float(std::rand()) / float(RAND_MAX) * 254.f + 1.f;
  m_position.y = float(std::rand()) / float(RAND_MAX) * 254.f + 1.f;
  m_nearestTile.x = int(std::floor(m_position.x));
  m_nearestTile.y = int(std::floor(m_position.y));
  m_shaderProps->m_diffuseColour.r = float(std::rand()) / float(RAND_MAX);
  m_shaderProps->m_diffuseColour.g = float(std::rand()) / float(RAND_MAX);
  m_shaderProps->m_diffuseColour.b = float(std::rand()) / float(RAND_MAX);
  m_velocity *= 0.2;
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
  m_position += m_velocity;
  m_nearestTile.x = int(std::floor(m_position.x));
  m_nearestTile.y = int(std::floor(m_position.y));
  if (!m_traversingJunction)
  {
    if (m_position.x >= m_currentDestinationTile.x + 0.49f && m_position.x <= m_currentDestinationTile.x + 0.51f &&
        m_position.y >= m_currentDestinationTile.y + 0.49f && m_position.y <= m_currentDestinationTile.y + 0.51f)
    {
      if (m_path.sections.size() > 1)
      {
        m_path.sections.pop_back();
        m_path.pairs.pop_back();
        m_currentGoalIndex = m_path.pairs.back()[1];
        m_currentSection = m_path.sections.back();
        m_currentDestinationTile = {m_currentGoalIndex % 8 + (8 * (m_currentSection % 32)), (m_currentGoalIndex / 8) + ((m_currentSection / 32) * 8)};
        m_traversingJunction = true;
        m_junctionTile = {m_path.pairs.back()[0] % 8 + (8 * (m_path.sections.back() % 32)), (m_path.pairs.back()[0] / 8) + ((m_path.sections.back() / 32) * 8)};
      }
      else
      {
        m_done = true;
      }
    }
  }
  else
  {
    glm::vec2 peepToTarget {m_junctionTile.x + 0.5f - m_position.x, m_junctionTile.y + 0.5f - m_position.y};
    peepToTarget = glm::normalize(peepToTarget);
    m_velocity = peepToTarget * 0.1f;
    if (m_position.x >= m_junctionTile.x + 0.49f && m_position.x <= m_junctionTile.x + 0.51f &&
        m_position.y >= m_junctionTile.y + 0.49f && m_position.y <= m_junctionTile.y + 0.51f)
    {
      m_traversingJunction = false;
    }
  }
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
  m_destinationIndex = p.pairs[0][1];
  m_destinationTile = destTile;
  m_currentDestinationTile = {m_currentGoalIndex % 8 + (8 * (m_currentSection % 32)), (m_currentGoalIndex / 8) + ((m_currentSection / 32) * 8)};
}

void Peep::pathComplete()
{
  m_hasPath = false;
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

void Peep::setVelocity(glm::vec2 velocity)
{
  m_velocity.x = velocity.x * 0.1f;
  m_velocity.y = velocity.y * 0.1f;
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
