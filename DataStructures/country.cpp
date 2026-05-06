#include <iostream>
#include <fstream>
#include "country.h"
#include "location.h"
#include "../GLUGLU/functions/new_line.h"
#include "../GLUGLU/functions/tabulator.h"
#include "../GLUGLU/datastructures/linear/queues/queue/queue.h"


country::country() : locations(){}

country::country(int64_t size) : locations(size){}

country::country(const country&) = delete;

country::~country(){
    for(int64_t i = 0; i <= locations.get_last_index(); i++){
        delete locations[i];
    }
}

country& country::operator=(const country&) = delete;

bool country::is_empty(){
    return locations.is_empty();
}

void country::append(location* new_location){
    locations.append(new_location);
}

void country::create_new_location(){
    locations.append(new location(locations.get_last_index()+1));
}

void country::create_new_location(double x_val, double y_val, double z_val){
    locations.append(new location(locations.get_last_index()+1, x_val, y_val, z_val));
}

int64_t country::get_last_index(){
    return locations.get_last_index();
}

bool country::edge_exists(int64_t from, int64_t to){
    if(from < 0 || to < 0 || from > locations.get_last_index() || to > locations.get_last_index()) return false;
    return locations[from]->is_connected_to(to);
}

bool country::make_connection(int64_t from, int64_t to, int64_t maximum_flow){
    if(from < 0 || to < 0 || from > locations.get_last_index() || to > locations.get_last_index()) return false;
    locations[from]->add_road_to(locations[to], maximum_flow);
    return true;
}

dynamic_array<int64_t>* country::find_path(int64_t from, int64_t to){
    if(from == to){
        dynamic_array<int64_t>* shortest_path_array = new dynamic_array<int64_t>();
        shortest_path_array->append(from);
        return (shortest_path_array);
    }
    bool* visited = new bool[locations.get_last_index()]{};
    int64_t* previous_node = new int64_t[locations.get_last_index()];
    previous_node[from] = -1; /// Starting value
    queue<int64_t> to_visit;
    to_visit.append(from);
    int64_t inspected_location_index;
    int64_t grey_node;

    while(!to_visit.is_empty()){
        inspected_location_index = to_visit.get_head()->val;
        if(visited[inspected_location_index]) continue;
        visited[inspected_location_index] = true;
        for(int i=0; i<locations[inspected_location_index]->get_amount_of_roads(); i++){
            grey_node = locations[inspected_location_index]->connected_to[i]->get_destination()->index;
            if(visited[grey_node]) continue;
            previous_node[grey_node] = inspected_location_index;
            if(grey_node == to){
                dynamic_array<int64_t>* shortest_path_array = new dynamic_array<int64_t>();
                int64_t indexed_node = grey_node;
                while(indexed_node != from){
                    shortest_path_array->append(indexed_node);
                    indexed_node = previous_node[indexed_node];
                }
                shortest_path_array->append(from);
                shortest_path_array->reverse_order();
                delete[] previous_node;
                delete[] visited;
                return shortest_path_array;
            }
            to_visit.append(grey_node);
        }
        to_visit.dequeue();
    }
    delete[] visited;
    delete[] previous_node;
    return nullptr;
}

int64_t country::bfs(int64_t from, int64_t to){
    if(from == to) return true;
    bool* visited = new bool[locations.get_last_index()+1]{};
    queue<int64_t> to_visit;
    to_visit.append(from);
    int64_t inspected_location_index;
    int64_t grey_node;
    while(!to_visit.is_empty()){
        inspected_location_index = to_visit.get_head()->val;
        if(visited[inspected_location_index]){
            to_visit.dequeue();
            continue;
        }

        visited[inspected_location_index] = true;
        for(int i=0; i<locations[inspected_location_index]->get_amount_of_roads(); i++){
            grey_node = locations[inspected_location_index]->connected_to[i]->get_destination()->index;
            if(visited[grey_node]) continue;
            if(grey_node == to){
                delete[] visited;
                return true;
            }
            to_visit.append(grey_node);
        }
        to_visit.dequeue();
    }
    delete[] visited;
    return false;
}

bool country::construct_from_file(const std::string &file_name){
    std::ifstream file(file_name);
    if(!file.is_open()){
        std::cerr << "ERROR, NO SUCH FILE" << std::endl;
        return false;
    }
    int64_t amount_of_nodes;
    file >> amount_of_nodes;
    double x_val;
    double y_val;
    double z_val;
    for(int i=0; i<amount_of_nodes; i++){
        file >> x_val >> y_val >> z_val;
        create_new_location(x_val, y_val, z_val);
    }
    int64_t to;
    int64_t weight;
    int64_t amount_of_connections;
    for(int i=0; i<amount_of_nodes; i++){
        file >> amount_of_connections;
        for(int j=0; j<amount_of_connections; j++){
            file >> to >> weight;
            make_connection(i, to, weight);
        }
    }
    return true;
}

int64_t country::find_lowest_point_for_rim(){
    int64_t lowest_left_vertex_index = 0;
    for(int i = 0; i<=locations.get_last_index(); i++){
        if(locations[lowest_left_vertex_index]->coordinates[location::axis::Y] > locations[i]->coordinates[location::axis::Y]) lowest_left_vertex_index = i;
        if(locations[lowest_left_vertex_index]->coordinates[location::axis::Y] > locations[i]->coordinates[location::axis::Y] && locations[lowest_left_vertex_index]->coordinates[location::axis::X] > locations[i]->coordinates[location::axis::X]) lowest_left_vertex_index = i;
    }
    return lowest_left_vertex_index;
}

double det(location* p0, location* pi, location* pj){
    return (pi->coordinates[location::axis::X] - p0->coordinates[location::axis::X]) * (pj->coordinates[location::axis::Y] - p0->coordinates[location::axis::Y]) - (pj->coordinates[location::axis::X] - p0->coordinates[location::axis::X]) * (pi->coordinates[location::axis::Y] - p0->coordinates[location::axis::Y]);
}

dynamic_array<int64_t>* country::polar_compare_for_rim(dynamic_array<int64_t>* array_of_points){
    int64_t p0 = 0;

    double angular_coordinate;
    dynamic_array<double> array_of_assinged_angular_values;

    for(int64_t pi = 1; pi <= locations.get_last_index(); pi++){
         array_of_assinged_angular_values.at(pi) = locations[array_of_points->at(pi)]->coordinates[location::axis::X] / locations[array_of_points->at(pi)]->coordinates[location::axis::Y];
    }



    return nullptr;
}

void country::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF A COUNTRY" << std::endl;
    tab(tabulation);
    std::cout << "SIZE: " << locations.get_size() << std::endl;
    tab(tabulation);
    std::cout << "LAST INDEX: " << locations.get_last_index() << std::endl;
    nl();
    tab(tabulation);
    std::cout << "CHILD LISTS: " << std::endl;
    tab(tabulation);
    for(int i=0; i<=locations.get_last_index(); i++){
        tab(tabulation + 3);
        std::cout << "Location index: " << i << "     its ptr: " << locations[i] << std::endl;
        for(int j=0; j<=locations[i]->connected_to.get_last_index(); j++){
            tab(tabulation + 3);
            locations[i]->connected_to[j]->dp();
        }
        locations[i]->dp(tabulation+3);
        std::cout<< std::endl;
    }
    tab(tabulation);
    std::cout << "<<<" << std::endl;
}
