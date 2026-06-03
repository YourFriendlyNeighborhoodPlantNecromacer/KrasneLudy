#ifndef __CONSTANTS__
#define __CONSTANTS__
enum material{
    plants,
    wood,
    clay,
    metal,
    gems,
    garden_gnomes,
    size
};



enum errors{
    out_of_range_error = -1,
    not_found_error = -2,
    file_not_found_error = -3
};

enum constant{
    tabulator_indent = 4
};

enum axis{
    X,
    Y,
    Z
};

#endif//__CONSTANTS__

/*
struct edge{
        int to;
        int rev;
        int capacity;
        int cost;
    };

    void add_edge(
        std::vector<std::vector<edge>>& graph,
        int from,
        int to,
        int capacity,
        int cost
    );

    int solve_mcmf(
        std::vector<std::vector<edge>>& graph,
        int source,
        int sink
    );
*/
