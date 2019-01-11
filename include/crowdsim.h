#ifndef CROWDSIM_H
#define CROWDSIM_H

#include "peep.h"
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <thread>
#include <glm/gtx/string_cast.hpp>
#include "map.h"

class CrowdSim
{
  public:
    CrowdSim();
    void update();
    void calculateRoutes();
    enum crowdsimEnum {numPeeps = 2500};
    std::array<Peep*, numPeeps> getPeeps() const;
  private:
    std::array<Peep*, numPeeps> m_arrPeeps;
    std::array<std::vector<Peep*>, 1024> m_peepSectionMap;
    bool m_gridDone = false;
    bool m_gridRunning = false;
    bool m_linkerDone = false;
    bool m_linkerRunning = false;
    int m_mapsDone = 0;
    int m_totalMaps = 0;
    int m_junctionsDone = 0;
    int m_totalJunctions = 0;
    bool m_etaDirty = false;
    enum m_directionEnum {d0 = 9, dN = 1, dNE = 2, dE = 3, dSE = 4, dS = 5, dSW = 6, dW = 7, dNW = 8, dNone = 0, dNoRoute = 10};
    struct node
    {
      bool unvisited = true;
      bool enabled = true;
      glm::ivec2 loc;
      int locX() const;
      int locY() const;
      float dist = FLT_MAX;
      std::vector<node*> nbrs;
      std::vector<node*> flowNbrs;
    };
    struct junction
    {
      glm::ivec2 nodeLocA;
      int sectionA;
      glm::ivec2 nodeLocB;
      int sectionB;
      float dist = FLT_MAX;
      std::vector<int> nbrs;
      std::vector<int> nbrsSection;
      int pJunc = INT_MAX;
      bool unvisited = true;
      int junctionID;
      junction();
    };
    struct pathStorage
    {
      typedef std::array<glm::vec2, 64> singleFlow;
      typedef std::array<singleFlow, 64> sectionFlow;
      typedef std::vector<sectionFlow> allFlow;
      typedef std::array<std::array<std::vector<node>, 64>, 1024> setNodes;
      typedef std::array<junction, 1984> junctionMap;
      typedef std::vector<junctionMap> linkerMap;
      typedef std::vector<junction*> linkerChain;
      singleFlow currentFlow;
      sectionFlow currentSectionFlow;
      allFlow allFlows;
      setNodes nodeSet;
      linkerMap linker;
      std::array<std::array<linkerChain, 1984>, 1984> linkerChains;
      std::array<std::array<std::vector<int>, 1984>, 1984> linkerSectionChain;
      std::array<junction, 1984> referenceJunctions;
    };
    static int m_junctionCounter;
    pathStorage m_paths;
    std::array<std::vector<int>, 1024> m_sectionMap;
    std::array<Peep*, numPeeps> initPeeps();
    std::vector<node> dijkstraGrid(int sectionNo, int sourceX, int sourceY);
    std::array<std::vector<node>, 64> calculateMap(int sectionNo);
    pathStorage::singleFlow generateFlow(std::vector<node> nodes, int goalX, int goalY);
    void spinDijkstra();
    void spinLinker();
    void generateLinkerMap();
    void getLinkerChain(int startJunction, int endJunction);
    float vectorLength(glm::vec2 a);
    pathStorage::junctionMap dijkstraLinker(int sourceJunction);
    int convertVec2ToIndex(glm::ivec2 vec);
    Path getPath(glm::ivec2 sourceGlobal, glm::ivec2 destinGlobal);
    glm::ivec2 getRandomPoint(Peep *peep);
    glm::vec2 interpolateFlowVelocity(int section, glm::vec2 currentPosition, int currentIndex, int destIndex) const;
    std::array<std::array<std::vector<glm::ivec2>, 1024>, 4> m_lightSectionMap;
    int m_lightSetABUpdates = 600;
    int m_lightSetCDUpdates = 600;
    bool m_lightSetAActive = true;
    bool m_lightSetBActive = false;
    bool m_lightSetCActive = true;
    bool m_lightSetDActive = false;
};

#endif
