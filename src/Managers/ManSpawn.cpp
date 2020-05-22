/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 #include "ManSpawn.h"

#include <iostream>
#include <include_json/include_json.hpp>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;
using json = nlohmann::json;
ManSpawn::ManSpawn() {

    double vertexX = 0, vertexY = 0, vertexZ = 0;

    std::ifstream i("data.json");
    std::stringstream buffer;
    buffer << i.rdbuf();
    string jsonString = buffer.str();
    json j = json::parse(jsonString);
    i.close();

    long unsigned int spawnsCout = j["KART SPAWNS"].size();
    //std::cout << "EL NUMERO DE SPAWNS de coches es: " << spawnsCout << std::endl;
    //Leemos el array de NavMesh
    for(long unsigned int i = 0; i< spawnsCout; ++i){
        string idSpawn = to_string(i);  //El primer elemento es "NAVMESH 1"
        auto currentSpawn = j["KART SPAWNS"][idSpawn];
        vertexX = currentSpawn["x"].get<double>();
        vertexY = currentSpawn["y"].get<double>();
        vertexZ = currentSpawn["z"].get<double>();
        posSpawn.push_back(glm::vec3(vertexX,vertexY,vertexZ));
    }

}

ManSpawn::~ManSpawn(){
    //cout << "Destruimos el manager de spawn" << endl; 
}