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
                                 0))
{
  m_position.x = float(std::rand()) / float(RAND_MAX) * 6 + 1;
  m_position.y = float(std::rand()) / float(RAND_MAX) * 6 + 1;
  m_nearestTile.x = int(std::floor(m_position.x));
  m_nearestTile.y = int(std::floor(m_position.y));
  m_shaderProps->m_diffuseColour.r = float(std::rand()) / float(RAND_MAX);
  m_shaderProps->m_diffuseColour.g = float(std::rand()) / float(RAND_MAX);
  m_shaderProps->m_diffuseColour.b = float(std::rand()) / float(RAND_MAX);
  m_velocity *= 0.2;
}

glm::vec3 Peep::getPosition() const
{
  return {m_position.x, 0.f, m_position.y};
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
}

bool Peep::needsPath() const
{
  return !m_hasPath;
}

void Peep::setPath(Path p, glm::ivec2 destTile)
{
  m_path = p;
  m_hasPath = true;
  m_currentGoalIndex = p.pairs.back()[1];
  m_currentSection = p.sections.back();
  m_destinationIndex = p.pairs[0][1];
  m_destinationTile = destTile;
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
  m_velocity.x = velocity.x * 0.01f;
  m_velocity.y = velocity.y * 0.01f;
}

glm::ivec2 Peep::getDestinationTile() const
{
  return m_destinationTile;
}
