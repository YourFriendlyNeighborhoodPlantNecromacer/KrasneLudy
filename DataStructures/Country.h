#ifndef __COUNTRY_H__
#define __COUNTRY_H__

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
    void insert(location* new_location);
    int64_t get_last_index();
    bool edge_exists(int64_t from, int64_t to);
    bool make_connection(int64_t from, int64_t to);
    int64_t find_path(int64_t from, int64_t to);
    int64_t bfs(int64_t from, int64_t to);
    int64_t dfs(int64_t from, int64_t to);
    bool construct_from_file(const std::string &file_name);

    void dp(int64_t tabulation = 0);

    private:
    int64_t rec_dfs(int64_t current_node, int64_t to, int64_t depth, bool* visited);

};

country::country() : locations(){}

country::country(int64_t size) : locations(size){}

country::country(const country& other) : locations(other.locations){}

country::~country(){
    std::cout << "See ya next time, space cowboy" << std::endl;
}


country& country::operator = (const country& other){
    if(this==&other) return *this;
    this->locations = other.locations;
    return *this;
}

bool country::is_empty(){
    return locations.is_empty();
}

void country::insert(location* new_location){
    locations.insert(new_location, locations.get_last_index());
}

int64_t country::get_last_index(){
    return locations.get_last_index();
}

bool country::edge_exists(int64_t from, int64_t to){
    return true; //custom .is_in operation needed. To hell with it making a class for this
}

bool country::make_connection(int64_t from, int64_t to){
    if(from < 0 || to < 0 || from > locations.get_last_index() || from > locations.get_last_index()) return false;
    //locations[from].connect_to(to);
    return true;
}

int64_t country::find_path(int64_t from, int64_t to){
    return -1;
}

int64_t country::bfs(int64_t from, int64_t to){
    return -1;
}


int64_t country::dfs(int64_t from, int64_t to){
    return -1;
}


bool country::construct_from_file(const std::string &file_name){
    return false;
}


void country::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF FARLAND" << std::endl;
    tab(tabulation);
    std::cout << "SIZE: " << locations.get_size() << std::endl;
    tab(tabulation);
    std::cout << "LAST INDEX: " << locations.get_last_index() << std::endl;
    nl();
    tab(tabulation);
    std::cout << "CHILD LISTS: " << std::endl;
    tab(tabulation);
    for(int i=0; i<locations.get_last_index(); i++){
        tab(tabulation + 3);
        std::cout << "i:" << i << "   locations[i]:" << locations[i] << std::endl;
    }
    tab(tabulation);
    std::cout << "<<<" << std::endl;
}

#endif//__COUNTRY_H__
