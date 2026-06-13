#include <iostream>
#include <fstream>
#include <sstream>
#include <inttypes.h>
#include <memory>
#include <vector>
#include "country.h"
#include "../src/Constants.h"
#include "../GLUGLU/functions/new_line.h"
#include "../GLUGLU/functions/tabulator.h"
#include "../GLUGLU/datastructures/linear/queues/queue/queue.h"
#include "../GLUGLU/functions/distance.h"

country::edge::edge(){}
country::edge::edge(int64_t to, int64_t rev, int64_t capacity, double cost) : to(to), rev(rev), capacity(capacity), cost(cost){}


country::country() : workplaces(namedValues::material::size), houses(namedValues::material::size)
{
    for(int i = 0; i < namedValues::material::size; i++){
        workplaces.insert(dynamicArray<std::unique_ptr<workplace>>(), i);
        houses.insert(dynamicArray<std::unique_ptr<house>>(), i);
    }
}

country::~country(){}

bool country::isEmpty(){
    return workplaces.isEmpty() && houses.isEmpty();
}

dynamicArray<couple<namedValues::material, int64_t>>
country::getActiveWorkplaces()
{
    dynamicArray<couple<namedValues::material, int64_t>> active;

    for(int64_t materialType = 0;
        materialType < namedValues::material::size;
        materialType++)
    {
        for(int64_t i = 0;
            i <= workplaces[materialType].getLastIndex();
            i++)
        {
            if(!workplaces[materialType][i]->isActive())
                continue;

            active.append(
                {
                    static_cast<namedValues::material>(materialType),
                    i
                }
            );
        }
    }

    return active;
}

couple<namedValues::material, int64_t> country::findStartingPointOfRim(const dynamicArray<couple<namedValues::material, int64_t>>& active){
    couple<namedValues::material, int64_t> best = active[0];

    for(int64_t i = 1; i <= active.getLastIndex();i++)
    {
        workplace* current = workplaces[active[i].valueA][active[i].valueB].get();
        workplace* lowest =workplaces[best.valueA][best.valueB].get();

        if(current->coordinates[namedValues::axis::Y] < lowest->coordinates[namedValues::axis::Y]){best = active[i];}
        else if(current->coordinates[namedValues::axis::Y]==lowest->coordinates[namedValues::axis::Y]){
            if(current->coordinates[namedValues::axis::X]<lowest->coordinates[namedValues::axis::X])best = active[i];
        }
    }
    return best;
}

bool country::constructFromFile(const std::string &file_name){
    std::ifstream file(file_name);

    if(!file.is_open()){
        std::cerr<< "ERROR, NO SUCH FILE"<< std::endl;
        return false;
    }

    std::string line;

    /// WORKPLACES
    for(int materialType = 0;materialType < namedValues::material::size;materialType++){
        int64_t workplaceIndex = 0;

        while(std::getline(file, line)){
            if(line.empty()) continue;
            if(line == ";;") break;

            std::stringstream ss(line);
            std::string token;

            std::getline(ss, token, ';');
            double x = std::stod(token);

            std::getline(ss, token, ';');
            double y = std::stod(token);

            std::getline(ss, token, ';');
            double z = std::stod(token);

            double nx = (x * 2.0 - 1.0);
            double ny = (y * 2.0 - 1.0);
            double nz = (z * 2.0 - 1.0);

            std::getline(ss, token, ';');
            int64_t capacity = static_cast<int64_t>(std::stoll(token));

            workplaces[materialType].append(std::make_unique<workplace>(workplaceIndex, nx, ny, nz, static_cast<namedValues::material>(materialType), capacity));
            workplaceIndex++;
        }
    }

    /// HOUSES
    for(
        int materialType = 0;
        materialType < namedValues::material::size;
        materialType++
    )
    {
        int64_t houseIndex = 0;
        while(std::getline(file, line))
        {
            if(line.empty()) continue;
            if(line == ";;") break;

            std::stringstream ss(line);
            std::string token;

            std::getline(ss, token, ';');
            double x = std::stod(token);

            std::getline(ss, token, ';');
            double y = std::stod(token);

            std::getline(ss, token, ';');
            double z = std::stod(token);

            double nx = (x * 2.0 - 1.0);
            double ny = (y * 2.0 - 1.0);
            double nz = (z * 2.0 - 1.0);

            houses[materialType].append(std::make_unique<house>(houseIndex, nx, ny, nz, static_cast<namedValues::material>(materialType)));
            houseIndex++;
        }
    }
    return true;
}




