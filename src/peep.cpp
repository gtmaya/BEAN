#include "peep.h"
#include "map.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

std::array<glm::vec3, 5> colours {glm::vec3(27, 153, 139), glm::vec3(255, 255, 220), glm::vec3(255, 253, 130), glm::vec3(255, 155, 113), glm::vec3(232, 72, 85)};

Peep::Peep(){}

Peep::Peep(glm::vec2 _position, Peep* _friend) : m_shaderProps  (new ShaderProps({1.f, 0.f, 1.f}, {1.f, 1.f, 1.f}, 1.f, 0.1f, 1.f, 0.f, 1.f, 0)),
                                                 m_position     (_position),
                                                 m_velocity     ({0.f, 0.f}),
                                                 m_direction    ({0.f, 0.f}),
                                                 m_friend       (_friend)
{
  m_hunger -= (float(std::rand()) / float(RAND_MAX)) * 0.5f;
  m_hygiene -= (float(std::rand()) / float(RAND_MAX)) * 0.5f;
  m_speed = (float(std::rand()) / float(RAND_MAX)) * 0.05f + 0.05f;
  if (m_friend != nullptr)
  {
    m_isFollowing = true;
    m_friend->addFollower(this);
  }
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
  if (!m_isFollowing)
  {
    m_hunger -= (float(std::rand()) / float(RAND_MAX)) * 0.0005f;
    m_hygiene -= (float(std::rand()) / float(RAND_MAX)) * 0.001f;
    if (m_hunger < 0.f) {m_hunger = 0.f;}
    if (m_hygiene < 0.f) {m_hygiene = 0.f;}
    if (m_hunger < 0.f) {m_hunger = 0.f;}
    if (m_hygiene < 0.f) {m_hygiene = 0.f;}
  }
  else
  {
    m_hunger = m_friend->getHunger();
    m_hygiene = m_friend->getHygiene();
  }

  m_velocity = m_speed * m_direction;
  m_position += m_velocity;
  m_nearestTile.x = int(std::floor(m_position.x));
  m_nearestTile.y = int(std::floor(m_position.y));
  if (!m_traversingJunction)
  {
    if (m_position.x >= m_currentDestinationTile.x + 0.41f && m_position.x <= m_currentDestinationTile.x + 0.59f &&
        m_position.y >= m_currentDestinationTile.y + 0.41f && m_position.y <= m_currentDestinationTile.y + 0.59f)
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
        if (m_isFollowing)
        {
          m_done = false;
          m_hasPath = false;
        }
        else if (m_activeNeed == 2)
        {
          m_hunger += 0.01f;
          m_hygiene -= (float(std::rand()) / float(RAND_MAX)) * 0.001f;
          if (m_hunger > 1.f)
          {
            m_done = false;
            m_hasPath = false;
          }
        }
        else if (m_activeNeed == 3)
        {
          m_hygiene += 0.01f;
          if (m_hygiene > 1.f)
          {
            m_done = false;
            m_hasPath = false;
            for (auto &f : m_followers)
            {
              f->setNeedsPath();
            }
          }
        }
      }
    }
  }
  else
  {
    glm::vec2 peepToTarget {m_junctionTile.x + 0.5f - m_position.x, m_junctionTile.y + 0.5f - m_position.y};
    peepToTarget = glm::normalize(peepToTarget);
    setDirection(peepToTarget, false);
    if (glm::length(m_direction) > 1.f) {m_direction = glm::normalize(m_direction);}
    if (m_position.x >= m_junctionTile.x + 0.41f && m_position.x <= m_junctionTile.x + 0.59f &&
        m_position.y >= m_junctionTile.y + 0.41f && m_position.y <= m_junctionTile.y + 0.59f)
    {
      m_vecContainerRemoveIDs.push_back(m_oldSection);
      m_traversingJunction = false;
      m_containerDirty = true;
    }
  }
  m_velocity *= 0.9f;
  m_direction *= 0.9f;
}

bool Peep::needsPath() const
{
  return !m_hasPath;
}

void Peep::setPath(Path p, glm::ivec2 destTile, int need)
{
  m_path = p;
  m_hasPath = true;
  m_currentGoalIndex = p.pairs.back()[1];
  m_currentSection = p.sections.back();
  m_destinationIndex = p.pairs[0][1];
  m_destinationTile = destTile;
  m_currentDestinationTile = {m_currentGoalIndex % 8 + (8 * (m_currentSection % 32)), (m_currentGoalIndex / 8) + ((m_currentSection / 32) * 8)};
  m_activeNeed = need;
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

void Peep::setDirection(glm::vec2 direction, bool fromAvoidance)
{
  if (m_waitingLight && !fromAvoidance) {return;}
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

glm::vec2 Peep::getDirection() const
{
  return m_direction;
}

Peep* Peep::getFriend() const
{
  return m_friend;
}

std::vector<Peep*> Peep::getFollowers() const
{
  return m_followers;
}

void Peep::addFollower(Peep* peep)
{
  m_followers.push_back(peep);
}

float Peep::getHunger() const
{
  return m_hunger;
}

float Peep::getHygiene() const
{
  return m_hygiene;
}

void Peep::setNeedsPath()
{
  m_hasPath = false;
  m_done = false;
}

void Peep::setWaitingLight(bool status)
{
  m_waitingLight = status;
}

bool Peep::isWaiting() const
{
  return m_waitingLight;
}
