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


country::country() : workplaces(NamedValues::material::size), houses(NamedValues::material::size)
{
    for(int i = 0; i < NamedValues::material::size; i++)
    {
        workplaces.insert(dynamic_array<std::unique_ptr<workplace>>(), i);

        houses.insert(dynamic_array<std::unique_ptr<house>>(), i);
    }
}

country::~country(){
}

bool country::is_empty(){
    return workplaces.is_empty() && houses.is_empty();
}

dynamic_array<couple<NamedValues::material, int64_t>>
country::get_active_workplaces()
{
    dynamic_array<couple<NamedValues::material, int64_t>> active;

    for(int64_t material_type = 0;
        material_type < NamedValues::material::size;
        material_type++)
    {
        for(int64_t i = 0;
            i <= workplaces[material_type].get_last_index();
            i++)
        {
            if(!workplaces[material_type][i]->is_active())
                continue;

            active.append(
                {
                    static_cast<NamedValues::material>(material_type),
                    i
                }
            );
        }
    }

    return active;
}

couple<NamedValues::material, int64_t> country::find_starting_point_of_rim(const dynamic_array<couple<NamedValues::material, int64_t>>& active){
    couple<NamedValues::material, int64_t> best = active[0];

    for(int64_t i = 1; i <= active.get_last_index();i++)
    {
        workplace* current = workplaces[active[i].value_a][active[i].value_b].get();
        workplace* lowest =workplaces[best.value_a][best.value_b].get();

        if(current->coordinates[NamedValues::axis::Y] < lowest->coordinates[NamedValues::axis::Y]){best = active[i];}
        else if(current->coordinates[NamedValues::axis::Y]==lowest->coordinates[NamedValues::axis::Y]){
            if(current->coordinates[NamedValues::axis::X]<lowest->coordinates[NamedValues::axis::X])best = active[i];
        }
    }
    return best;
}

bool country::construct_from_file(const std::string &file_name){
    std::ifstream file(file_name);

    if(!file.is_open()){
        std::cerr<< "ERROR, NO SUCH FILE"<< std::endl;
        return false;
    }

    std::string line;

    /// WORKPLACES
    for(int material_type = 0;material_type < NamedValues::material::size;material_type++){
        int64_t workplace_index = 0;

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

            std::getline(ss, token, ';');
            int64_t capacity = static_cast<int64_t>(std::stoll(token));

            workplaces[material_type].append(std::make_unique<workplace>(workplace_index, x, y, z, static_cast<NamedValues::material>(material_type), capacity));
            workplace_index++;
        }
    }

    /// HOUSES
    for(
        int material_type = 0;
        material_type < NamedValues::material::size;
        material_type++
    )
    {
        int64_t house_index = 0;
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

            houses[material_type].append(std::make_unique<house>(house_index, x, y, z, static_cast<NamedValues::material>(material_type)));
            house_index++;
        }
    }
    return true;
}




double country::det(const couple<NamedValues::material, int64_t>& p0, const couple<NamedValues::material, int64_t>& pi, const couple<NamedValues::material, int64_t>& pj)
{
    workplace* wp0 = workplaces[p0.value_a][p0.value_b].get();
    workplace* wpi = workplaces[pi.value_a][pi.value_b].get();
    workplace* wpj = workplaces[pj.value_a][pj.value_b].get();

    return (wpi->coordinates[NamedValues::axis::X] - wp0->coordinates[NamedValues::axis::X]) * (wpj->coordinates[NamedValues::axis::Y] - wp0->coordinates[NamedValues::axis::Y])
        - (wpj->coordinates[NamedValues::axis::X] - wp0->coordinates[NamedValues::axis::X]) * (wpi->coordinates[NamedValues::axis::Y] - wp0->coordinates[NamedValues::axis::Y]);
}

double country::angle_in_relation_to_p0(int64_t p0_index, int64_t pi_index, workplace* arr){
    return ((arr[p0_index].coordinates[NamedValues::axis::X] - arr[pi_index].coordinates[NamedValues::axis::X]) / (arr[p0_index].coordinates[NamedValues::axis::Y] - arr[pi_index].coordinates[NamedValues::axis::Y]));
}

