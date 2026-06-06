#ifndef __COUNTRY_GENERATOR_H__
#define __COUNTRY_GENERATOR_H__
#include "../../DataStructures/country.h"
#include <iostream>
#include <inttypes.h>
#include <fstream>
#include <ostream>
#include "../../src/Constants.h"



void generateCountry(const std::string file_name, int64_t precisionPoint = 4, int64_t workplaceQuantityPerMaterial = 10, int64_t houseQuantityPerMaterial = 10, int64_t defultCapacity = 5, bool randomizeQuantity = true, int64_t seed = 1){
    std::cout << "GENERATING COUNTRY" << std::endl;
    std::ofstream newSaveFile(file_name);
    double x;
    double y;
    double z;
    double currentCapacity = defultCapacity;
    double precision = 1;
    for(int i = 0; i < precisionPoint; i++) precision *= 10;
    if(seed == 0) seed = 1;
    std::cout << "VALUES ARE READY" << std::endl;

    int64_t currentStructureQuantity = workplaceQuantityPerMaterial;
    for(int64_t currentMaterial = 0; currentMaterial < namedValues::material::size; currentMaterial++){

        if(randomizeQuantity)currentStructureQuantity = rand()%(workplaceQuantityPerMaterial+1);
        std::cout << "CURRENT MATERIAL: " << currentMaterial << "   WORKPLACE QUANTITY: " << currentStructureQuantity << std::endl;

        for(int64_t index = 0; index<currentStructureQuantity; index++){
            x = rand()*seed%((int64_t)precision*+1)/precision;
            y = rand()*seed%((int64_t)precision+1)/precision;
            z = rand()*seed%((int64_t)precision+1)/precision;
            if(randomizeQuantity) currentCapacity = rand()*seed%(defultCapacity)+1;
            newSaveFile << x << ";" << y << ";" << z << ";" << currentCapacity << ";" << '\n';
        }

        newSaveFile << ";;" << '\n';
    }
    newSaveFile << '\n' << '\n';


    currentStructureQuantity = houseQuantityPerMaterial;
    for(int64_t currentMaterial = 0; currentMaterial < namedValues::material::size; currentMaterial++){

        if(randomizeQuantity)currentStructureQuantity = rand()%(houseQuantityPerMaterial+1);
        std::cout << "CURRENT MATERIAL: " << currentMaterial << "   HOUSE QUANTITY: " << currentStructureQuantity << std::endl;

        for(int64_t index = 0; index<currentStructureQuantity; index++){
            x = rand()*seed%((int64_t)precision+1)/precision;
            y = rand()*seed%((int64_t)precision+1)/precision;
            z = rand()*seed%((int64_t)precision+1)/precision;
            newSaveFile << x << ";" << y << ";" << z << ";" << '\n';
        }

        newSaveFile << ";;" << '\n';
    }

    newSaveFile.close();
}

void generateCountry(const std::string file_name, int64_t precisionPoint = 4, int64_t workplaceQuantityPerMaterial = 10, int64_t houseQuantityPerMaterial = 10, int64_t defultCapacity = 5, bool randomizeQuantity = true, const std::string seed = " "){
    int64_t h = 9001;
    for(char c : seed) h = (h<<5)+h + c;
    generateCountry(file_name, precisionPoint = 4, workplaceQuantityPerMaterial = 10, houseQuantityPerMaterial = 10, defultCapacity = 5, randomizeQuantity = true, h);
}

#endif//__COUNTRY_GENERATOR_H__
