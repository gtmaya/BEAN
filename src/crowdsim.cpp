#include "crowdsim.h"

CrowdSim::CrowdSim() : m_arrPeeps (initPeeps())
{}

std::array<Peep, CrowdSim::numPeeps> CrowdSim::getPeeps() const
{
  return m_arrPeeps;
}

std::array<Peep, CrowdSim::numPeeps> CrowdSim::initPeeps()
{
  std::array<Peep, numPeeps> peeps;
  for (size_t i = 0; i < numPeeps; i++)
  {
    peeps[i] = Peep();
  }
  return peeps;
}

glm::ivec2 CrowdSim::getRandomPoint()
{
  return {int((float(std::rand()) / float(RAND_MAX)) * 254 + 1), int((float(std::rand()) / float(RAND_MAX)) * 254 + 1)};
}

//CURRENTLY
void CrowdSim::update()
{
  for (size_t i = 0; i < numPeeps; i++)
  {
    if (m_arrPeeps[i].getGridPosition() == m_arrPeeps[i].getDestinationTile()) {m_arrPeeps[i].pathComplete();}
    if (m_arrPeeps[i].needsPath())
    {
      glm::ivec2 dest = getRandomPoint();
      m_arrPeeps[i].setPath(getPath(m_arrPeeps[i].getGridPosition(), dest), dest);
    }
    glm::vec2 x = m_paths.allFlows[m_arrPeeps[i].getCurrentSection()][m_arrPeeps[i].getLocalGoalIndex()][convertVec2ToIndex(m_arrPeeps[i].getGridPosition())];
    m_arrPeeps[i].setVelocity(x);
    m_arrPeeps[i].update();
  }
}

int CrowdSim::m_junctionCounter = 0;

int CrowdSim::node::locX() const
{
  return int(loc.x);
}

int CrowdSim::node::locY() const
{
  return int(loc.y);
}

CrowdSim::junction::junction()
{
  junctionID = m_junctionCounter;
  m_junctionCounter++;
}

void CrowdSim::spinDijkstra()
{
  while (!m_gridRunning)
  {
    nanosleep((const struct timespec[]){{0, 25000L}}, NULL);
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  auto t2 = std::chrono::high_resolution_clock::now();
  float delta;
  int percent = 0;
  std::string tString;
  while(!m_gridDone)
  {
    t2 = std::chrono::high_resolution_clock::now();
    delta = float(std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count());
    if (m_etaDirty)
    {
      percent = int((float(m_mapsDone) / float(m_totalMaps)) * 100.f);
      float tpm = delta/float(m_mapsDone);
      float trem = (float(m_totalMaps) - float(m_mapsDone)) * tpm;
      while(true)
      {
        tString = std::to_string(int(trem)) + 's';
        if (trem > 86400)
        {
          int days = int(trem / 86400.f);
          int hours = int(trem / 3600.f) - (days * 24);
          int minutes = int(trem / 60.f) - (hours * 60) - (days * 1440);
          int seconds = int(trem - (minutes * 60) - (hours * 3600) - (days * 86400));
          tString = std::to_string(days) + "d :" + std::to_string(hours) + "h :" + std::to_string(minutes) + "m :" + std::to_string(seconds) + 's';
          break;
        }
        if (trem > 3600)
        {
          int hours = int(trem / 3600.f);
          int minutes = int(trem / 60.f) - (hours * 60);
          int seconds = int(trem - (minutes * 60) - (hours * 3600));
          tString = std::to_string(hours) + "h :" + std::to_string(minutes) + "m :" + std::to_string(seconds) + 's';
          break;
        }
        if (trem > 60)
        {
          int minutes = int(trem / 60.f);
          int seconds = int(trem - (minutes * 60));
          tString = std::to_string(minutes) + "m :" + std::to_string(seconds) + 's';
          break;
        }
        break;
      }
      m_etaDirty = false;
    }

    std::cout << "\rDijkstra ⠁ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rDijkstra ⠂ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rDijkstra ⠄ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rDijkstra ⡀ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rDijkstra ⢀ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rDijkstra ⠠ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rDijkstra ⠐ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rDijkstra ⠈ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")   "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_gridDone) {std::cout<<"\nDijkstra complete.\nCrunching paths...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
  }
}