std::unique_ptr<dynamic_array<couple<NamedValues::material, int64_t>>> country::construct_rim_around_country(){
    dynamic_array<couple<NamedValues::material, int64_t>> active_locations = get_active_workplaces();
    dynamic_array<couple<NamedValues::material, int64_t>> indexes = active_locations;
    if(active_locations.get_last_index() < 2)return nullptr;
    auto p0 = find_starting_point_of_rim(active_locations);
    int64_t p0_index = -1;

    for(int64_t i = 0;i <= indexes.get_last_index(); i++)
    {
        if(!(indexes[i].value_a == p0.value_a && indexes[i].value_b == p0.value_b)) continue;
        p0_index = i;
        break;
    }

    indexes.swap(0, p0_index);

    for(int64_t i = 1;i <= indexes.get_last_index();i++)
    {
        for(int64_t j = i + 1; j <= indexes.get_last_index(); j++)
        {
            double orientation = det(indexes[0], indexes[i], indexes[j]);
            if(orientation < 0)indexes.swap(i, j);
        }
    }

    auto hull =std::make_unique<dynamic_array<couple<NamedValues::material, int64_t>>>();
    hull->append(indexes[0]);
    hull->append(indexes[1]);

    for(int64_t i = 2;i <= indexes.get_last_index();i++)
    {
        while(hull->get_last_index() >= 1){
            auto top =hull->back();
            auto next_to_top =(*hull)[hull->get_last_index() - 1];
            if(det(next_to_top,top,indexes[i])<= 0){hull->remove(hull->get_last_index());}
            else{break;}
        }
        hull->append(indexes[i]);
    }
    return hull;
}

void country::add_edge(dynamic_array<dynamic_array<edge>>& graph, int64_t from, int64_t to, int64_t capacity, double cost){
    edge forward;
    forward.to = to;
    forward.rev = graph[to].get_last_index() + 1;
    forward.capacity = capacity;
    forward.cost = cost;

    edge backward;
    backward.to = from;
    backward.rev = graph[from].get_last_index() + 1;
    backward.capacity = 0;
    backward.cost = -cost;

    graph[from].append(forward);
    graph[to].append(backward);
}

bool country::spfa( dynamic_array<dynamic_array<edge>>& graph, int64_t source, int64_t sink, dynamic_array<double>& dist, dynamic_array<int64_t>& parent_v, dynamic_array<int64_t>& parent_e){
    const double INF = 1e100;//Duża liczba

    int64_t V = graph.get_last_index() + 1;

    dist.clear();
    parent_v.clear();
    parent_e.clear();

    dynamic_array<bool> in_queue;

    for(int64_t i = 0; i < V; i++){
        dist.append(INF);
        parent_v.append(-1);
        parent_e.append(-1);
        in_queue.append(false);
    }

    queue<int64_t> q;

    dist[source] = 0;
    q.append(source);
    in_queue[source] = true;

    while(!q.is_empty()){
        int64_t v = q.get_head()->val;
        q.dequeue();

        in_queue[v] = false;

        for(int64_t e_index = 0; e_index <= graph[v].get_last_index(); e_index++)
        {
            edge& e = graph[v][e_index];

            if(e.capacity <= 0) continue;

            if(dist[v] + e.cost >= dist[e.to]) continue;
            dist[e.to] = dist[v] + e.cost;
            parent_v[e.to] = v;
            parent_e[e.to] = e_index;

            if(in_queue[e.to]) continue;
            q.append(e.to);
            in_queue[e.to] = true;
        }
    }

    return parent_v[sink] != -1;
}

double country::min_cost_max_flow(dynamic_array<dynamic_array<edge>>& graph, int64_t source, int64_t sink){
    std::cout << "Calling min_cost_max_flow(" << source << ", " << sink << ")" << std::endl;
    double total_cost = 0.0;

    if(source == sink){
        std::cerr << "ERROR: source == sink" << std::endl;
        return 0.0;
    }

    dynamic_array<double> dist;
    dynamic_array<int64_t> parent_v;
    dynamic_array<int64_t> parent_e;

    while(spfa(graph, source, sink, dist, parent_v, parent_e)){
        int64_t flow = INT64_MAX;

        for( int64_t v = sink; v != source; v = parent_v[v]){
            edge& e = graph[parent_v[v]][parent_e[v]];
            if(e.capacity < flow)flow = e.capacity;
        }
        for(int64_t v = sink; v != source; v = parent_v[v]){
            edge& e = graph[parent_v[v]][parent_e[v]];
            e.capacity -= flow;
            graph[e.to][e.rev].capacity += flow;
            total_cost += flow * e.cost;
        }
    }
    std::cout << "   TOTAL COST: " << total_cost << std::endl;
    return total_cost;
}

double country::min_cost_max_flow()
{
    dynamic_array<dynamic_array<edge>> graph;

    int64_t source = 0;
    int64_t sink = 0;
    std::cout << "Calling min_cost_max_flow()" << std::endl;

    return min_cost_max_flow(graph, source, sink);
}

