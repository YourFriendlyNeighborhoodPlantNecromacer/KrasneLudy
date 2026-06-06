#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include "workplace.h"
#include "house.h"
#include <memory>
#include <string>

#include "../GLUGLU/datastructures/linear/dynamicArray/dynamicArray.h"
#include "../GLUGLU/datastructures/structures/couple/couple.h"
#include "../src/Constants.h"

class country{
private:
    struct edge{
        int64_t to;
        int64_t rev;
        int64_t capacity;
        double cost;

        edge();
        edge(int64_t to, int64_t rev, int64_t capacity, double cost);
    };
    dynamicArray<dynamicArray<std::unique_ptr<workplace>>> workplaces;
    dynamicArray<dynamicArray<std::unique_ptr<house>>> houses;
public:
    country();
    country(const country&) = delete;
    country& operator=(const country&) = delete;

    ~country();

    bool constructFromFile(const std::string &file_name);

    const dynamicArray<dynamicArray<std::unique_ptr<workplace>>>& getWorkplaces() const;
    const dynamicArray<dynamicArray<std::unique_ptr<house>>>& getHouses() const;

    dynamicArray<couple<namedValues::material, int64_t>> getActiveWorkplaces();
    std::unique_ptr<dynamicArray<couple<namedValues::material, int64_t>>>constructRimAroundCountry();
    couple<namedValues::material,int64_t> findStartingPointOfRim(const dynamicArray<couple<namedValues::material,int64_t>>& active);
    bool isEmpty();
    double det(const couple<namedValues::material, int64_t>& p0, const couple<namedValues::material, int64_t>& pi,const couple<namedValues::material, int64_t>& pj);
    double angleInRelationToP0(int64_t p0Index, int64_t piIndex, workplace* arr);
    double optimizeMaterial(namedValues::material m);
    void addEdge(dynamicArray< dynamicArray<edge> >& graph, int64_t from, int64_t to, int64_t capacity, double cost);
    bool assignDwarfsToWorkplaces(const std::string& outputFile);

    bool spfa(dynamicArray<dynamicArray<edge>>& graph, int64_t source, int64_t sink, dynamicArray<double>& dist, dynamicArray<int64_t>& parentV, dynamicArray<int64_t>& parentE);

    double minCostMaxFlow(dynamicArray<dynamicArray<edge>>& graph, int64_t source, int64_t sink);
    double minCostMaxFlow();
    void saveActiveWorkplaces(const std::unique_ptr<dynamicArray<couple<namedValues::material, int64_t>>> hull, const std::string saveFileName);
    void dp(int64_t tabulation = 0);
};

#endif