double country::det(const couple<namedValues::material, int64_t>& p0, const couple<namedValues::material, int64_t>& pi, const couple<namedValues::material, int64_t>& pj)
{
    workplace* wp0 = workplaces[p0.valueA][p0.valueB].get();
    workplace* wpi = workplaces[pi.valueA][pi.valueB].get();
    workplace* wpj = workplaces[pj.valueA][pj.valueB].get();

    return (wpi->coordinates[namedValues::axis::X] - wp0->coordinates[namedValues::axis::X]) * (wpj->coordinates[namedValues::axis::Y] - wp0->coordinates[namedValues::axis::Y])
        - (wpj->coordinates[namedValues::axis::X] - wp0->coordinates[namedValues::axis::X]) * (wpi->coordinates[namedValues::axis::Y] - wp0->coordinates[namedValues::axis::Y]);
}

double country::angleInRelationToP0(int64_t p0Index, int64_t piIndex, workplace* arr){
    return ((arr[p0Index].coordinates[namedValues::axis::X] - arr[piIndex].coordinates[namedValues::axis::X]) / (arr[p0Index].coordinates[namedValues::axis::Y] - arr[piIndex].coordinates[namedValues::axis::Y]));
}

std::unique_ptr<dynamicArray<couple<namedValues::material, int64_t>>> country::constructRimAroundCountry(){
    dynamicArray<couple<namedValues::material, int64_t>> activeLocations = getActiveWorkplaces();
    dynamicArray<couple<namedValues::material, int64_t>> indexes = activeLocations;
    if(activeLocations.getLastIndex() < 2)return nullptr;
    auto p0 = findStartingPointOfRim(activeLocations);
    int64_t p0Index = -1;

    for(int64_t i = 0;i <= indexes.getLastIndex(); i++)
    {
        if(!(indexes[i].valueA == p0.valueA && indexes[i].valueB == p0.valueB)) continue;
        p0Index = i;
        break;
    }

    indexes.swap(0, p0Index);

    for(int64_t i = 1;i <= indexes.getLastIndex();i++)
    {
        for(int64_t j = i + 1; j <= indexes.getLastIndex(); j++)
        {
            double orientation = det(indexes[0], indexes[i], indexes[j]);
            if(orientation < 0)indexes.swap(i, j);
        }
    }

    auto hull =std::make_unique<dynamicArray<couple<namedValues::material, int64_t>>>();
    hull->append(indexes[0]);
    hull->append(indexes[1]);

    for(int64_t i = 2;i <= indexes.getLastIndex();i++)
    {
        while(hull->getLastIndex() >= 1){
            auto top =hull->back();
            auto nextToTop =(*hull)[hull->getLastIndex() - 1];
            if(det(nextToTop,top,indexes[i])<= 0){hull->remove(hull->getLastIndex());}
            else{break;}
        }
        hull->append(indexes[i]);
    }
    return hull;
}

void country::saveActiveWorkplaces(const std::unique_ptr<dynamicArray<couple<namedValues::material, int64_t>>> hull, const std::string saveFileName){
    std::ofstream saveFile(saveFileName);
    for(int i=0; i<=hull->getLastIndex(); i++){
        saveFile << hull->at(i).valueA << ";" << hull->at(i).valueB << '\n';
    }
}

void country::addEdge(dynamicArray<dynamicArray<edge>>& graph, int64_t from, int64_t to, int64_t capacity, double cost){
    edge forward;
    forward.to = to;
    forward.rev = graph[to].getLastIndex() + 1;
    forward.capacity = capacity;
    forward.cost = cost;

    edge backward;
    backward.to = from;
    backward.rev = graph[from].getLastIndex() + 1;
    backward.capacity = 0;
    backward.cost = -cost;

    graph[from].append(forward);
    graph[to].append(backward);
}

