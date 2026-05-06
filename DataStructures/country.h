#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include "location.h"
#include "../GLUGLU/datastructures/linear/dynamic_array/dynamic_array.h"

class country{
    private:
    dynamic_array<location*> locations;

    public:
    country();
    country(int64_t size);
    country(const country& other);
    ~country();

    country& operator = (const country& other);

    bool is_empty();
    void append(location* new_location);
    void create_new_location();
    void create_new_location(double x_val, double y_val, double z_val);
    int64_t get_last_index();
    bool edge_exists(int64_t from, int64_t to);
    bool make_connection(int64_t from, int64_t to, int64_t maximum_flow);
    bool brake_connection(int64_t from, int64_t to);
    dynamic_array<int64_t>* find_path(int64_t from, int64_t to);
    int64_t bfs(int64_t from, int64_t to);
    bool construct_from_file(const std::string &file_name);

    void dp(int64_t tabulation = 0);

    dynamic_array<int64_t>* create_boarder();

    private:
    int64_t rec_dfs(int64_t current_node, int64_t to, int64_t depth, bool* visited);
    int64_t find_lowest_point_for_rim();
    dynamic_array<int64_t>* polar_compare_for_rim(dynamic_array<int64_t>* array_of_points);
};


#endif//__COUNTRY_H__
