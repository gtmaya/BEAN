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
    ShaderProps* m_shaderProps;
    glm::vec2 m_position;
    glm::ivec2 m_nearestTile;
    glm::ivec2 m_destinationTile;
    glm::ivec2 m_currentDestinationTile;
    glm::ivec2 m_junctionTile;
    glm::vec2 m_velocity;
    glm::vec2 m_direction;
    float m_speed;
    int m_activeNeed;
    float m_hygiene = 1.f;
    float m_hunger = 1.f;
    bool m_hasPath = false;
    bool m_traversingJunction = false;
    bool m_isFollowing = false;
    Peep* m_friend = nullptr;
    std::vector<Peep*> m_followers;
    Path m_path;
    int m_currentSection;
    int m_currentGoalIndex;
    int m_destinationIndex;
    bool m_done = false;
    int m_oldSection;
    std::vector<int> m_vecContainerAddIDs;
    std::vector<int> m_vecContainerRemoveIDs;
    std::vector<int> m_vecActiveContainers;
    bool m_containerDirty = true;
    bool m_waitingLight = false;
  public:
    Peep();
    Peep(glm::vec2 _position, Peep* _friend);
    glm::vec2 getPosition() const;
    glm::ivec2 getGridPosition() const;
    glm::ivec2 getDestinationTile() const;
    ShaderProps* getShaderProps() const;
    void update();
    bool needsPath() const;
    void setPath(Path p, glm::ivec2 destTile, int need);
    void setDirection(glm::vec2 direction, bool fromAvoidance);
    void makeContainerClean();
    void addFollower(Peep* peep);
    void setNeedsPath();
    int getDestinationIndex() const;
    int getCurrentSection() const;
    int getLocalGoalIndex() const;
    int getNeediestNeed() const;
    bool isTraversingJunction() const;
    bool isDone() const;
    bool containerIsDirty() const;
    bool isWaiting() const;
    float getHunger() const;
    float getHygiene() const;
    std::vector<int> getNewContainers() const;
    std::vector<int> getOldContainers() const;
    std::vector<Peep*> getFollowers() const;
    Peep* getFriend() const;
    glm::vec2 getDirection() const;
    void setWaitingLight(bool status);
};

#endif
