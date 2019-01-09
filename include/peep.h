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
    int m_framesDone = 0;
    glm::vec2 m_position;
    glm::vec2 prevPos {0.f, 0.f};
    glm::ivec2 m_nearestTile;
    glm::ivec2 m_destinationTile;
    glm::ivec2 m_currentDestinationTile;
    glm::ivec2 m_junctionTile;
    glm::vec2 m_velocity;
    float m_speed;
    float m_hygiene = 1.f;
    float m_hunger = 1.f;
    bool m_isFollowing;
    Peep* m_friend;
    glm::vec2 m_direction;
    ShaderProps* m_shaderProps;
    bool m_hasPath = false;
    bool m_traversingJunction = false;
    Path m_path;
    int m_currentSection;
    int m_currentGoalIndex;
    int m_destinationIndex;
    static int temp;
    bool m_done = false;
    int m_oldSection;
    std::vector<int> m_vecContainerAddIDs;
    std::vector<int> m_vecContainerRemoveIDs;
    std::vector<int> m_vecActiveContainers;
    bool m_containerDirty = true;
  public:
    glm::vec2 getPosition() const;
    glm::ivec2 getGridPosition() const;
    glm::ivec2 getDestinationTile() const;
    ShaderProps* getShaderProps() const;
    Peep();
    void update();
    bool needsPath() const;
    void setPath(Path p, glm::ivec2 destTile);
    int getDestinationIndex() const;
    int getCurrentSection() const;
    int getLocalGoalIndex() const;
    void setDirection(glm::vec2 direction);
    bool isTraversingJunction() const;
    bool isDone() const;
    std::vector<int> getNewContainers() const;
    std::vector<int> getOldContainers() const;
    bool containerIsDirty() const;
    void makeContainerClean();
    int getNeediestNeed() const;
    glm::vec2 getDirection() const;
};

#endif