bool country::spfa( dynamicArray<dynamicArray<edge>>& graph, int64_t source, int64_t sink, dynamicArray<double>& dist, dynamicArray<int64_t>& parentV, dynamicArray<int64_t>& parentE){
    const double INF = 1e100;//Duża liczba

    int64_t V = graph.getLastIndex() + 1;

    dist.clear();
    parentV.clear();
    parentE.clear();

    dynamicArray<bool> inQueue;

    for(int64_t i = 0; i < V; i++){
        dist.append(INF);
        parentV.append(-1);
        parentE.append(-1);
        inQueue.append(false);
    }

    queue<int64_t> q;

    dist[source] = 0;
    q.append(source);
    inQueue[source] = true;

    while(!q.isEmpty()){
        int64_t v = q.getHead()->val;
        q.dequeue();

        inQueue[v] = false;

        for(int64_t eIndex = 0; eIndex <= graph[v].getLastIndex(); eIndex++)
        {
            edge& e = graph[v][eIndex];

            if(e.capacity <= 0) continue;

            if(dist[v] + e.cost >= dist[e.to]) continue;
            dist[e.to] = dist[v] + e.cost;
            parentV[e.to] = v;
            parentE[e.to] = eIndex;

            if(inQueue[e.to]) continue;
            q.append(e.to);
            inQueue[e.to] = true;
        }
    }

    return parentV[sink] != -1;
}

double country::minCostMaxFlow(dynamicArray<dynamicArray<edge>>& graph, int64_t source, int64_t sink){
    //std::cout << "Calling minCostMaxFlow(" << source << ", " << sink << ")" << std::endl;
    double totalCost = 0.0;

    if(source == sink){
        //std::cerr << "ERROR: source == sink" << std::endl;
        return 0.0;
    }

    dynamicArray<double> dist;
    dynamicArray<int64_t> parentV;
    dynamicArray<int64_t> parentE;

    while(spfa(graph, source, sink, dist, parentV, parentE)){
        int64_t flow = INT64_MAX;

        for( int64_t v = sink; v != source; v = parentV[v]){
            edge& e = graph[parentV[v]][parentE[v]];
            if(e.capacity < flow)flow = e.capacity;
        }
        for(int64_t v = sink; v != source; v = parentV[v]){
            edge& e = graph[parentV[v]][parentE[v]];
            e.capacity -= flow;
            graph[e.to][e.rev].capacity += flow;
            totalCost += flow * e.cost;
        }
    }
    //std::cout << "   TOTAL COST: " << totalCost << std::endl;
    return totalCost;
}

double country::minCostMaxFlow()
{
    dynamicArray<dynamicArray<edge>> graph;

    int64_t source = 0;
    int64_t sink = 0;
    //std::cout << "Calling minCostMaxFlow()" << std::endl;

    return minCostMaxFlow(graph, source, sink);
}

