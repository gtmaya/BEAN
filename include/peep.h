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
    glm::ivec2 m_currentDestinationTile;
    glm::ivec2 m_junctionTile;
    glm::vec2 m_velocity;
    ShaderProps* m_shaderProps;
    bool m_hasPath = false;
    bool m_traversingJunction = false;
    Path m_path;
    int m_currentSection;
    int m_currentGoalIndex;
    int m_destinationIndex;
    static int temp;
    bool m_done = false;
  public:
    glm::vec2 getPosition() const;
    glm::ivec2 getGridPosition() const;
    glm::ivec2 getDestinationTile() const;
    ShaderProps* getShaderProps() const;
    Peep();
    void update();
    bool needsPath() const;
    void setPath(Path p, glm::ivec2 destTile);
    void pathComplete();
    int getDestinationIndex() const;
    int getCurrentSection() const;
    int getLocalGoalIndex() const;
    void setVelocity(glm::vec2 velocity);
    bool isTraversingJunction() const;
    bool isDone() const;
};

#endif
