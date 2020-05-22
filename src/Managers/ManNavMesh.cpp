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
 
 
 #include "ManNavMesh.h"
#include "ManTotem.h"

#include <iostream>
#include "ManCar.h"
#include <Entities/NavMesh.h>
#include <Entities/Car.h>
#include <Components/CDimensions.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CNavMesh.h>
#include <Components/CId.h>
#include <include_json/include_json.hpp>
#include <cmath> 

using namespace std;
using json = nlohmann::json;

ManNavMesh::ManNavMesh() {
    //Leemos y añadimos los NavMesh
    double vertex1X=0, vertex1Y=0,vertex1Z=0;   // utilizamos double porque tiene mas precison que float (64b vs 32b)
    double vertex2X=0, vertex2Y=0,vertex2Z=0; 
    double vertex3X=0, vertex3Y=0,vertex3Z=0; 
    double vertex4X=0, vertex4Y=0,vertex4Z=0; 
    double vertex5X=0, vertex5Y=0,vertex5Z=0;   // utilizamos double porque tiene mas precison que float (64b vs 32b)
    double vertex6X=0, vertex6Y=0,vertex6Z=0; 
    double vertex7X=0, vertex7Y=0,vertex7Z=0; 
    double vertex8X=0, vertex8Y=0,vertex8Z=0;
    vector<int> waypointsId;


    ifstream i("data.json");
    json j = json::parse(i);

    long unsigned int navMeshCount = j["NAVMESHES"].size();

    //std::cout << "EL NUMERO DE NAVMESH EN EL JSON ES: " << navMeshCount << std::endl;
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
            vertex1Y   = vertex1["y"].get<double>();
            vertex1Z   = vertex1["z"].get<double>();
        auto vertex2    = navMeshActual["vertex2"];
            vertex2X   = vertex2["x"].get<double>();
            vertex2Y   = vertex2["y"].get<double>();
            vertex2Z   = vertex2["z"].get<double>();
        auto vertex3    = navMeshActual["vertex3"];
            vertex3X   = vertex3["x"].get<double>();
            vertex3Y   = vertex3["y"].get<double>();
            vertex3Z   = vertex3["z"].get<double>();
        auto vertex4    = navMeshActual["vertex4"];
            vertex4X   = vertex4["x"].get<double>();
            vertex4Y   = vertex4["y"].get<double>();
            vertex4Z   = vertex4["z"].get<double>();

        auto vertex5    = navMeshActual["vertex5"];
            vertex5X   = vertex5["x"].get<double>();
            vertex5Y   = vertex5["y"].get<double>();
            vertex5Z   = vertex5["z"].get<double>();
        auto vertex6    = navMeshActual["vertex6"];
            vertex6X   = vertex6["x"].get<double>();
            vertex6Y   = vertex6["y"].get<double>();
            vertex6Z   = vertex6["z"].get<double>();
        auto vertex7    = navMeshActual["vertex7"];
            vertex7X   = vertex7["x"].get<double>();
            vertex7Y   = vertex7["y"].get<double>();
            vertex7Z   = vertex7["z"].get<double>();
        auto vertex8    = navMeshActual["vertex8"];
            vertex8X   = vertex8["x"].get<double>();
            vertex8Y   = vertex8["y"].get<double>();
            vertex8Z   = vertex8["z"].get<double>();

        // una vez tenemos los 4 vertices, tenemos que sacar el centro y sus dimensiones (offset)
        double centroNavMeshX = (vertex1X+vertex2X+vertex3X+vertex4X + vertex5X+vertex6X+vertex7X+vertex8X)/8;
        double centroNavMeshY = (vertex1Y+vertex2Y+vertex3Y+vertex4Y + vertex5Y+vertex6Y+vertex7Y+vertex8Y)/8;
        double centroNavMeshZ = (vertex1Z+vertex2Z+vertex3Z+vertex4Z + vertex5Z+vertex6Z+vertex7Z+vertex8Z)/8;
        vec3 centroNavMesh = vec3(centroNavMeshX,centroNavMeshY+30,centroNavMeshZ); 
        double dimensionX = (abs(centroNavMeshX - vertex1X))*2;    
        double dimensionY = ((abs(centroNavMeshY - vertex1Y))*2)+60; 
        double dimensionZ = (abs(centroNavMeshZ - vertex1Z))*2; 
        // cogemos el array de wayPoints
        auto wayPointsIdCount = navMeshActual["waypoints"].size();
        //std::cout << "EL Navmesh " << idNavMesh << " tiene: " << wayPointsIdCount << " waypoints" << std::endl;
        for(long unsigned int k=0; k<wayPointsIdCount; k++){
            int wayPointIdActual = navMeshActual["waypoints"][k].get<int>();
            waypointsId.push_back(wayPointIdActual);
            //std::cout << "meteneos waypoint con ID: " << wayPointIdActual << std::endl;
        }
        // POSICION , ROTACION, OFFSETX, OFFSETY, OFFSETZ, VECTOR WAYPOINTS
        //std::cout <<" VOY A CREAR UN Navmesh LOCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
        CreateNavMesh(centroNavMesh,glm::vec3(0.0f,0.0f,0.0f),dimensionX,dimensionY,dimensionZ,waypointsId); 
    }