void CrowdSim::spinLinker()
{
  while (!m_linkerRunning)
  {
    nanosleep((const struct timespec[]){{0, 25000L}}, NULL);
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  auto t2 = std::chrono::high_resolution_clock::now();
  float delta;
  int percent = 0;
  std::__cxx11::string tString;
  while(!m_linkerDone)
  {
    t2 = std::chrono::high_resolution_clock::now();
    delta = float(std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count());
    if (m_etaDirty)
    {
      percent = int((float(m_junctionsDone) / float(m_totalJunctions)) * 100.f);
      float tpm = delta/float(m_junctionsDone);
      float trem = (float(m_totalJunctions) - float(m_junctionsDone)) * tpm;
      while(true)
      {
        tString = std::to_string(int(trem)) + 's';
        if (trem > 86400)
        {
          int days = int(trem / 86400.f);
          int hours = int(trem / 3600.f) - (days * 24);
          int minutes = int(trem / 60.f) - (hours * 60) - (days * 1440);
          int seconds = int(trem - (minutes * 60) - (hours * 3600) - (days * 86400));
          tString = std::to_string(days) + "d :" + std::to_string(hours) + "h :" + std::to_string(minutes) + "m :" + std::to_string(seconds) + 's';
          break;
        }
        if (trem > 3600)
        {
          int hours = int(trem / 3600.f);
          int minutes = int(trem / 60.f) - (hours * 60);
          int seconds = int(trem - (minutes * 60) - (hours * 3600));
          tString = std::to_string(hours) + "h :" + std::to_string(minutes) + "m :" + std::to_string(seconds) + 's';
          break;
        }
        if (trem > 60)
        {
          int minutes = int(trem / 60.f);
          int seconds = int(trem - (minutes * 60));
          tString = std::to_string(minutes) + "m :" + std::to_string(seconds) + 's';
          break;
        }
        break;
      }
      m_etaDirty = false;
    }

    std::cout << "\rLinker ⠁ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rLinker ⠂ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rLinker ⠄ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rLinker ⡀ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rLinker ⢀ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rLinker ⠠ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rLinker ⠐ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
    std::cout << "\rLinker ⠈ (running for "<<std::setprecision(10)<< delta<< "s, "<<percent<<"% complete"<<std::setprecision(6)<<", time remaining: "<<tString<<")     "<<std::flush;
    for (size_t i = 0; i < 100; i++)
    {
      if (m_linkerDone) {std::cout<<"\nLinker complete.\nCrunching links...\n"<<std::flush; return;}
      nanosleep((const struct timespec[]){{0, 1250000L}}, NULL);
    }
  }
}

float CrowdSim::vectorLength(glm::vec2 a)
{
  float dist = sqrt(pow(a.x, 2) + pow(a.y, 2));
  return dist;
}

std::vector<CrowdSim::node> CrowdSim::dijkstraGrid(int sectionNo, int sourceX, int sourceY)
{
  m_gridRunning = true;
  int sectionOffset = sectionNo * 64;
  std::vector<node> vecNodes;
  int numUnvisitedNodes = 8 * 8;

  int y = 0;
  int x = 0;
  for (int i = sectionNo * 64; i < sectionNo * 64 + 64; i++)
  {
    x = i % 8;
    if (i != 0 && x == 0) {y++;}
    node n;
    n.loc.x = x;
    n.loc.y = y;
    if(mapGrid[i] != 1) {n.unvisited = true;}
    else                         {n.unvisited = false; n.enabled = false; numUnvisitedNodes--;}
    vecNodes.push_back(n);
  }

  for (size_t i = 0; i < vecNodes.size(); i++)
  {
    static int y = 0;
    if (i == 0) {y = 0;}
    int x = i % 8;
    if (x == 0 && i != 0) {y++;}

    if (x != 0 && vecNodes[i - 1].unvisited)
    {
      vecNodes[i].nbrs.push_back(&vecNodes[i - 1]);
      vecNodes[i].flowNbrs.push_back(&vecNodes[i - 1]);
      if (y != 0 && vecNodes[(y - 1) * 8 + (x - 1)].unvisited) {vecNodes[i].flowNbrs.push_back(&vecNodes[(y - 1) * 8 + (x - 1)]);}
      if (y != 8 - 1 && vecNodes[(y + 1) * 8 + (x - 1)].unvisited) {vecNodes[i].flowNbrs.push_back(&vecNodes[(y + 1) * 8 + (x - 1)]);}
    }
    if (x != 8 - 1)
    {
      if (vecNodes[i + 1].unvisited) {vecNodes[i].nbrs.push_back(&vecNodes[i + 1]);}
      if (vecNodes[i + 1].unvisited) {vecNodes[i].flowNbrs.push_back(&vecNodes[i + 1]);}
      if (y != 0 && vecNodes[(y - 1) * 8 + (x + 1)].unvisited) {vecNodes[i].flowNbrs.push_back(&vecNodes[(y - 1) * 8 + (x + 1)]);}
      if (y != 8 - 1 && vecNodes[(y + 1) * 8 + (x + 1)].unvisited) {vecNodes[i].flowNbrs.push_back(&vecNodes[(y + 1) * 8 + (x + 1)]);}
    }
    if (y != 0 && vecNodes[(y - 1) * 8 + x].unvisited)
    {
      vecNodes[i].nbrs.push_back(&vecNodes[(y - 1) * 8 + x]);
      vecNodes[i].flowNbrs.push_back(&vecNodes[(y - 1) * 8 + x]);
    }
    if (y != 8 - 1 && vecNodes[(y + 1) * 8 + x].unvisited)
    {
      vecNodes[i].nbrs.push_back(&vecNodes[(y + 1) * 8 + x]);
      vecNodes[i].flowNbrs.push_back(&vecNodes[(y + 1) * 8 + x]);
    }
    if (x == sourceX && y == sourceY)
    {
      for (size_t j = 0; j < vecNodes[i].nbrs.size(); j++)
      {
        vecNodes[i].nbrs[j]->dist = 1;
        vecNodes[i].dist = 0;
      }
    }
  }

  int startIndex = (sourceY * 8) + sourceX;
  int startNumRemNodes = numUnvisitedNodes;
  while (numUnvisitedNodes != 0)
  {
    int index = INT_MAX;
    for (size_t i = 0; i < vecNodes.size(); i++)
    {
      if (!vecNodes[i].unvisited) {continue;}
      if (index == INT_MAX)
      {
        index = i;
        continue;
      }
      if (vecNodes[i].dist < vecNodes[index].dist)
      {
        index = i;
      }
    }
    vecNodes[index].unvisited = false;

    for (size_t i = 0; i < vecNodes[index].nbrs.size(); i++)
    {
      float alt = vecNodes[index].dist + vectorLength(vecNodes[index].loc - vecNodes[index].nbrs[i]->loc);
      if (alt < vecNodes[index].nbrs[i]->dist)
      {
        vecNodes[index].nbrs[i]->dist = alt;
      }
    }
    numUnvisitedNodes--;
  }
  return vecNodes;
}

std::array<std::vector<CrowdSim::node>, 64> CrowdSim::calculateMap(int sectionNo)
{
  std::array<std::vector<node>, 64> allNodes;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      allNodes[j * 8 + i] = dijkstraGrid(sectionNo, i, j);
      m_mapsDone++;
      m_etaDirty = true;
    }
  }
  return allNodes;
}