bool country::assign_dwarfs_to_workplaces(const std::string& output_file){
    std::ofstream file(output_file);

    if(!file.is_open()) return false;

    for(int64_t material_type = 0; material_type < NamedValues::material::size; material_type++){
        for(int64_t i = 0; i <= workplaces[material_type].get_last_index(); i++) workplaces[material_type][i]->amount_of_dwarfs_working = 0;
    }

    for(int64_t material_type = 0; material_type < NamedValues::material::size; material_type++){
        int64_t houses_count = houses[material_type].get_last_index() + 1;

        int64_t workplaces_count = workplaces[material_type].get_last_index() + 1;

        file << "===== MATERIAL " << material_type << " =====\n";

        if(houses_count <= 0 || workplaces_count <= 0)continue;

        int64_t source = 0;
        int64_t house_offset = 1;

        int64_t workplace_offset = house_offset + houses_count;

        int64_t sink = workplace_offset + workplaces_count;

        int64_t vertices = sink + 1;

        dynamic_array<dynamic_array<edge>> graph;

        for(int64_t i = 0; i < vertices; i++) graph.append(dynamic_array<edge>());

        for(int64_t h = 0; h < houses_count; h++) add_edge(graph, source, house_offset + h, 1, 0.0 );

        for(int64_t h = 0; h < houses_count; h++){
            house* current_house = houses[material_type][h].get();

            for(int64_t w = 0; w < workplaces_count; w++){
                workplace* current_workplace = workplaces[material_type][w].get();

                double cost = distance(current_house->coordinates, current_workplace->coordinates);

                add_edge(graph, house_offset + h, workplace_offset + w, 1, cost);
            }
        }

        for(int64_t w = 0; w < workplaces_count; w++){
            add_edge(graph, workplace_offset + w, sink, workplaces[material_type][w]->capacity, 0.0);
        }

        min_cost_max_flow(graph, source, sink);

        bool found_assignment = false;

        for(int64_t h = 0; h < houses_count; h++){
            int64_t house_vertex = house_offset + h;

            for(int64_t e = 0;e <= graph[house_vertex].get_last_index();e++){
                edge& ed = graph[house_vertex][e];

                if(ed.to >= workplace_offset && ed.to < sink && ed.capacity == 0){
                    int64_t workplace_index = ed.to - workplace_offset;

                    workplaces[material_type][workplace_index]->amount_of_dwarfs_working++;

                    file << "House " << h << " -> Workplace " << workplace_index << '\n';
                    found_assignment = true;
                    break;
                }
            }
        }

        if(!found_assignment)
        {
            file << "(no assignments)\n";
        }

        file << '\n';
    }

    return true;
}

const dynamic_array<dynamic_array<std::unique_ptr<workplace>>>& country::get_workplaces() const{
    return workplaces;
}

const dynamic_array<dynamic_array<std::unique_ptr<house>>>& country::get_houses() const{
    return houses;
}

void country::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF A COUNTRY" << std::endl;

    tab(tabulation);
    std::cout << "WORKPLACES:" << std::endl;

    for( int material_type = 0; material_type < NamedValues::material::size; material_type++){
        tab(tabulation + NamedValues::constant::tabulator_indent);
        std::cout << "MATERIAL: " << material_type << std::endl;

        for(int64_t i = 0; i <= workplaces[material_type].get_last_index(); i++){
            tab(tabulation + NamedValues::constant::tabulator_indent * 2);
            std::cout << "INDEX: " << workplaces[material_type][i]->index << std::endl;

            tab(tabulation + NamedValues::constant::tabulator_indent * 2);
            std::cout << "CAPACITY: " << workplaces[material_type][i]->capacity << std::endl;

            tab(tabulation + NamedValues::constant::tabulator_indent * 2);
            std::cout << "DWARFS WORKING: " << workplaces[material_type][i]->amount_of_dwarfs_working << std::endl;

            tab(tabulation + NamedValues::constant::tabulator_indent * 2);
            std::cout << "COORDINATES: ";

            for(int a = NamedValues::axis::X; a <= NamedValues::axis::Z; a++) std::cout << workplaces[material_type][i]->coordinates[a] << " ";

            std::cout << std::endl;
        }
    }

    tab(tabulation);
    std::cout << "HOUSES:" << std::endl;

    for(int material_type = 0; material_type < NamedValues::material::size; material_type++)
    {
        tab(tabulation + 4);
        std::cout << "FAVORITE MATERIAL: " << material_type << std::endl;

        for(int64_t i = 0; i <= houses[material_type].get_last_index(); i++){
            tab(tabulation + 8);
            std::cout << "INDEX: " << houses[material_type][i]->index << std::endl;

            tab(tabulation + 8);
            std::cout << "COORDINATES: ";

            for(int a = NamedValues::axis::X; a <= NamedValues::axis::Z; a++) std::cout << houses[material_type][i]->coordinates[a] << " ";

            std::cout << std::endl;
        }
    }

    tab(tabulation);
    std::cout << "<<<" << std::endl;
}