/*
        double centroNavMeshX = 0.0, centroNavMeshY= 0.0, centroNavMeshZ= 0.0;
        for(int j=1; j<9; j++){
            auto currentVertex = navMeshActual["vertex"+to_string(j)];
            centroNavMeshX += currentVertex["x"].get<double>();
            centroNavMeshY += currentVertex["y"].get<double>();
            centroNavMeshZ += currentVertex["z"].get<double>();
        }
        vec3 centroNavMesh = vec3(centroNavMeshX/8,centroNavMeshY/8,centroNavMeshZ/8); 
        cout << "el centro del NavMesh en X es: " << centroNavMesh.x << endl;

        auto vertexRef    = navMeshActual["vertex1"];
        double dimensionX = (abs(centroNavMeshX - vertexRef["x"].get<double>()))*2;    
        double dimensionY = (abs(centroNavMeshY - vertexRef["y"].get<double>()))*2; 
        double dimensionZ = (abs(centroNavMeshZ - vertexRef["z"].get<double>()))*2; 
  */  //nos suscribimos
    SubscribeToEvents();


//    // NAVMESH HERMANOS
//    bool next= false;
//    for( auto currentNavMesh : this->GetEntities()){
//        auto cNavMesh = static_cast<CNavMesh*>(currentNavMesh.get()->GetComponent(CompType::NavMeshComp).get());
//        for(long unsigned int i=0; i<this->GetEntities().size(); i++){
//            next = false;
//            auto cOtherNavMesh = static_cast<CNavMesh*>(this->GetEntities()[i].get()->GetComponent(CompType::NavMeshComp).get());
//            for(auto curretWayPoint : cNavMesh->waypoints){
//                for(auto otherWayPoint : cOtherNavMesh->waypoints){
//                    if(otherWayPoint == curretWayPoint && !next){
//                    // Son WayPoints Hermanos
//                        static_cast<NavMesh*>(currentNavMesh.get())->AddNavMeshBro(this->GetEntities()[i].get());
//                        //auto cId = static_cast<CId*>(currentNavMesh.get()->GetComponent(CompType::IdComp).get());
//                        //auto cId2 = static_cast<CId*>(this->GetEntities()[i].get()->GetComponent(CompType::IdComp).get());
//                        //cout << "En el Navmesh: " << cId->id << " hacemos hermano al navmesh: " << cId2->id << endl;
//                        next = true;
//                    }
//                }
//            }
//        }
//    }
//    // cuando hemos acabado de crear los NavMesh vamos a ver cque NavMesh son los que estan conectados con ellos
//
//    // DECIDIMOS EL NAVMESH CENTRAAAAAAAAAL
//    float maxArea = 0.0;
//    Entity* maxNavMesh = nullptr;
//    if(this->GetEntities().size() > 0) maxNavMesh = this->GetEntities()[0].get();
//    // SELECCIONAR NAVMHES CENTRAL
//    for( auto currentNavMesh : this->GetEntities()){
//        auto cDimNav = static_cast<CDimensions*>(currentNavMesh.get()->GetComponent(CompType::DimensionsComp).get());
//        if(maxArea < (cDimNav->width*cDimNav->depth)){
//            maxArea = cDimNav->width*cDimNav->depth;
//            maxNavMesh = currentNavMesh.get();
//        }
//    }
//    auto cMaxNavMesh = static_cast<CNavMesh*>(maxNavMesh->GetComponent(CompType::NavMeshComp).get());
//    cMaxNavMesh->centralNavMESH = true;
}

