#include "ManNavMesh.h"
#include "ManTotem.h"

#include <iostream>
#include <Entities/NavMesh.h>
#include <Components/CDimensions.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CNavMesh.h>
#include <include_json/include_json.hpp>
#include <cmath> 

using namespace std;
using json = nlohmann::json;

ManNavMesh::ManNavMesh(Entity *carPlayer, ManTotem *manTotems) {
    //Leemos y añadimos los NavMesh
    double vertex1X=0, /*vertex1Y=0,*/vertex1Z=0;   // utilizamos double porque tiene mas precison que float (64b vs 32b)
    double vertex2X=0, /*vertex2Y=0,*/vertex2Z=0; 
    double vertex3X=0, /*vertex3Y=0,*/vertex3Z=0; 
    double vertex4X=0, /*vertex4Y=0,*/vertex4Z=0; 
    double vertex5X=0, /*vertex5Y=0,*/vertex5Z=0;   // utilizamos double porque tiene mas precison que float (64b vs 32b)
    double vertex6X=0, /*vertex6Y=0,*/vertex6Z=0; 
    double vertex7X=0, /*vertex7Y=0,*/vertex7Z=0; 
    double vertex8X=0, /*vertex8Y=0,*/vertex8Z=0;
    vector<int> waypointsId;


    ifstream i("data.json");
    json j = json::parse(i);

    long unsigned int navMeshCount = j["NAVMESHES"].size();

    std::cout << "EL NUMERO DE NAVMESH EN EL JSON ES: " << navMeshCount << std::endl;
    //Leemos el array de NavMesh
    for(long unsigned int i = 0; i< navMeshCount; ++i){

        while(!waypointsId.empty()){
            // hasta que no vaciemos el array
            waypointsId.pop_back();
        }
        string idNavMesh = to_string(i);  //El primer elemento es "NAVMESH 1"
        auto navMeshActual = j["NAVMESHES"][idNavMesh];
        // cogemos los 4 vertices
        auto vertex1    = navMeshActual["vertex1"];
            vertex1X   = vertex1["x"].get<double>();
            //vertex1Y   = vertex1["y"].get<double>();
            vertex1Z   = vertex1["z"].get<double>();
        auto vertex2    = navMeshActual["vertex2"];
            vertex2X   = vertex2["x"].get<double>();
            //vertex2Y   = vertex2["y"].get<double>();
            vertex2Z   = vertex2["z"].get<double>();
        auto vertex3    = navMeshActual["vertex3"];
            vertex3X   = vertex3["x"].get<double>();
            //vertex3Y   = vertex3["y"].get<double>();
            vertex3Z   = vertex3["z"].get<double>();
        auto vertex4    = navMeshActual["vertex4"];
            vertex4X   = vertex4["x"].get<double>();
            //vertex4Y   = vertex4["y"].get<double>();
            vertex4Z   = vertex4["z"].get<double>();

        auto vertex5    = navMeshActual["vertex5"];
            vertex5X   = vertex5["x"].get<double>();
            //vertex5Y   = vertex5["y"].get<double>();
            vertex5Z   = vertex5["z"].get<double>();
        auto vertex6    = navMeshActual["vertex6"];
            vertex6X   = vertex6["x"].get<double>();
            //vertex6Y   = vertex6["y"].get<double>();
            vertex6Z   = vertex6["z"].get<double>();
        auto vertex7    = navMeshActual["vertex7"];
            vertex7X   = vertex7["x"].get<double>();
            //vertex7Y   = vertex7["y"].get<double>();
            vertex7Z   = vertex7["z"].get<double>();
        auto vertex8    = navMeshActual["vertex8"];
            vertex8X   = vertex8["x"].get<double>();
            //vertex8Y   = vertex8["y"].get<double>();
            vertex8Z   = vertex8["z"].get<double>();

        // una vez tenemos los 4 vertices, tenemos que sacar el centro y sus dimensiones (offset)
        double centroNavMeshX = (vertex1X+vertex2X+vertex3X+vertex4X + vertex5X+vertex6X+vertex7X+vertex8X)/8;
        // Actualmente como no hay altura no la utilizamos.. por defecto en la 20
        double centroNavMeshY = 20.0f;
        //double centroNavMeshY = (vertex1Y+vertex2Y+vertex3Y+vertex4Y + vertex5Y+vertex6Y+vertex7Y+vertex8Y)/8;
        double centroNavMeshZ = (vertex1Z+vertex2Z+vertex3Z+vertex4Z + vertex5Z+vertex6Z+vertex7Z+vertex8Z)/8;
        vec3 centroNavMesh = vec3(centroNavMeshX,centroNavMeshY,centroNavMeshZ); 
        double dimensionX = (abs(centroNavMeshX - vertex1X))*2;    
        //double dimensionY = (abs(centroNavMeshY - vertex1Y))*2; 
        double dimensionZ = (abs(centroNavMeshZ - vertex1Z))*2; 
        // cogemos el array de wayPoints
        auto wayPointsIdCount = navMeshActual["waypoints"].size();
        std::cout << "EL Navmesh " << idNavMesh << " tiene: " << wayPointsIdCount << " waypoints" << std::endl;
        for(long unsigned int k=0; k<wayPointsIdCount; k++){
            int wayPointIdActual = navMeshActual["waypoints"][k].get<int>();
            waypointsId.push_back(wayPointIdActual);
            std::cout << "meteneos waypoint con ID: " << wayPointIdActual << std::endl;
        }
        // POSICION , ROTACION, OFFSETX, OFFSETY, OFFSETZ, VECTOR WAYPOINTS
        //std::cout <<" VOY A CREAR UN Navmesh LOCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
        CreateNavMesh(centroNavMesh,glm::vec3(0.0f,0.0f,0.0f),dimensionX,10,dimensionZ,waypointsId); 
    }
    // CREAMOS DE PRUEBA UN NAVMESH
    //vector<int> waypoints1{7,8,9,10,11,13};
    //vector<int> waypoints0{0,1,2,3,4,12};
    //vector<int> waypoints2{3,5,7};  // el 5 debe ser referencia
    //vector<int> waypoints3{4,6,8};  // el 6 debe ser referencia
    //CreateNavMesh(glm::vec3(0.0f,0.0f,-200.0f),glm::vec3(0.0f,0.0f,0.0f),1000,32,500,waypoints0);  //0
    //CreateNavMesh(glm::vec3(0.0f,0.0f,500.0f),glm::vec3(0.0f,0.0f,0.0f),1000,32,500,waypoints1);   //1
    //CreateNavMesh(glm::vec3(-300.0f,0.0f,150.0f),glm::vec3(0.0f,0.0f,0.0f),150,32,200,waypoints2); //2
    //CreateNavMesh(glm::vec3(300.0f,0.0f,150.0f),glm::vec3(0.0f,0.0f,0.0f),150,32,200,waypoints3);  //3

    //nos suscribimos
    SubscribeToEvents();

    // Para no duplicar codigo, pasamos el coche de tipo CarHuman a Entity
    UpdateNavMeshPlayer(carPlayer);
    InitNavMeshTotem(manTotems);
}