CrowdSim::pathStorage::singleFlow CrowdSim::generateFlow(std::vector<node> nodes, int goalX, int goalY)
{
  pathStorage::singleFlow flow;
  int x = 0;
  int y = 0;
  for (int i = 0; i < 64; i++)
  {
    int dX;
    int dY;
    x = i % 8;
    if (i != 0 && x == 0) {y++;}
    if (nodes[i].enabled == false || nodes[i].flowNbrs.size() == 0)
    {
      flow[i] = {0.f, 0.f};
    }
    else if (nodes[i].dist == FLT_MAX)
    {
      flow[i] = {FLT_MAX, FLT_MAX};
    }
    else
    {
      if (x == goalX && y == goalY)
      {
        flow[i] = {0.f, 0.f};
      }
      else
      {
        float minDist = FLT_MAX;
        int index = INT_MAX;
        for (size_t j = 0; j < nodes[i].flowNbrs.size(); j++)
        {
          glm::vec2 dVector = nodes[i].flowNbrs[j]->loc - nodes[i].loc;
          float dist = nodes[i].flowNbrs[j]->dist;
          minDist = std::min(dist, minDist);
          if (minDist == dist) {index = j;}
        }
        dX = nodes[i].flowNbrs[index]->locX() - nodes[i].locX();
        dY = nodes[i].flowNbrs[index]->locY() - nodes[i].locY();
        flow[i] = {dX, dY};
      }
    }
  }
  return flow;
}