bool country::assignDwarfsToWorkplaces(const std::string& outputFile){
    std::ofstream file(outputFile);

    if(!file.is_open()) return false;

    for(int64_t materialType = 0; materialType < namedValues::material::size; materialType++){
        for(int64_t i = 0; i <= workplaces[materialType].getLastIndex(); i++) workplaces[materialType][i]->amountOfDwarfsWorking = 0;
    }

    for(int64_t materialType = 0; materialType < namedValues::material::size; materialType++){
        int64_t housesCount = houses[materialType].getLastIndex() + 1;

        int64_t workplacesCount = workplaces[materialType].getLastIndex() + 1;


        if(housesCount <= 0 || workplacesCount <= 0){
            file << ";;" << '\n';
            continue;
        }

        int64_t source = 0;
        int64_t houseOffset = 1;

        int64_t workplaceOffset = houseOffset + housesCount;

        int64_t sink = workplaceOffset + workplacesCount;

        int64_t vertices = sink + 1;

        dynamicArray<dynamicArray<edge>> graph;

        for(int64_t i = 0; i < vertices; i++) graph.append(dynamicArray<edge>());

        for(int64_t h = 0; h < housesCount; h++) addEdge(graph, source, houseOffset + h, 1, 0.0 );

        for(int64_t h = 0; h < housesCount; h++){
            house* currentHouse = houses[materialType][h].get();

            for(int64_t w = 0; w < workplacesCount; w++){
                workplace* currentWorkplace = workplaces[materialType][w].get();

                double cost = distance(currentHouse->coordinates, currentWorkplace->coordinates);

                addEdge(graph, houseOffset + h, workplaceOffset + w, 1, cost);
            }
        }

        for(int64_t w = 0; w < workplacesCount; w++){
            addEdge(graph, workplaceOffset + w, sink, workplaces[materialType][w]->capacity, 0.0);
        }

        minCostMaxFlow(graph, source, sink);

        for(int64_t h = 0; h < housesCount; h++){
            int64_t houseVertex = houseOffset + h;

            for(int64_t e = 0;e <= graph[houseVertex].getLastIndex();e++){
                edge& ed = graph[houseVertex][e];

                if(ed.to >= workplaceOffset && ed.to < sink && ed.capacity == 0){
                    int64_t workplaceIndex = ed.to - workplaceOffset;

                    workplaces[materialType][workplaceIndex]->amountOfDwarfsWorking++;

                    file << h << ';' << workplaceIndex << '\n';
                    break;
                }
            }
        }

        file << ";;";

        file << '\n';
    }

    return true;
}

const dynamicArray<dynamicArray<std::unique_ptr<workplace>>>& country::getWorkplaces() const{
    return workplaces;
}

const dynamicArray<dynamicArray<std::unique_ptr<house>>>& country::getHouses() const{
    return houses;
}

void country::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF A COUNTRY" << std::endl;

    tab(tabulation);
    std::cout << "WORKPLACES:" << std::endl;

    for( int materialType = 0; materialType < namedValues::material::size; materialType++){
        tab(tabulation + namedValues::constant::tabulatorIndent);
        std::cout << "MATERIAL: " << materialType << std::endl;

        for(int64_t i = 0; i <= workplaces[materialType].getLastIndex(); i++){
            tab(tabulation + namedValues::constant::tabulatorIndent * 2);
            std::cout << "INDEX: " << workplaces[materialType][i]->index << std::endl;

            tab(tabulation + namedValues::constant::tabulatorIndent * 2);
            std::cout << "CAPACITY: " << workplaces[materialType][i]->capacity << std::endl;

            tab(tabulation + namedValues::constant::tabulatorIndent * 2);
            std::cout << "DWARFS WORKING: " << workplaces[materialType][i]->amountOfDwarfsWorking << std::endl;

            tab(tabulation + namedValues::constant::tabulatorIndent * 2);
            std::cout << "COORDINATES: ";

            for(int a = namedValues::axis::X; a <= namedValues::axis::Z; a++) std::cout << workplaces[materialType][i]->coordinates[a] << " ";

            std::cout << std::endl;
        }
    }

    tab(tabulation);
    std::cout << "HOUSES:" << std::endl;

    for(int materialType = 0; materialType < namedValues::material::size; materialType++)
    {
        tab(tabulation + namedValues::constant::tabulatorIndent);
        std::cout << "FAVORITE MATERIAL: " << materialType << std::endl;

        for(int64_t i = 0; i <= houses[materialType].getLastIndex(); i++){
            tab(tabulation + namedValues::constant::tabulatorIndent * 2);
            std::cout << "INDEX: " << houses[materialType][i]->index << std::endl;

            tab(tabulation + namedValues::constant::tabulatorIndent * 2);
            std::cout << "COORDINATES: ";

            for(int a = namedValues::axis::X; a <= namedValues::axis::Z; a++) std::cout << houses[materialType][i]->coordinates[a] << " ";

            std::cout << std::endl;
        }
    }

    tab(tabulation);
    std::cout << "<<<" << std::endl;
}
