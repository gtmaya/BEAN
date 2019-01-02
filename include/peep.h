#ifndef PEEP_H
#define PEEP_H

#include <glm/vec2.hpp>
#include "shaderhandler.h"

struct Path
{
  std::vector<std::array<int, 2>> pairs;
  std::vector<int> sections;
};

class Peep
{
  private:
    glm::vec2 m_position;
    glm::ivec2 m_nearestTile;
    glm::ivec2 m_destinationTile;
    glm::vec2 m_velocity;
    ShaderProps* m_shaderProps;
    bool m_hasPath = false;
    Path m_path;
    int m_currentSection;
    int m_currentGoalIndex;
    int m_destinationIndex;
    static int temp;
  public:
    glm::vec3 getPosition() const;
    glm::ivec2 getGridPosition() const;
    ShaderProps* getShaderProps() const;
    Peep();
    void update();
    bool needsPath() const;
    void setPath(Path p);
    void pathComplete();
};

#endif