void CrowdSim::getLinkerChain(int startJunction, int endJunction)
{
  pathStorage::linkerChain chain;
  std::vector<int> sections;
  chain.push_back(&m_paths.linker[startJunction][endJunction]);
  bool doneChain = false;
  int pJIndex = m_paths.linker[startJunction][endJunction].pJunc;
  while (!doneChain)
  {
    if (pJIndex == INT_MAX) {doneChain = true; break;}
    chain.push_back(&m_paths.linker[startJunction][pJIndex]);
    pJIndex = m_paths.linker[startJunction][pJIndex].pJunc;
  }
  chain.push_back(&m_paths.linker[startJunction][startJunction]);
  for (size_t i = 0; i < chain.size() - 1; i++)
  {
    int commonSection = INT_MAX;
    if (chain[i]->sectionA == chain[i + 1]->sectionA) {sections.push_back(chain[i]->sectionA); continue;}
    if (chain[i]->sectionA == chain[i + 1]->sectionB) {sections.push_back(chain[i]->sectionA); continue;}
    if (chain[i]->sectionB == chain[i + 1]->sectionA) {sections.push_back(chain[i]->sectionB); continue;}
    if (chain[i]->sectionB == chain[i + 1]->sectionB) {sections.push_back(chain[i]->sectionB); continue;}
  }
  m_paths.linkerSectionChain[startJunction][endJunction] = sections;
  m_paths.linkerChains[startJunction][endJunction] = chain;
}

