#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include "workplace.h"
#include "house.h"
#include <memory>
#include <string>

#include "../GLUGLU/datastructures/linear/dynamic_array/dynamic_array.h"
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
    dynamic_array<dynamic_array<std::unique_ptr<workplace>>> workplaces;
    dynamic_array<dynamic_array<std::unique_ptr<house>>> houses;

// WORKPLACES[GOLD][3]
// HOUSES[gold][i]
public:
    country();
    country(const country&) = delete;
    country& operator=(const country&) = delete;

    ~country();

    bool construct_from_file(const std::string &file_name);


    dynamic_array<couple<NamedValues::material, int64_t>> get_active_workplaces();
    std::unique_ptr<dynamic_array<couple<NamedValues::material, int64_t>>>construct_rim_around_country();
    couple<NamedValues::material,int64_t> find_starting_point_of_rim(const dynamic_array<couple<NamedValues::material,int64_t>>& active);
    bool is_empty();
    double det(
    const couple<NamedValues::material, int64_t>& p0,
    const couple<NamedValues::material, int64_t>& pi,
    const couple<NamedValues::material, int64_t>& pj
    );
    double angle_in_relation_to_p0(int64_t p0_index, int64_t pi_index, workplace* arr);
    double optimize_material(NamedValues::material m);
    void add_edge(
    dynamic_array<
        dynamic_array<edge>
    >& graph,
    int64_t from,
    int64_t to,
    int64_t capacity,
    double cost
);
    bool assign_dwarfs_to_workplaces(const std::string& output_file);

    bool spfa(dynamic_array<dynamic_array<edge>>& graph,

    int64_t source,
    int64_t sink,

    dynamic_array<double>& dist,
    dynamic_array<int64_t>& parent_v,
    dynamic_array<int64_t>& parent_e
    );

    double min_cost_max_flow(
    dynamic_array<dynamic_array<edge>>& graph,
    int64_t source,
    int64_t sink
    );
    double min_cost_max_flow();
    void dp(int64_t tabulation = 0);
};

#endif
