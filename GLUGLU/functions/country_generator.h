#ifndef __COUNTRY_GENERATOR_H__
#define __COUNTRY_GENERATOR_H__
#include "../../DataStructures/country.h"
#include <iostream>
#include <inttypes.h>
#include <fstream>
#include <ostream>
#include "../../src/Constants.h"

void generate_country(const std::string file_name, int64_t precision_point = 4, int64_t workplace_quantity_per_material = 10, int64_t house_quantity_per_material = 10, int64_t defult_capacity = 5, bool randomize_quantity = true, int64_t seed = 1){
    std::cout << "GENERATING COUNTRY" << std::endl;
    std::ofstream new_save_file(file_name);
    double x;
    double y;
    double z;
    double capacity;
    double precision = 1;
    for(int i = 0; i < precision_point; i++) precision *= 10;
    if(seed == 0) seed = 1;
    std::cout << "VALUES ARE READY" << std::endl;
    for(int64_t current_material = 0; current_material < NamedValues::material::size; current_material++){
        int64_t current_material_count;
        int64_t capacity = defult_capacity;
        if(randomize_quantity)current_material_count = rand()%(workplace_quantity_per_material+1);
        std::cout << "CURRENT MATERIAL: " << current_material << "   WORKPLACE QUANTITY: " << current_material_count << std::endl;
        for(int64_t index = 0; index<current_material_count; index++){
            x = rand()*seed%((int64_t)precision*+1)/precision;
            y = rand()*seed%((int64_t)precision+1)/precision;
            z = rand()*seed%((int64_t)precision+1)/precision;
            if(randomize_quantity) capacity = rand()*seed%(defult_capacity)+1;
            new_save_file << x << ";" << y << ";" << z << ";" << capacity << ";" << '\n';
        }
        new_save_file << ";;" << '\n';
    }
    new_save_file << '\n' << '\n';
    for(int64_t current_material = 0; current_material < NamedValues::material::size; current_material++){
        int64_t current_material_count;
        int64_t capacity = defult_capacity;
        if(randomize_quantity)current_material_count = rand()%(workplace_quantity_per_material+1);
        std::cout << "CURRENT MATERIAL: " << current_material << "   WORKPLACE QUANTITY: " << current_material_count << std::endl;
        for(int64_t index = 0; index<current_material_count; index++){
            x = rand()*seed%((int64_t)precision+1)/precision;
            y = rand()*seed%((int64_t)precision+1)/precision;
            z = rand()*seed%((int64_t)precision+1)/precision;
            new_save_file << x << ";" << y << ";" << z << ";" << '\n';
        }
        new_save_file << ";;" << '\n';
    }
    new_save_file.close();
}


#endif//__COUNTRY_GENERATOR_H__