CrowdSim::pathStorage::junctionMap CrowdSim::dijkstraLinker(int sourceJunction)
{
  std::array<junction, 1984> junctions = m_paths.referenceJunctions;
  int numUnvisitedJunctions = 1984;

  for (size_t i = 0; i < junctions[sourceJunction].nbrs.size(); i++)
  {
    int commonSection = INT_MAX;
    int activeIndexSource = INT_MAX;
    int activeIndexNeighbour = INT_MAX;
    junctions[sourceJunction].dist = 0.f;
    if (junctions[sourceJunction].sectionA == junctions[junctions[sourceJunction].nbrs[i]].sectionA)
    {
      commonSection = junctions[sourceJunction].sectionA;
      activeIndexSource = (junctions[sourceJunction].nodeLocA.x % 8) + (8 * (junctions[sourceJunction].nodeLocA.y % 8));
      activeIndexNeighbour = (junctions[junctions[sourceJunction].nbrs[i]].nodeLocA.x % 8) + (8 * (junctions[junctions[sourceJunction].nbrs[i]].nodeLocA.y % 8));
    }
    if (junctions[sourceJunction].sectionA == junctions[junctions[sourceJunction].nbrs[i]].sectionB)
    {
      commonSection = junctions[sourceJunction].sectionA;
      activeIndexSource = (junctions[sourceJunction].nodeLocA.x % 8) + (8 * (junctions[sourceJunction].nodeLocA.y % 8));
      activeIndexNeighbour = (junctions[junctions[sourceJunction].nbrs[i]].nodeLocB.x % 8) + (8 * (junctions[junctions[sourceJunction].nbrs[i]].nodeLocB.y % 8));
    }
    if (junctions[sourceJunction].sectionB == junctions[junctions[sourceJunction].nbrs[i]].sectionA)
    {
      commonSection = junctions[sourceJunction].sectionB;
      activeIndexSource = (junctions[sourceJunction].nodeLocB.x % 8) + (8 * (junctions[sourceJunction].nodeLocB.y % 8));
      activeIndexNeighbour = (junctions[junctions[sourceJunction].nbrs[i]].nodeLocA.x % 8) + (8 * (junctions[junctions[sourceJunction].nbrs[i]].nodeLocA.y % 8));
    }
    if (junctions[sourceJunction].sectionB == junctions[junctions[sourceJunction].nbrs[i]].sectionB)
    {
      commonSection = junctions[sourceJunction].sectionB;
      activeIndexSource = (junctions[sourceJunction].nodeLocB.x % 8) + (8 * (junctions[sourceJunction].nodeLocB.y % 8));
      activeIndexNeighbour = (junctions[junctions[sourceJunction].nbrs[i]].nodeLocB.x % 8) + (8 * (junctions[junctions[sourceJunction].nbrs[i]].nodeLocB.y % 8));
    }
    junctions[junctions[sourceJunction].nbrs[i]].dist = m_paths.nodeSet[commonSection][activeIndexSource][activeIndexNeighbour].dist;
  }

  while (numUnvisitedJunctions != 0)
  {
    int index = INT_MAX;
    float minDist = FLT_MAX;
    for (size_t i = 0; i < junctions.size(); i++)
    {
      minDist = std::min(junctions[i].dist, minDist);
      if (!junctions[i].unvisited) {continue;}
      if (index == INT_MAX)
      {
        index = i;
        continue;
      }
      if (junctions[i].dist < junctions[index].dist)
      {
        index = i;
      }
    }
    junctions[index].unvisited = false;

    for (size_t i = 0; i < junctions[index].nbrs.size(); i++)
    {
      int commonSection = INT_MAX;
      int activeIndexSource = INT_MAX;
      int activeIndexNeighbour = INT_MAX;
      if (junctions[index].sectionA == junctions[junctions[index].nbrs[i]].sectionA)
      {
        commonSection = junctions[index].sectionA;
        activeIndexSource = (junctions[index].nodeLocA.x % 8) + (8 * (junctions[index].nodeLocA.y % 8));
        activeIndexNeighbour = (junctions[junctions[index].nbrs[i]].nodeLocA.x % 8) + (8 * (junctions[junctions[index].nbrs[i]].nodeLocA.y % 8));
      }
      if (junctions[index].sectionA == junctions[junctions[index].nbrs[i]].sectionB)
      {
        commonSection = junctions[index].sectionA;
        activeIndexSource = (junctions[index].nodeLocA.x % 8) + (8 * (junctions[index].nodeLocA.y % 8));
        activeIndexNeighbour = (junctions[junctions[index].nbrs[i]].nodeLocB.x % 8) + (8 * (junctions[junctions[index].nbrs[i]].nodeLocB.y % 8));
      }
      if (junctions[index].sectionB == junctions[junctions[index].nbrs[i]].sectionA)
      {
        commonSection = junctions[index].sectionB;
        activeIndexSource = (junctions[index].nodeLocB.x % 8) + (8 * (junctions[index].nodeLocB.y % 8));
        activeIndexNeighbour = (junctions[junctions[index].nbrs[i]].nodeLocA.x % 8) + (8 * (junctions[junctions[index].nbrs[i]].nodeLocA.y % 8));
      }
      if (junctions[index].sectionB == junctions[junctions[index].nbrs[i]].sectionB)
      {
        commonSection = junctions[index].sectionB;
        activeIndexSource = (junctions[index].nodeLocB.x % 8) + (8 * (junctions[index].nodeLocB.y % 8));
        activeIndexNeighbour = (junctions[junctions[index].nbrs[i]].nodeLocB.x % 8) + (8 * (junctions[junctions[index].nbrs[i]].nodeLocB.y % 8));
      }
      float alt = junctions[index].dist + m_paths.nodeSet[commonSection][activeIndexSource][activeIndexNeighbour].dist;
      if (alt < junctions[junctions[index].nbrs[i]].dist)
      {
        junctions[junctions[index].nbrs[i]].dist = alt;
        junctions[junctions[index].nbrs[i]].pJunc = index;
      }
    }
    numUnvisitedJunctions--;
  }
  m_junctionsDone++;
  m_etaDirty = true;
  return junctions;
}