void ManNavMesh::Update(ManCar &manCar_) {

    // recorremos todas las entidades y actualizamos su NavMesh
    for(auto currentCar : manCar_.GetEntities()){
        UpdateNavMeshEntity(currentCar.get());
    }

}

void ManNavMesh::CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints){
    shared_ptr<NavMesh> p = make_shared<NavMesh>(pos,rot,width,height,depth,waypoints);
    entities.push_back(p);

    //auto cNavMesh = static_cast<CNavMesh*>(p.get()->GetComponent(CompType::NavMeshComp).get());
    //std::cout << "EL ID DEL NAVMESH EN EL QUE ESTAMOS ES: " << cNavMesh->id << std::endl;
}

void ManNavMesh::UpdateNavMeshEntity(Entity* entity_) const{
    auto cTransformableCar = static_cast<CTransformable*>(entity_->GetComponent(CompType::TransformableComp).get());     

    auto newNavMesh = CalculateNavMesh(*cTransformableCar);
    if(newNavMesh != -1){
        auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(entity_->GetComponent(CompType::CurrentNavMeshComp).get());
        cCurrentNavMesh->currentNavMesh = newNavMesh;
        //cout << "MI NAVMESH ES:::::::::::::::::::::::::::::::::::::::::::::::::::::::: " << newNavMesh << endl;
    }//else
        //cout << " NO PERTENECEMOS A NINGUN NAVMEEEESHHH " << endl;
}

int ManNavMesh::CalculateNavMesh(CTransformable &cTransformable) const{
    for(const auto& navmesh : GetEntities()){
        auto cDim = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTranNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformable.position.x >= (cTranNav->position.x-(cDim->width/2))) && 
            (cTransformable.position.x <= (cTranNav->position.x+(cDim->width/2))) ) &&
            ( (cTransformable.position.z >= (cTranNav->position.z-(cDim->depth/2))) && 
            (cTransformable.position.z <= (cTranNav->position.z+(cDim->depth/2))) ) &&
            ( (cTransformable.position.y >= (cTranNav->position.y-(cDim->height/2))) && 
            (cTransformable.position.y <= (cTranNav->position.y+(cDim->height/2))) ) ){
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                //std::cout << " El cochecito lereee pertenece al naveMesh: " << cNavMesh->id << std::endl;
                return cNavMesh->id;
            }       
    }
    return -1;
}