void ManNavMesh::CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints){
    shared_ptr<NavMesh> p = make_shared<NavMesh>(pos,rot,width,height,depth,waypoints);
    entities.push_back(p);

    auto cNavMesh = static_cast<CNavMesh*>(p.get()->GetComponent(CompType::NavMeshComp).get());
    std::cout << "EL ID DEL NAVMESH EN EL QUE ESTAMOS ES: " << cNavMesh->id << std::endl;
}


void ManNavMesh::SubscribeToEvents() {

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::ACTUALIZE_NAVMESH_TOTEM,
        bind(&ManNavMesh::ActualizeNavMeshTotem, this, placeholders::_1),
        "ActualizeNavMeshTotem"));
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::ACTUALIZE_NAVMESH_CARAI,
        bind(&ManNavMesh::ActualizeNavMeshCarAI, this, placeholders::_1),
        "ActualizeNavMeshCarAI"));
}

//TODO: CARLOS controlar "violacion de segmento (core)" soltar/crear totem en navmeshe -1. Ha petado cuando le he dado a una IA pasando al lado de las paredes del centro
void ManNavMesh::ActualizeNavMeshTotem(DataMap* d){
    //std::cout << "Actualizamos NavMesh del Totem" << std::endl;
    bool todoCorrecto = false;
    auto cTransformableTotem = static_cast<CTransformable*>(any_cast<Entity*>((*d)[TOTEM])->GetComponent(CompType::TransformableComp).get());     
    for(const auto& navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableTotem->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableTotem->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableTotem->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableTotem->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(any_cast<Entity*>((*d)[TOTEM])->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El totem pertenece al naveMesh: " << cNavMesh->id << std::endl;
                todoCorrecto = true;
            }       
    }
    if(!todoCorrecto){
        // hay que comprobar tambien si donde lo queremos crear esta dentro de un objeto (colisionando con un objeto)
        std::cout << "ALGOOOOOOOOOOOOO VAAAAAAAAAAAAAAAAAAAAAAAA MAAAAAAAAAAAAAAAAAAAAAAAAAAAAL" << std::endl;
        std::cout << "El totem no esta en ningun navMesh, y como por defecto se pone el -1... PETA" << std::endl;

    }
}

void ManNavMesh::InitNavMeshTotem(ManTotem *manTotems){
    auto cTransformableTotem = static_cast<CTransformable*>(manTotems->GetEntities()[0].get()->GetComponent(CompType::TransformableComp).get());     
    for(const auto& navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableTotem->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableTotem->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableTotem->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableTotem->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << "222222222222222222222222222 El totem pertenece al naveMesh: " << cNavMesh->id << std::endl;
            }       
    }
}

void ManNavMesh::ActualizeNavMeshCarAI(DataMap* d){
    auto cTransformableCar = static_cast<CTransformable*>(any_cast<Entity*>((*d)[CAR_AI])->GetComponent(CompType::TransformableComp).get());     
    for(const auto& navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(any_cast<Entity*>((*d)[CAR_AI])->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
            }       
    }
}

void ManNavMesh::UpdateNavMeshPlayer(Entity* carPlayer){
    auto cTransformableCar = static_cast<CTransformable*>(carPlayer->GetComponent(CompType::TransformableComp).get());     
    for(const auto& navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableCar->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableCar->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableCar->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableCar->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carPlayer->GetComponent(CompType::CurrentNavMeshComp).get());
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                cCurrentNavMesh->currentNavMesh = cNavMesh->id;
                //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
            }       
    }
}