void CrowdSim::generateLinkerMap()
{
  m_linkerRunning = true;
  int junctionCounter = 0;
  for (int j = 0; j < 32; j++)
  {
    for (int i = 0; i < 32; i++)
    {
      if (i != 31)
      {
        glm::ivec2 currentSectionCoordsRight {i * 8 + 7, j * 8 + 3};
        junction junc;
        junc.sectionA = i + j * 32;
        junc.sectionB = junc.sectionA + 1;
        junc.nodeLocA = currentSectionCoordsRight;
        junc.nodeLocB = currentSectionCoordsRight;
        junc.nodeLocB.x += 1;
        m_paths.referenceJunctions[junctionCounter] = junc;
        m_sectionMap[junc.sectionA].push_back(junctionCounter);
        m_sectionMap[junc.sectionB].push_back(junctionCounter);
        junctionCounter++;
      }
      if (j != 31)
      {
        glm::ivec2 currentSectionCoordsTop {i * 8 + 3, j * 8 + 7};
        junction junc;
        junc.sectionA = i + j * 32;
        junc.sectionB = junc.sectionA + 32;
        junc.nodeLocA = currentSectionCoordsTop;
        junc.nodeLocB = currentSectionCoordsTop;
        junc.nodeLocB.y += 1;
        m_paths.referenceJunctions[junctionCounter] = junc;
        m_sectionMap[junc.sectionA].push_back(junctionCounter);
        m_sectionMap[junc.sectionB].push_back(junctionCounter);
        junctionCounter++;
      }
    }
  }
  for (int i = 0; i < 1984; i++)
  {
    for (int j = 0; j < 1984; j++)
    {
      if (i == j) {continue;}
      if (m_paths.referenceJunctions[i].sectionA == m_paths.referenceJunctions[j].sectionA ||
          m_paths.referenceJunctions[i].sectionA == m_paths.referenceJunctions[j].sectionB ||
          m_paths.referenceJunctions[i].sectionB == m_paths.referenceJunctions[j].sectionA ||
          m_paths.referenceJunctions[i].sectionB == m_paths.referenceJunctions[j].sectionB)
          {
            bool shouldAdd = true;
            for (size_t k = 0; k < m_paths.referenceJunctions[i].nbrs.size(); k++)
            {
              if (m_paths.referenceJunctions[m_paths.referenceJunctions[i].nbrs[k]].junctionID == m_paths.referenceJunctions[j].junctionID) {shouldAdd = false;}
            }
            m_paths.referenceJunctions[i].nbrs.push_back(j);
          }
    }
  }
  for (size_t i = 0; i < 1984; i++) {m_paths.linker.push_back(dijkstraLinker(i));}
  m_linkerDone = true;
  for (size_t i = 0; i < 1984; i++)
  {
    for (size_t j = 0; j < 1984; j++)
    {
      getLinkerChain(i, j);
    }
  }
}