int ManNavMesh::CalculateNavMesh(glm::vec3 &position_) const{
    for(const auto& navmesh : GetEntities()){
        auto cDim = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTranNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (position_.x >= (cTranNav->position.x-(cDim->width/2))) && 
            (position_.x <= (cTranNav->position.x+(cDim->width/2))) ) &&
            ( (position_.z >= (cTranNav->position.z-(cDim->depth/2))) && 
            (position_.z <= (cTranNav->position.z+(cDim->depth/2))) ) &&
            ( (position_.y >= (cTranNav->position.y-(cDim->height/2))) && 
            (position_.y <= (cTranNav->position.y+(cDim->height/2))) ) ){
                auto cNavMesh = static_cast<CNavMesh*>(navmesh.get()->GetComponent(CompType::NavMeshComp).get());
                //std::cout << " El totem pertenece al navmesh " << cNavMesh->id << std::endl;
                return cNavMesh->id;
            }       
    }
    //cout << "no pertenecemos a ningun navmesh y la pos es (" << position_.x<< " , " << position_.y<< " , " << position_.z<< " )" << endl;
    
    return -1;
}


void ManNavMesh::SubscribeToEvents() {

    //EventManager::GetInstance().SubscribeMulti(Listener(
    //    EventType::ACTUALIZE_NAVMESH_TOTEM,
    //    bind(&ManNavMesh::ActualizeNavMeshTotem, this, placeholders::_1),
    //    "ActualizeNavMeshTotem"));
    //EventManager::GetInstance().SubscribeMulti(Listener(
    //    EventType::ACTUALIZE_NAVMESH_CARAI,
    //    bind(&ManNavMesh::ActualizeNavMeshCarAI, this, placeholders::_1),
    //    "ActualizeNavMeshCarAI"));
}


/*
//TODO: CARLOS controlar "violacion de segmento (core)" soltar/crear totem en navmeshe -1. Ha petado cuando le he dado a una IA pasando al lado de las paredes del centro
void ManNavMesh::ActualizeNavMeshTotem(DataMap* d){
    //std::cout << "Actualizamos NavMesh del Totem" << std::endl;
    bool todoCorrecto = false;
    auto totem = any_cast<Entity*>((*d)[TOTEM]);
    auto cTransformableTotem = static_cast<CTransformable*>(totem->GetComponent(CompType::TransformableComp).get());     
    for(const auto& navmesh : GetEntities()){
        auto cDimensions = static_cast<CDimensions*>(navmesh.get()->GetComponent(CompType::DimensionsComp).get());
        auto cTransformableNav = static_cast<CTransformable*>(navmesh.get()->GetComponent(CompType::TransformableComp).get()); 
        if( ( (cTransformableTotem->position.x >= (cTransformableNav->position.x-(cDimensions->width/2))) && 
            (cTransformableTotem->position.x <= (cTransformableNav->position.x+(cDimensions->width/2))) ) &&
            ( (cTransformableTotem->position.z >= (cTransformableNav->position.z-(cDimensions->depth/2))) && 
            (cTransformableTotem->position.z <= (cTransformableNav->position.z+(cDimensions->depth/2))) )  ){
                auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(totem->GetComponent(CompType::CurrentNavMeshComp).get());
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

    auto newNavMesh = CalculateNavMesh(*cTransformableTotem);
    if(newNavMesh != -1){
        auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
        cCurrentNavMesh->currentNavMesh = newNavMesh;
    }else
        cout << " Al iniciar el totem NO PERTENECEMOS A NINGUN NAVMEEEESHHH HDIIIIIOOOOSOSSOSOOSOSOS  ---> va a petar broooo" << endl;

}

void ManNavMesh::ActualizeNavMeshCarAI(DataMap* d){
    auto cTransformableCar = static_cast<CTransformable*>(any_cast<Entity*>((*d)[CAR_AI])->GetComponent(CompType::TransformableComp).get());     

    auto newNavMesh = CalculateNavMesh(*cTransformableCar);
    if(newNavMesh != -1){
        auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(any_cast<Entity*>((*d)[CAR_AI])->GetComponent(CompType::CurrentNavMeshComp).get());
        cCurrentNavMesh->currentNavMesh = newNavMesh;
    }else
        cout << " LA IA NO PERTENECEMOS A NINGUN NAVMEEEESHHH HDIIIIIOOOOSOSSOSOOSOSOS  ---> en ese caso nos quedamos con el que teniamos" << endl;
}
*/