void CrowdSim::calculateRoutes(bool printFlag)
{
  auto t1 = std::chrono::high_resolution_clock::now();
  m_totalMaps = 8 * 8 * 1024;
  std::thread dijkstraPrint(&CrowdSim::spinDijkstra, this);
  for (int i = 0; i < 1024; i++) {m_paths.nodeSet[i] = calculateMap(i);}
  m_gridDone = true;
  for (int section = 0; section < 1024; section++)
  {
    int x = 0;
    int y = 0;
    for (int flowInSection = 0; flowInSection < 64; flowInSection++)
    {
      x = flowInSection % 8;
      if (flowInSection != 0 && x == 0) {y++;}
      if (m_paths.nodeSet[section][flowInSection][flowInSection].enabled) {m_paths.currentFlow = generateFlow(m_paths.nodeSet[section][flowInSection], x, y);}
      else {m_paths.currentFlow = pathStorage::singleFlow();}
      m_paths.currentSectionFlow[flowInSection] = m_paths.currentFlow;
    }
    m_paths.allFlows.push_back(m_paths.currentSectionFlow);
  }
  m_totalJunctions = 1984;
  std::thread linkerPrint(&CrowdSim::spinLinker, this);
  generateLinkerMap();
  dijkstraPrint.join();
  linkerPrint.join();
  auto t2 = std::chrono::high_resolution_clock::now();
  float delta = float(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
  std::cout<<std::setprecision(10)<<"Total time taken to calculate map: "<<delta / 1000.f<<'\n';
}

int CrowdSim::convertVec2ToIndex(glm::ivec2 vec)
{
  return (vec.x % 8) + (8 * (vec.y % 8));
}

Path CrowdSim::getPath(glm::ivec2 sourceGlobal, glm::ivec2 destinGlobal)
{
  glm::ivec2 sourceLocal {sourceGlobal.x % 8, sourceGlobal.y % 8};
  glm::ivec2 destinLocal {destinGlobal.x % 8, destinGlobal.y % 8};
  int sourceSection = sourceGlobal.x / 8 + ((sourceGlobal.y / 8) * 32);
  int destinSection = destinGlobal.x / 8 + ((destinGlobal.y / 8) * 32);
  int sourceIndex = sourceLocal.x + sourceLocal.y * 8;
  int destinIndex = destinLocal.x + destinLocal.y * 8;

  if (sourceSection == destinSection)
  {
    Path returnPath;
    returnPath.pairs.push_back({sourceIndex, destinIndex});
    returnPath.sections.push_back(sourceSection);
    return returnPath;
  }

  float shortestDist = FLT_MAX;
  int selectedSource = INT_MAX;
  int selectedDestin = INT_MAX;
  bool failed = false;
  for (size_t i = 0; i < m_sectionMap[sourceSection].size(); i++)
  {
    int sourceJunction = m_sectionMap[sourceSection][i];
    glm::ivec2 sourceJunctionCoord;
    if (sourceSection == m_paths.referenceJunctions[sourceJunction].sectionA) {sourceJunctionCoord = m_paths.referenceJunctions[sourceJunction].nodeLocA;}
    if (sourceSection == m_paths.referenceJunctions[sourceJunction].sectionB) {sourceJunctionCoord = m_paths.referenceJunctions[sourceJunction].nodeLocB;}
    float distSourceJunction = m_paths.nodeSet[sourceSection][sourceIndex][convertVec2ToIndex(sourceJunctionCoord)].dist;
    for (size_t j = 0; j < m_sectionMap[destinSection].size(); j++)
    {
      int destinJunction = m_sectionMap[destinSection][j];
      glm::ivec2 destinJunctionCoord;
      if (destinSection == m_paths.referenceJunctions[destinJunction].sectionA) {destinJunctionCoord = m_paths.referenceJunctions[destinJunction].nodeLocA;}
      if (destinSection == m_paths.referenceJunctions[destinJunction].sectionB) {destinJunctionCoord = m_paths.referenceJunctions[destinJunction].nodeLocB;}
      float distDestinJunction = m_paths.nodeSet[destinSection][destinIndex][convertVec2ToIndex(destinJunctionCoord)].dist;
      float juncDist = m_paths.linker[sourceJunction][destinJunction].dist;
      float totalDist = juncDist + distSourceJunction + distDestinJunction;
      if (totalDist < shortestDist)
      {
        shortestDist = totalDist;
        selectedSource = sourceJunction;
        selectedDestin = destinJunction;
      }
    }
  }

  Path returnPath;
  bool nextStartIsSecA;
  junction* endJunction = m_paths.linkerChains[selectedSource][selectedDestin][0];
  std::array<int, 2> cIndexPair;
  if (endJunction->sectionA == destinSection) {cIndexPair[0] = convertVec2ToIndex(endJunction->nodeLocA);}
  else if (endJunction->sectionB == destinSection) {cIndexPair[0] = convertVec2ToIndex(endJunction->nodeLocB);}
  cIndexPair[1] = destinIndex;
  returnPath.sections.push_back(destinSection);
  returnPath.pairs.push_back(cIndexPair);
  for (size_t i = 0; i < m_paths.linkerSectionChain[selectedSource][selectedDestin].size(); i++)
  {
    junction* sJ = m_paths.linkerChains[selectedSource][selectedDestin][i];
    junction* eJ = m_paths.linkerChains[selectedSource][selectedDestin][i + 1];
    if (m_paths.linkerSectionChain[selectedSource][selectedDestin][i]== sJ->sectionA) {cIndexPair[0] = convertVec2ToIndex(sJ->nodeLocA);}
    if (m_paths.linkerSectionChain[selectedSource][selectedDestin][i]== sJ->sectionB) {cIndexPair[0] = convertVec2ToIndex(sJ->nodeLocB);}
    if (m_paths.linkerSectionChain[selectedSource][selectedDestin][i]== eJ->sectionA) {cIndexPair[1] = convertVec2ToIndex(eJ->nodeLocA);}
    if (m_paths.linkerSectionChain[selectedSource][selectedDestin][i]== eJ->sectionB) {cIndexPair[1] = convertVec2ToIndex(eJ->nodeLocB);}
    returnPath.pairs.push_back(cIndexPair);
    returnPath.sections.push_back(m_paths.linkerSectionChain[selectedSource][selectedDestin][i]);
  }
  returnPath.sections.push_back(sourceSection);
  junction* startJunction = m_paths.linkerChains[selectedSource][selectedDestin].back();
  if (startJunction->sectionA == sourceSection) {cIndexPair[1] = convertVec2ToIndex(startJunction->nodeLocA);}
  if (startJunction->sectionB == sourceSection) {cIndexPair[1] = convertVec2ToIndex(startJunction->nodeLocB);}
  cIndexPair[0] = sourceIndex;
  returnPath.pairs.push_back(cIndexPair);
  return returnPath;
}
