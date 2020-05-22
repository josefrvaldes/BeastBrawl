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
 
 
 #include "RenderFacadeClover.h"


#include <cmath>
#include <algorithm>    // std::sort

#include <Components/CBoundingChassis.h>
#include <Components/CBoundingOBB.h>
#include <Components/CPowerUp.h>
#include <Components/CBoundingPlane.h>
#include <Components/CBoundingSphere.h>
#include <Components/CCamera.h>
#include <Components/CParentNode.h>
#include <Components/CDimensions.h>
#include <Components/CId.h>
#include <Components/CWheel.h>
#include <Components/CMesh.h>
#include <Components/CParticleSystem.h>
#include <Components/CNamePlate.h>
#include <Components/CTexture.h>
#include <Components/CTotem.h>
#include <Components/CType.h>
#include <Components/CShader.h>
#include <Components/CWayPointEdges.h>
#include <Components/CLight.h>
#include <Components/CBrainAI.h>
#include <Components/CNavMesh.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CCar.h>
#include <Components/CAnimation.h>
#include <Components/CClock.h>
// #include <Components/CBrain.h>

#include <Entities/CarAI.h>
#include <Entities/CarHuman.h>

#include <Systems/Physics.h>
#include <Systems/Utils.h>

#include <Managers/ManNavMesh.h>

#include <Constants.h>

#include <Game.h>

#include <Facade/Input/InputFacadeManager.h>




RenderFacadeClover::~RenderFacadeClover() {
}

/**
 * Inicializamos la fachada de clover e inicializamos el motor creando la ventana y pasandosela a las otras fachadas
 */
RenderFacadeClover::RenderFacadeClover() {
    //cout << "************************************\n";
    //cout << "(づ｡◕‿‿◕｡)づ   \n";
    //cout << "Se inicia el motor grafico de Clover\n";
    //cout << "(づ｡◕‿‿◕｡)づ   \n";
    //cout << "************************************\n";

    string title = "BeastBrawl";
    device = new CLEngine(1280, 720, title);
    
    // auto input = InputFacadeManager::GetInstance()->GetInputFacade();
    // auto inputClover = static_cast<InputFacadeClover*>(input);
    // inputClover->SetWindowContext(device);

    // auto physics = PhysicsFacadeManager::GetInstance()->GetPhysicsFacade();
    // auto physicsClover = static_cast<PhysicsFacadeClover*>(physics);
    // physicsClover->SetWindowContext(device);

    //Inicializamos el arbol de la escena
    smgr = device->GetSceneManager();
    resourceManager = device->GetResourceManager();
    FacadeInitIntro();


}

/**
 * Suscripcion de eventos
 */
void RenderFacadeClover::FacadeSuscribeEvents() {
    EventManager::GetInstance().Subscribe(Listener{
        EventType::UPDATE_POWERUP_HUD,
        bind( &RenderFacadeClover::FacadeUpdatePowerUpHUD, this, placeholders::_1 ),
        "facadeUpdatePowerUpHUD"});

    EventManager::GetInstance().Subscribe(Listener{
        EventType::INIT_PARTICLES_BOX,
        bind( &RenderFacadeClover::FacadeInitParticleSystem, this, placeholders::_1 ),
        "FacadeInitParticleSystem"});

    EventManager::GetInstance().Subscribe(Listener{
        EventType::UPDATE_FACADE_VISIBILITY,
        bind( &RenderFacadeClover::FacadeUpdateVisibility, this, placeholders::_1 ),
        "FacadeUpdateVisibility"});
}

/**
 * Suscripcion de eventos del settings 
 */
void RenderFacadeClover::FacadeSuscribeEventsSettings() {

    EventManager::GetInstance().Subscribe(Listener{
        EventType::SET_RESOLUTION,
        bind( &RenderFacadeClover::FacadeSetWindowSize, this, placeholders::_1 ),
        "FacadeSetWindowSize"});

    EventManager::GetInstance().Subscribe(Listener{
        EventType::ENABLE_PARTICLES,
        bind( &RenderFacadeClover::FacadeSetParticlesVisibility, this, placeholders::_1 ),
        "FacadeSetParticlesVisibility"});

    EventManager::GetInstance().Subscribe(Listener{
        EventType::ENABLE_VEGETATION,
        bind( &RenderFacadeClover::FacadeSetGrassActivate, this, placeholders::_1 ),
        "FacadeSetGrassActivate"});
    
    EventManager::GetInstance().Subscribe(Listener{
        EventType::ENABLE_SHADOWS,
        bind( &RenderFacadeClover::FacadeSetShadowsActivate, this, placeholders::_1 ),
        "FacadeSetShadowsActivate"});
}



/**
 * Actualiza en el hud el icono de powerup
 * @param {CTypePowerUp}
 */
void RenderFacadeClover::FacadeUpdatePowerUpHUD(DataMap* d) {
    auto type = any_cast<typeCPowerUp>((*d)[TYPE_POWER_UP]);
    currentPowerUp = int(type);

    //Si es la primera vez que se coge un powerup se crea la animacion
    if(!powerUpAnimation){
        powerUpAnimation = make_unique<Animation2D>("media/animacionPowerUp/animacionPU.png",55,60);
        powerUpAnimation->Start();
    }else if(powerUpAnimation && type != typeCPowerUp::None){
        powerUpAnimation->Restart();
        powerUpAnimation->Start();

    }
}

void RenderFacadeClover::FacadeInitParticleSystem(DataMap* d) const{
    auto idEntity = any_cast<uint16_t>((*d)[ID]);

    auto node = device->GetNodeByID(idEntity);
    auto clParticleSystem = static_cast<CLParticleSystem*>(node->GetEntity());

    if(!clParticleSystem->GetStarted()){
        clParticleSystem->Start();

    }else{
        clParticleSystem->StartOneIteration();
    }
}

void RenderFacadeClover::FacadeUpdateVisibility(DataMap* d){
    //Seguramente venga un ID y un true o false
    auto idEntity = any_cast<uint16_t>((*d)[ID]);
    auto visibility = any_cast<bool>((*d)[TRUEFALSE]);

    auto node = device->GetNodeByID(idEntity);
    node->SetVisible(visibility);
}


void RenderFacadeClover::FacadeSetParticlesVisibility(DataMap* d) const{
    auto mode = any_cast<int>((*d)[TRUEFALSE]);

    device->SetParticlesVisibility(mode);
}

void RenderFacadeClover::FacadeSetGrassActivate(DataMap* d) const{
    auto mode = any_cast<int>((*d)[TRUEFALSE]);

    device->SetGrassActivate(mode);
}


void RenderFacadeClover::FacadeSetShadowsActivate(DataMap* d) const{
    auto mode = any_cast<int>((*d)[TRUEFALSE]);

    device->SetShadowsActivate(mode);
}

/**
 * Se llama una vez para añadir las NamePlates
 * @param {manager de nameplates}
 */
void RenderFacadeClover::FacadeAddPlates(Manager* manNamePlates) {
    for(const auto& nameplate : manNamePlates->GetEntities()){
        auto cId = static_cast<CId*>(nameplate->GetComponent(CompType::IdComp).get());
        auto nameplateComp = static_cast<CNamePlate*>(nameplate->GetComponent(CompType::NamePlateComp).get());
        auto father = device->GetNodeByID(nameplateComp->idCarAsociated);
        auto node = device->AddBillBoard(father,cId->id,nameplateComp->billboardPath,false,80,30);
        node->SetTranslation(glm::vec3(0.0f,28.0f,0.0f));
    }
}

/**
 * Actualiza las posiciones de las nameplates
 */
void RenderFacadeClover::FacadeUpdatePlates(Manager* manNamePlates, Manager* manCars) {
    bool haveTotem;
    for(const auto& nameplate : manNamePlates->GetEntities()){
        auto cId = static_cast<CId*>(nameplate->GetComponent(CompType::IdComp).get());
        auto nameplateComp = static_cast<CNamePlate*>(nameplate->GetComponent(CompType::NamePlateComp).get());
        haveTotem = false;
        for(const auto& car : manCars->GetEntities()){
            auto cIdCar = static_cast<CId*>(car->GetComponent(CompType::IdComp).get());
            if(cIdCar->id == nameplateComp->idCarAsociated){
                auto cTotem = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
                if(cTotem->active) haveTotem = true;
            }
        }

        if(!haveTotem)
            device->GetNodeByID(cId->id)->SetTranslation(glm::vec3(0.0f,28.0f,0.0f));
        else
            device->GetNodeByID(cId->id)->SetTranslation(glm::vec3(0.0f,58.0f,0.0f));
    }
}

/**
 * Metodo para añadir muchas entidades de golpe
 */
const void RenderFacadeClover::FacadeAddObjects(vector<Entity*> entities) {
    for (Entity* e : entities) {
        FacadeAddObject(e);
    }
}

/**
 * Añadimos el objeto al arbol de la escena
 * @return {id del objeto que pasas}
 */
const uint16_t RenderFacadeClover::FacadeAddObject(Entity* entity) {
    return FacadeAddObject(entity, false);
}

const uint16_t RenderFacadeClover::FacadeAddObject(Entity* entity, bool mainCar) {

    //Comprobamos si el objeto debe añadirse a la raiz o a algun nodo padre
    CLNode* father = smgr;
    if(entity->HasComponent(CompType::ParentNodeComp)){
        auto cParentNode = static_cast<CParentNode*>(entity->GetComponent(CompType::ParentNodeComp).get());
        father = device->GetNodeByID(cParentNode->idParentNode);
    } 

    auto cTransformable = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    auto cType = static_cast<CType*>(entity->GetComponent(CompType::TypeComp).get());
    auto cShader = static_cast<CShader*>(entity->GetComponent(CompType::ShaderComp).get());

    CLResourceMesh* mesh = nullptr;
    // CLResourceMaterial* mat = nullptr;
    if(entity->HasComponent(CompType::MeshComp)){
        auto cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());
        std::string currentMesh = cMesh->activeMesh;
        std::string meshPath = "media/" + currentMesh;
        mesh = resourceManager->GetResourceMesh(meshPath, false);
        // mat = resourceManager->GetResourceMaterial(meshPath);
    } 

    
    CLNode* node = nullptr;
    // añadimos el node al sceneManager dependiendo del tipo de node que sea
    switch (cType->type) {


        case ModelType::Sphere:

            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            //static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);

            break;

        case ModelType::Cube:
            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            break;

        case ModelType::StaticMesh: {
            node = device->AddMesh(father,cId->id);

            // TODO: esto debería recorrer un vector de animaciones en vez de meterlas una a una. 2.0
            // WIN
            auto cAnimation = static_cast<CAnimation*>(entity->GetComponent(CompType::AnimationComp).get());
            std::string path = cAnimation->GetAnimWin()->path;
            std::string animationPath = "media/" + path;
            vector<CLResourceMesh*> clAnimationsWin = resourceManager->GetResourceAnimation(animationPath, cAnimation->GetAnimWin()->numKeyFrames, false);

            shared_ptr<CLAnimation> animWin;
            if(cAnimation->GetAnimWin()->IsInterpolated())
                animWin = make_shared<CLAnimation>(path, clAnimationsWin, cAnimation->GetAnimWin()->loop, cAnimation->GetAnimWin()->GetDistances());
            else
                animWin = make_shared<CLAnimation>(path, clAnimationsWin, cAnimation->GetAnimWin()->loop);
            static_cast<CLMesh*>(node->GetEntity())->AddAnimation(cAnimation->GetAnimWin()->path, animWin);

            if (mainCar) {
                // LEFT
                path = cAnimation->GetAnimLeft()->path;
                animationPath = "media/" + path;
                vector<CLResourceMesh*> clAnimationsLeft = resourceManager->GetResourceAnimation(animationPath, cAnimation->GetAnimLeft()->numKeyFrames, false);

                shared_ptr<CLAnimation> animLeft;
                if(cAnimation->GetAnimLeft()->IsInterpolated())
                    animLeft = make_shared<CLAnimation>(path, clAnimationsLeft, cAnimation->GetAnimLeft()->loop, cAnimation->GetAnimLeft()->GetDistances());
                else
                    animLeft = make_shared<CLAnimation>(path, clAnimationsLeft, cAnimation->GetAnimLeft()->loop);
                static_cast<CLMesh*>(node->GetEntity())->AddAnimation(cAnimation->GetAnimLeft()->path, animLeft);

                // RIGHT
                path = cAnimation->GetAnimRight()->path;
                animationPath = "media/" + path;
                vector<CLResourceMesh*> clAnimationsRight = resourceManager->GetResourceAnimation(animationPath, cAnimation->GetAnimRight()->numKeyFrames, false);

                shared_ptr<CLAnimation> animRight;
                if(cAnimation->GetAnimRight()->IsInterpolated())
                    animRight = make_shared<CLAnimation>(path, clAnimationsRight, cAnimation->GetAnimRight()->loop, cAnimation->GetAnimRight()->GetDistances());
                else
                    animRight = make_shared<CLAnimation>(path, clAnimationsRight, cAnimation->GetAnimRight()->loop);
                static_cast<CLMesh*>(node->GetEntity())->AddAnimation(cAnimation->GetAnimRight()->path, animRight);
            }

            // IDLE
            path = cAnimation->GetAnimIdle()->path;
            animationPath = "media/" + path;
            vector<CLResourceMesh*> clAnimationsIdle = resourceManager->GetResourceAnimation(animationPath, cAnimation->GetAnimIdle()->numKeyFrames, false);

            shared_ptr<CLAnimation> animIdle;
            if(cAnimation->GetAnimIdle()->IsInterpolated())
                animIdle = make_shared<CLAnimation>(path, clAnimationsIdle, cAnimation->GetAnimIdle()->loop, cAnimation->GetAnimIdle()->GetDistances());
            else
                animIdle = make_shared<CLAnimation>(path, clAnimationsIdle, cAnimation->GetAnimIdle()->loop);
            static_cast<CLMesh*>(node->GetEntity())->AddAnimation(cAnimation->GetAnimIdle()->path, animIdle);

            // activamos por defecto la IDLE
            static_cast<CLMesh*>(node->GetEntity())->ActivateAnimation(cAnimation->GetAnimIdle()->path);
        }   break;

        case ModelType::AnimatedMesh:
            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);

            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            //static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);

            break;

        case ModelType::Shield:
            father = smgr;
            node = device->AddMesh(father,cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            //static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);
            node->SetVisible(false);
            break;

        case ModelType::Text:
            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);

            break;

        case ModelType::PointLight:{
            auto cLight = static_cast<CLight*>(entity->GetComponent(CompType::LightComp).get());
            //node = father->AddPointLight(cId->id,cLight->intensity,cLight->ambient,cLight->diffuse,cLight->specular,cLight->constant,cLight->linear,cLight->quadratic);
            node = device->AddPointLight(father,cId->id,cLight->intensity,cLight->ambient,cLight->diffuse,cLight->specular,cLight->constant,cLight->linear,cLight->quadratic);
            
            break;
        }
            
        
        case ModelType::ParticleSystem:{
            auto cParticleSystem = static_cast<CParticleSystem*>(entity->GetComponent(CompType::ParticleSystemComp).get());
 
            if(cParticleSystem->radious != 0){  
                //Es circulo o esfera
                if(cParticleSystem->orientation == glm::vec3(0.0f,0.0f,0.0f)){
                    //Es esfera
                    //node = father->AddParticleSystem(cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->flags);
                    node = device->AddParticleSystem(father,cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->flags);
                }else{
                    //Es circulo
                    //node = father->AddParticleSystem(cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->orientation,cParticleSystem->flags);
                    node = device->AddParticleSystem(father,cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->orientation,cParticleSystem->flags);
                }
            }else{
                //Es punto, linea, cuadrado o cubo
                node = device->AddParticleSystem(father,cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->offset,cParticleSystem->orientation,cParticleSystem->flags);
            }
            static_cast<CLParticleSystem*>(node->GetEntity())->SetLoop(cParticleSystem->loop);

            if(cParticleSystem->started){
                static_cast<CLParticleSystem*>(node->GetEntity())->Start();
            }

            
            node->SetTranslation(glm::vec3(cTransformable->position.x,cTransformable->position.y,-cTransformable->position.z));
            node->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));
            node->SetScalation(cTransformable->scale);

            return cId->id;
            break;
        }
            
    } 


    
    auto shader = resourceManager->GetResourceShader(cShader->vertexShader,cShader->fragmentShader);
    node->SetShaderProgramID(shader->GetProgramID());

    //Seteamos valores
    node->SetTranslation(glm::vec3(cTransformable->position.x,cTransformable->position.y,-cTransformable->position.z));
    node->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));
    node->SetScalation(cTransformable->scale);

    //auto pos = cTransformable->position;
    //cout << " la posicion donde esta la entidad es ( " << pos.x<< " , " << pos.y<< " , " <<-pos.z<< " )" << endl;
    // BOUNDING BOX
    if(cType->type != ModelType::PointLight){
        float dimAABB = node->CalculateBoundingBox();
        // TODO: el CDimensions solo para NavMesh y para la creacion de PowerUps, no en los coches (o ya veremos)
        shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(dimAABB*2, dimAABB*2, dimAABB*2);
        cDimensions->boundingBoxMesh = GetBoundingByMesh(cId->id); 
        entity->AddComponent(cDimensions);  //Le añadimos el componente CDimensions al Entity que sea
    }


    bool hasChassis = entity->HasComponent(CompType::CompBoundingChassis);
        if (hasChassis && Constants::DEBUG_SHOW_CHASSIS) {
            auto cChassis = static_cast<CBoundingChassis *>(entity->GetComponent(CompType::CompBoundingChassis).get());
            // primera esfera
            auto radiousSph1 = cChassis->sphereBehind->radius; 
            auto centerSph1 = cChassis->sphereBehind->center;
            //cout << "POS X: " << centerSph1.x << " POS Y: " << centerSph1.y << "POS Z:" << centerSph1.z << endl;
            auto nodeSphere1 = device->AddMesh(device->GetRootNode(),cId->id + Component::ID_DIFFERENCE,"media/sphera.obj");
            nodeSphere1->SetTranslation(glm::vec3(centerSph1.x, centerSph1.y, -centerSph1.z));
            nodeSphere1->SetScalation(glm::vec3(radiousSph1));
            nodeSphere1->SetVisible(false);
            nodeSphere1->SetShaderProgramID(shader->GetProgramID());
            // segunda esfera
            auto radiousSph2 = cChassis->sphereFront->radius;
            auto centerSph2 = cChassis->sphereFront->center;
            //cout << "POS X: " << centerSph2.x << " POS Y: " << centerSph2.y << "POS Z:" << centerSph2.z << endl;
            auto nodeSphere2 = device->AddMesh(device->GetRootNode(),cId->id + Component::ID_DIFFERENCE + Component::ID_DIFFERENCE,"media/sphera.obj");
            nodeSphere2->SetTranslation(glm::vec3(centerSph2.x, centerSph2.y, -centerSph2.z));
            nodeSphere2->SetScalation(glm::vec3(radiousSph2));
            nodeSphere2->SetVisible(false);
            nodeSphere2->SetShaderProgramID(shader->GetProgramID());
        }
    

    //Si tiene ruedas porque es un coche se las añadimos
    if(entity->HasComponent(CompType::WheelComp)){
        //Importante el padre sera el propio node que acabamos de crear que es el coche
        auto cWheel = static_cast<CWheel*>(entity->GetComponent(CompType::WheelComp).get());
        auto wheel1 = device->AddMesh(node,cWheel->IdWheelTopLeft,"media/"+cWheel->meshTopLeft);
        auto wheel2 = device->AddMesh(node,cWheel->IdWheelTopRight,"media/"+cWheel->meshTopRight);
        auto wheel3 = device->AddMesh(node,cWheel->IdWheelBottomLeft,"media/"+cWheel->meshBottomLeft);
        auto wheel4 = device->AddMesh(node,cWheel->IdWheelBottomRight,"media/"+cWheel->meshBottomRight);


        wheel1->SetTranslation(glm::vec3(cWheel->offsetTopLeft.x,cWheel->offsetTopLeft.y,-cWheel->offsetTopLeft.z));
        wheel2->SetTranslation(glm::vec3(cWheel->offsetTopRight.x,cWheel->offsetTopRight.y,-cWheel->offsetTopRight.z));
        wheel3->SetTranslation(glm::vec3(cWheel->offsetBottomLeft.x,cWheel->offsetBottomLeft.y,-cWheel->offsetBottomLeft.z));
        wheel4->SetTranslation(glm::vec3(cWheel->offsetBottomRight.x,cWheel->offsetBottomRight.y,-cWheel->offsetBottomRight.z));

        wheel1->SetRotation(glm::vec3(cWheel->rotationTopLeft.x,Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopLeft.y),cWheel->rotationTopLeft.z));
        wheel2->SetRotation(glm::vec3(cWheel->rotationTopRight.x,Utils::IrrlichtAngleToOpenGL(cWheel->rotationTopRight.y),cWheel->rotationTopRight.z));
        wheel3->SetRotation(glm::vec3(cWheel->rotationBottomLeft.x,Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomLeft.y),cWheel->rotationBottomLeft.z));
        wheel4->SetRotation(glm::vec3(cWheel->rotationBottomRight.x,Utils::IrrlichtAngleToOpenGL(cWheel->rotationBottomRight.y),cWheel->rotationBottomRight.z));

        wheel1->SetScalation(cWheel->scaleTopLeft);
        wheel2->SetScalation(cWheel->scaleTopRight);
        wheel3->SetScalation(cWheel->scaleBottomLeft);
        wheel4->SetScalation(cWheel->scaleBottomRight);

        wheel1->SetShaderProgramID(shader->GetProgramID());
        wheel2->SetShaderProgramID(shader->GetProgramID());
        wheel3->SetShaderProgramID(shader->GetProgramID());
        wheel4->SetShaderProgramID(shader->GetProgramID());
    }

    return cId->id;
}



const uint16_t RenderFacadeClover::FacadeAddStaticObject(Entity* entity) {

    //Comprobamos si el objeto debe añadirse a la raiz o a algun nodo padre
    CLNode* father = smgr;
    if(entity->HasComponent(CompType::ParentNodeComp)){
        auto cParentNode = static_cast<CParentNode*>(entity->GetComponent(CompType::ParentNodeComp).get());
        father = device->GetNodeByID(cParentNode->idParentNode);
    } 

    auto cTransformable = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    auto cType = static_cast<CType*>(entity->GetComponent(CompType::TypeComp).get());
    auto cShader = static_cast<CShader*>(entity->GetComponent(CompType::ShaderComp).get());

    CLResourceMesh* mesh = nullptr;
    // CLResourceMaterial* mat = nullptr;
    if(entity->HasComponent(CompType::MeshComp)){
        auto cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());
        std::string currentMesh = cMesh->activeMesh;
        std::string meshPath = "media/" + currentMesh;
        mesh = resourceManager->GetResourceMesh(meshPath, false);
        // mat = resourceManager->GetResourceMaterial(meshPath);
    } 


    CLNode* node = nullptr;
    // añadimos el node al sceneManager dependiendo del tipo de node que sea
    switch (cType->type) {
        case ModelType::Sphere:
            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            //static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);
            break;
        case ModelType::Cube:
            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            break;
        case ModelType::StaticMesh: {
            // auto cAnimation = static_cast<CAnimation*>(entity->GetComponent(CompType::AnimationComp).get());
            // std::string path = cAnimation->activeAnimation->path;
            // std::string animationPath = "media/" + path;
            // vector<CLResourceMesh*> clAnimations = resourceManager->GetResourceAnimation(animationPath, cAnimation->activeAnimation->numKeyFrames, false);
            // // mat = resourceManager->GetResourceMaterial(animationPath);
            // //node = father->AddMesh(cId->id); 
            // node = device->AddMesh(father,cId->id);

            // if (cAnimation->activeAnimation->IsInterpolated()) {
            //     static_cast<CLMesh*>(node->GetEntity())->SetAnimationInterpolated(clAnimations, cAnimation->activeAnimation->GetDistances(), cAnimation->activeAnimation->loop);
            // } else {
            //     static_cast<CLMesh*>(node->GetEntity())->SetAnimation(clAnimations, cAnimation->activeAnimation->loop);
            // }
            // //static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);
        }   break;
        case ModelType::AnimatedMesh:
            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            //static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);
            break;
        case ModelType::Shield:
            node = device->AddMesh(father,cId->id);
            static_cast<CLMesh*>(node->GetEntity())->SetMesh(mesh);
            //static_cast<CLMesh*>(node->GetEntity())->SetMaterial(mat);
            node->SetVisible(false);
            break;
        case ModelType::Text:
            //node = father->AddMesh(cId->id);
            node = device->AddMesh(father,cId->id);

            break;
        case ModelType::PointLight:{
            auto cLight = static_cast<CLight*>(entity->GetComponent(CompType::LightComp).get());
            //node = father->AddPointLight(cId->id,cLight->intensity,cLight->ambient,cLight->diffuse,cLight->specular,cLight->constant,cLight->linear,cLight->quadratic);
            node = device->AddPointLight(father,cId->id,cLight->intensity,cLight->ambient,cLight->diffuse,cLight->specular,cLight->constant,cLight->linear,cLight->quadratic);
            
            break;
        }
        case ModelType::ParticleSystem:{
            auto cParticleSystem = static_cast<CParticleSystem*>(entity->GetComponent(CompType::ParticleSystemComp).get());
 
            if(cParticleSystem->radious != 0){  
                //Es circulo o esfera
                if(cParticleSystem->orientation == glm::vec3(0.0f,0.0f,0.0f)){
                    //Es esfera
                    //node = father->AddParticleSystem(cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->flags);
                    node = device->AddParticleSystem(father,cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->flags);
                }else{
                    //Es circulo
                    //node = father->AddParticleSystem(cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->orientation,cParticleSystem->flags);
                    node = device->AddParticleSystem(father,cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->radious,cParticleSystem->orientation,cParticleSystem->flags);
                }
            }else{
                //Es punto, linea, cuadrado o cubo
                node = device->AddParticleSystem(father,cId->id,cParticleSystem->nParticles,cParticleSystem->velocity,cParticleSystem->textures,cParticleSystem->width,cParticleSystem->height,cParticleSystem->spawnDelay,cParticleSystem->particlesToSpawn,cParticleSystem->lifeSpan,cParticleSystem->offset,cParticleSystem->orientation,cParticleSystem->flags);
            }
            static_cast<CLParticleSystem*>(node->GetEntity())->SetLoop(cParticleSystem->loop);

            if(cParticleSystem->started){
                static_cast<CLParticleSystem*>(node->GetEntity())->Start();
            }
            node->SetTranslation(glm::vec3(cTransformable->position.x,cTransformable->position.y,-cTransformable->position.z));
            node->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));
            node->SetScalation(cTransformable->scale);

            return cId->id;
            break;
        }
            
    } 

    auto shader = resourceManager->GetResourceShader(cShader->vertexShader,cShader->fragmentShader);
    node->SetShaderProgramID(shader->GetProgramID());

    //Seteamos valores
    node->SetTranslation(glm::vec3(cTransformable->position.x,cTransformable->position.y,-cTransformable->position.z));
    node->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));
    node->SetScalation(cTransformable->scale);

    //auto pos = cTransformable->position;
    //cout << " la posicion donde esta la entidad es ( " << pos.x<< " , " << pos.y<< " , " <<-pos.z<< " )" << endl;
    // BOUNDING BOX
    if(cType->type != ModelType::PointLight){
        float dimAABB = node->CalculateBoundingBox();
        // TODO: el CDimensions solo para NavMesh y para la creacion de PowerUps, no en los coches (o ya veremos)
        shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(dimAABB*2, dimAABB*2, dimAABB*2);
        cDimensions->boundingBoxMesh = GetBoundingByMesh(cId->id); 
        entity->AddComponent(cDimensions);  //Le añadimos el componente CDimensions al Entity que sea

        // y creamos el frustun OBB
        node->CalculateBoundingBoxOBB();

    }
    return cId->id;
}











/**
 * Añade el coche principal
 */
const uint16_t RenderFacadeClover::FacadeAddObjectCar(Entity* entity) {
    idCar = FacadeAddObject(entity, true);
    return idCar;
}

/**
 * Añade el totem
 */
const uint16_t RenderFacadeClover::FacadeAddObjectTotem(Entity* entity) {
    idTotem = FacadeAddObject(entity);
    return idTotem;
}



/**
 * Actualiza la camara
 * @param { camara , todos los coches para saber quien tiene el totem a seguir}
 */
void RenderFacadeClover::UpdateCamera(Entity* cam, ManCar* manCars) {
    
    
    //Cogemos los componentes de la camara
    auto cTransformable = static_cast<CTransformable*>(cam->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());

    //auto cTransformableCar = static_cast<CTransformable*>(manCars->GetCar()->GetComponent(CompType::IdComp).get());

    //Cogemos la posicion de nuestro coche
    glm::vec3 targetPosition = device->GetNodeByID(idCar)->GetTranslation();
    //glm::vec3 targetPosition = cTransformableCar->position;


    auto cameraEntity = static_cast<CLCamera*>(camera1->GetEntity());
    targetPosition.y += cCamera->upTargetDistance;

    if(cCamera->camType == CamType::INVERTED_CAM){
        targetPosition.y += 0;

        float distX = abs(cTransformable->position.x - targetPosition.x);
        float distZ = abs(-cTransformable->position.z - targetPosition.z);

        if(cTransformable->position.x - targetPosition.x < 0)   targetPosition.x = targetPosition.x - (2*distX);
        else                                                    targetPosition.x = targetPosition.x + (2*distX);

        if(-cTransformable->position.z - targetPosition.z < 0)  targetPosition.z = targetPosition.z - (2*distZ);
        else                                                    targetPosition.z = targetPosition.z + (2*distZ);

        //float angleRotation = (60 * M_PI) / 180.0;
        cameraEntity->SetFOV(60);
        cameraEntity->SetCameraTarget(glm::vec3(targetPosition.x,targetPosition.y,targetPosition.z));
        camera1->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y-5, -cTransformable->position.z));
        //camera1->SetRotation(glm::vec3(cTransformable->rotation.x,cTransformable->rotation.y,cTransformable->rotation.z));
        
    }else if(cCamera->camType == CamType::NORMAL_CAM){
        //float angleRotation = (70 * M_PI) / 180.0;

        cameraEntity->SetCameraTarget(glm::vec3(targetPosition.x,targetPosition.y,targetPosition.z));
        
        cameraEntity->SetFOV(70);
        camera1->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));
        //camera1->SetRotation(glm::vec3(cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),cTransformable->rotation.z));

    }else if (cCamera->camType == CamType::TOTEM_CAM){

        auto car = manCars->GetCar();
        auto cTotemCar = static_cast<CTotem*>(car->GetComponent(CompType::TotemComp).get());
        auto cTransformableCar = static_cast<CTransformable*>(car->GetComponent(CompType::TransformableComp).get());

        //Si somos nosotros quien tenemos el totem ponemos camara normal
        if(cTotemCar->active){
            cameraEntity->SetCameraTarget(glm::vec3(targetPosition.x,targetPosition.y,targetPosition.z));
            cameraEntity->SetFOV(70);
            camera1->SetTranslation(glm::vec3(cTransformable->position.x, cTransformable->position.y, -cTransformable->position.z));

            cCamera->camType = CamType::NORMAL_CAM;
            return;

        }

        auto idCarAIWithTotem = -1;
        //Buscamos el ID del coche que tiene el totem (en caso de tenerlo)
        for(const auto& cars : manCars->GetEntities()){
            if(manCars->GetCar().get() != cars.get()){
                auto cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());
                if(cTotem->active){
                    auto cId = static_cast<CId*>(cars->GetComponent(CompType::IdComp).get());
                    idCarAIWithTotem = cId->id;
                }
            }
        }

        //Nadie tiene el totem
        if(idCarAIWithTotem==-1){
            //Posicion del totem en el suelo
            targetPosition = device->GetNodeByID(idTotem)->GetTranslation();

        }else{
            //Posicion del coche que lleva el totem
            targetPosition = device->GetNodeByID(idCarAIWithTotem)->GetTranslation();

        }

        //Calculamos el angulo hasta el totem
        float vectorX = (cTransformableCar->position.x - targetPosition.x );
        float vectorZ = (-cTransformableCar->position.z - targetPosition.z );

        float valueAtan2 = atan2(vectorZ,vectorX);

        //float angleRotation = (90 * M_PI) / 180.0; 

        
        cameraEntity->SetCameraTarget(glm::vec3(targetPosition.x,targetPosition.y,targetPosition.z));
        
        cameraEntity->SetFOV(90);
        camera1->SetTranslation(glm::vec3(
            cTransformableCar->position.x + 32.5 * cos(valueAtan2), 
            cTransformable->position.y, 
            -cTransformableCar->position.z + 35 * sin(valueAtan2)));

        //camera1->SetRotation(glm::vec3(-cTransformable->rotation.x,Utils::IrrlichtAngleToOpenGL(cTransformable->rotation.y),-cTransformable->rotation.z));
        
    }

    
}

/**
 * Se llama una vez para crear la camara
 */
void RenderFacadeClover::FacadeAddCamera(Entity* camera) {
    auto cId = static_cast<CId*>(camera->GetComponent(CompType::IdComp).get());
    auto cShader = static_cast<CShader*>(camera->GetComponent(CompType::ShaderComp).get());

    camera1 = device->AddCamera(device->GetRootNode(),cId->id);

    //leemos y asignamos el shader
    auto shader = resourceManager->GetResourceShader(cShader->vertexShader,cShader->fragmentShader);
    camera1->SetShaderProgramID(shader->GetProgramID());

    auto cameraEntity = static_cast<CLCamera*>(camera1->GetEntity());

    auto cTransformable = static_cast<CTransformable*>(camera->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera*>(camera->GetComponent(CompType::CameraComp).get());

    float posX = cCamera->tarX - 40.0 * glm::sin( glm::radians( cTransformable->rotation.x ));
    float posZ = cCamera->tarZ - 40.0 * glm::cos(glm::radians( cTransformable->rotation.z ));
    cameraEntity->SetCameraTarget(glm::vec3(cCamera->tarX, cCamera->tarY, cCamera->tarZ));
    camera1->SetTranslation(glm::vec3(posX, cTransformable->position.y+100, posZ));
    camera1->SetRotation(glm::vec3(cTransformable->rotation.x,cTransformable->rotation.y,cTransformable->rotation.z));
    camera1->SetScalation(cTransformable->scale);



    //float dimAABB = node->CalculateBoundingBox();
    //Sacamos sus dimensiones
    float height = 10.0;
    float width = 10.0;
    float depth = 10.0;
    shared_ptr<CDimensions> cDimensions = make_shared<CDimensions>(width, height, depth);
    //cDimensions->boundingBoxMesh = GetBoundingByMesh(cId->id);
    camera->AddComponent(cDimensions);  //Le añadimos el componente CDimensions al Entity que sea

}


void RenderFacadeClover::FacadeUpdateMeshesLoD(vector<shared_ptr<Entity>> entities) {
    for (const auto& entity : entities) {
        CMesh *cMesh = static_cast<CMesh*>(entity->GetComponent(CompType::MeshComp).get());
        if(cMesh->activeMesh != cMesh->previousMesh) {
            CId *cid = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
            auto node = device->GetNodeByID(cid->id);
            if(node) {
                std::string meshPath = "media/" + cMesh->activeMesh;
                // string nameCurrentMesh = static_cast<CLMesh*>(node->GetEntity())->GetMesh()->GetName();
                // cout << "meshPash[" << meshPath << "]  nameCurrentMesh[" << nameCurrentMesh << "]" << endl;
                // if (nameCurrentMesh != meshPath) {
                    //cout << "Vamos a cambiar la malla" << endl;
                    static_cast<CLMesh*>(node->GetEntity())->SetMesh(resourceManager->GetResourceMesh(meshPath, false));
                // }
            }
        }
    }
}


void RenderFacadeClover::FacadeUpdateAnimationsLoD(vector<shared_ptr<Entity>> entities) {
    for (const auto& entity : entities) {
        if(entity->HasComponent(CompType::AnimationComp)) {
            auto cAnimation = static_cast<CAnimation*>(entity->GetComponent(CompType::AnimationComp).get());
            if(cAnimation->animationChanged) {
                // cout << "Cambiamos animación por el LoD" << endl;
                CId *cid = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
                auto node = device->GetNodeByID(cid->id);
                if(node) {
                    std::string path = cAnimation->activeAnimation->path;
                    static_cast<CLMesh*>(node->GetEntity())->ActivateAnimation(path);                    
                }
                cAnimation->animationChanged = false;
            }
        }
    }
}



void RenderFacadeClover::FacadeAnimate(vector<shared_ptr<Entity>> entities) {
    auto cameraEntity = static_cast<CLCamera*>(camera1->GetEntity());
    vec3 pointTarget = cameraEntity->GetCameraTarget();
    pointTarget.z = -pointTarget.z;
    vec3 posCamara = camera1->GetTranslation();
    posCamara.z = -posCamara.z;
    vec3 normalCamara = posCamara - pointTarget;
    
    // cout << "la rotación de la cámara es x["<<normalCamara.x<<"] y["<<normalCamara.y<<"] z["<<normalCamara.z<<"]" << endl;
    // cout << "Hay "<< entities.size() <<" cosas para animar" << endl;
    for (const auto& entity : entities) {
        if(entity->HasComponent(CompType::AnimationComp)) {
            CTransformable *cTrans = static_cast<CTransformable*>(entity->GetComponent(CompType::TransformableComp).get());

            // esto sirve para hacer backclipping. Solo se anima lo que está frente a la cámara
            float mDot = glm::dot(normalCamara, (cTrans->position - posCamara));
            if (mDot < 0) {
                // cout << "Estamos animando algo" << endl;
                CId *cid = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
                auto node = device->GetNodeByID(cid->id);
                if(node) {
                    auto cAnimation = static_cast<CAnimation*>(entity->GetComponent(CompType::AnimationComp).get());
                    if (cAnimation->activeAnimation->IsInterpolated()) {
                        static_cast<CLMesh*>(node->GetEntity())->AnimateInterpolated();
                    } else {
                        static_cast<CLMesh*>(node->GetEntity())->Animate();
                    }
                }
            }
        }
    }
}
 
/**
 * @return - ¿El juego sigue abierto?
 */
bool RenderFacadeClover::FacadeRun() {
    return device->Run();
}

/**
 * @return - Tiempo de glfw
 */
uint32_t RenderFacadeClover::FacadeGetTime() const{
    return device->GetTime();
}

void RenderFacadeClover::ThrowEventChangeToMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline){
}

/**
 * Devuelve los fps a los que vamos
 */
int RenderFacadeClover::FacadeGetFPS() const{
    return 1;
}

/**
 * Cambia el titulo de la ventana
 */
void RenderFacadeClover::FacadeSetWindowCaption(std::string title, int fps) const{
    std::string name = title + " - " + std::to_string(fps) + " FPS";
    device->SetTitle(name);
}

/**
 * Redimensiona la pantalla
 */
void RenderFacadeClover::FacadeSetWindowSize(DataMap* d) {
    auto w = any_cast<int>((*d)[SCREEN_WIDTH]);
    auto h = any_cast<int>((*d)[SCREEN_HEIGHT]);
    device->SetScreenHeight(h);
    device->SetScreenWidth(w);
}

//////////////////////////
//  INICIA LOS MENUS    //
//////////////////////////

/**
 * Cargamos lo necesario para la partida.
 */
void RenderFacadeClover::FacadeInitResources(mainCharacter character){
    
    auto w = device->GetScreenWidth();
    auto scale = 0.5f;
    if ( w > 1600 ) { scale = 0.75f; }
    else if (w < 600 ) { scale = 0.25f; }
    std::string file = "media/menu/loading_screen.png";


    FacadeBeginScene();
    //Esto es lo unico que se necesita para dibujar la pantalla de carga
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    int64_t time = Utils::getMicrosSinceEpoch();
    int indx = time % (tipsTexts.size()-1);
    device->RenderText2D(tipsTexts.at(indx), device->GetScreenWidth()/2 - 125.0f*scale, device->GetScreenHeight()/2,0.5f, scale,glm::vec3(1.0f,1.0f,1.0f));
    FacadeEndScene();

    //Cargamos todas las mallas
    //Mallas
    resourceManager->GetResourceMesh("media/kart_physics.obj");
    resourceManager->GetResourceMesh("media/melon.obj");
    resourceManager->GetResourceMesh("media/totem.obj");
    resourceManager->GetResourceMesh("media/TEST_BOX.obj");
    resourceManager->GetResourceMesh("media/pudin.obj");
    resourceManager->GetResourceMesh("media/telebanana.obj");

    // cargamos TODAS las animaciones de win, porque cualquiera puede ganar
    // resourceManager->GetResourceAnimation("media/animations/sharky/win/victorysharky_000001.obj", 31, false);
    // resourceManager->GetResourceAnimation("media/animations/kong/win/victorykong_000001.obj", 31, false);
    // resourceManager->GetResourceAnimation("media/animations/cyberoctopus/win/victoryoctopus_000001.obj", 26, false);
    // resourceManager->GetResourceAnimation("media/animations/dragon/win/victorydragon_000001.obj", 31, false);
    // resourceManager->GetResourceAnimation("media/animations/penguin/win/victorypenguin_000001.obj", 31, false);
    // resourceManager->GetResourceAnimation("media/animations/baxter/win/victorybaxter_000001.obj", 31, false);

    /*resourceManager->GetResourceAnimation("media/animations/sharky/win/victorysharky_000001.obj", 10, false);
    resourceManager->GetResourceAnimation("media/animations/kong/win/victorykong_000001.obj", 10, false);

    FacadeBeginScene();
    //Esto es lo unico que se necesita para dibujar la pantalla de carga
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    time = Utils::getMicrosSinceEpoch();
    indx = time % (tipsTexts.size()-1);
    device->RenderText2D(tipsTexts.at(indx), device->GetScreenWidth()/2 - 125.0f*scale, device->GetScreenHeight()/2,0.5f, scale,glm::vec3(1.0f,1.0f,1.0f));
    FacadeEndScene();


    resourceManager->GetResourceAnimation("media/animations/cyberoctopus/win/victoryoctopus_000001.obj", 10, false);
    resourceManager->GetResourceAnimation("media/animations/dragon/win/victorydragon_000001.obj", 10, false);
    
    
    FacadeBeginScene();
    //Esto es lo unico que se necesita para dibujar la pantalla de carga
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    time = Utils::getMicrosSinceEpoch();
    indx = time % (tipsTexts.size()-1);
    device->RenderText2D(tipsTexts.at(indx), device->GetScreenWidth()/2 - 125.0f*scale, device->GetScreenHeight()/2,0.5f, scale,glm::vec3(1.0f,1.0f,1.0f));
    FacadeEndScene();


    resourceManager->GetResourceAnimation("media/animations/penguin/win/victorypenguin_000001.obj", 10, false);
    resourceManager->GetResourceAnimation("media/animations/baxter/win/victorybaxter_000001.obj", 10, false);*/


    FacadeBeginScene();
    //Esto es lo unico que se necesita para dibujar la pantalla de carga
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    time = Utils::getMicrosSinceEpoch();
    indx = time % (tipsTexts.size()-1);
    device->RenderText2D(tipsTexts.at(indx), device->GetScreenWidth()/2 - 125.0f*scale, device->GetScreenHeight()/2,0.5f, scale,glm::vec3(1.0f,1.0f,1.0f));
    FacadeEndScene();



    // cargamos el resto de animaciones del personaje principal
    switch(character) {
        case mainCharacter::PENGUIN:
            resourceManager->GetResourceAnimation("media/animations/penguin/right/turnrightpenguin_000001.obj", 10, false);
            resourceManager->GetResourceAnimation("media/animations/penguin/left/turnleftpenguin_000001.obj", 10, false);
            break;
        case mainCharacter::TIGER:
            resourceManager->GetResourceAnimation("media/animations/baxter/right/turnrightbaxter_000001.obj", 10, false);
            resourceManager->GetResourceAnimation("media/animations/baxter/left/turnleftbaxter_000001.obj", 10, false);
            break;
        case mainCharacter::GORILLA:
            resourceManager->GetResourceAnimation("media/animations/kong/right/turnrightkong_000001.obj", 10, false);
            resourceManager->GetResourceAnimation("media/animations/kong/left/turnleftkong_000001.obj", 10, false);
            break;
        case mainCharacter::SHARK:
            resourceManager->GetResourceAnimation("media/animations/sharky/right/turnrightsharky_000001.obj", 10, false);
            resourceManager->GetResourceAnimation("media/animations/sharky/left/turnleftsharky_000001.obj", 10, false);
            break;
        case mainCharacter::DRAGON:
            resourceManager->GetResourceAnimation("media/animations/dragon/right/turnrightdragon_000001.obj", 10, false);
            resourceManager->GetResourceAnimation("media/animations/dragon/left/turnleftdragon_000001.obj", 10, false);
            break;
        case mainCharacter::OCTOPUS:
            resourceManager->GetResourceAnimation("media/animations/cyberoctopus/right/turnrightoctopus_000001.obj", 10, false);
            resourceManager->GetResourceAnimation("media/animations/cyberoctopus/left/turnleftoctopus_000001.obj", 10, false);
            break;
    }

    //Shaders
    resourceManager->GetResourceShader("CLEngine/src/Shaders/cartoonShader.vert", "CLEngine/src/Shaders/cartoonShader.frag");

    //Reseteo el showTableMinimap
    showTableMinimap = Constants::ShowTableMinimap::BOTH;

}

void RenderFacadeClover::FacadeInitIntro() {


}

void RenderFacadeClover::FacadeInitMenu() {
    // -------- CARGAMOS TEXTURAS

    // HUD - POWERUPS
    currentPowerUp = 0;
    powerUps[0] = "media/nonepowerup.png";
    powerUps[1] = "media/robojorobo.png";
    powerUps[2] = "media/nitro.png";
    powerUps[3] = "media/pudin.png";
    powerUps[4] = "media/escudomerluzo.png";
    powerUps[5] = "media/telebanana.png";
    powerUps[6] = "media/melonmolon.png";
    device->GetResourceManager()->GetResourceTexture(powerUps[0], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[1], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[2], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[3], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[4], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[5], true);
    device->GetResourceManager()->GetResourceTexture(powerUps[6], true);

    // HUD - Countdown
    resourceManager->GetResourceTexture("media/1.png", true);
    resourceManager->GetResourceTexture("media/2.png", true);
    resourceManager->GetResourceTexture("media/3.png", true);

    // HUD - Caritas y minimapa
    resourceManager->GetResourceTexture("media/hudPenguin.png", true);
    resourceManager->GetResourceTexture("media/hudTiger.png", true);
    resourceManager->GetResourceTexture("media/hudShark.png", true);
    resourceManager->GetResourceTexture("media/hudGorilla.png", true);
    resourceManager->GetResourceTexture("media/hudDragon.png", true);
    resourceManager->GetResourceTexture("media/hudOctopus.png", true);
    resourceManager->GetResourceTexture("media/totemCogido.png", true);
    resourceManager->GetResourceTexture("media/totemSuelo.png", true);
    resourceManager->GetResourceTexture("media/Minimapa240v2.png", true);

    // HUD - Marcadores
    resourceManager->GetResourceTexture("media/indicator_tiempo.png", true);
    resourceManager->GetResourceTexture("media/indicator_totem.png", true);
    resourceManager->GetResourceTexture("media/ranking.png", true);

    // HUD - Avisos
    resourceManager->GetResourceTexture("media/stoleHUD.png", true);
    resourceManager->GetResourceTexture("media/loseHUD.png", true);
    resourceManager->GetResourceTexture("media/catchHUD.png", true);

    // HUD - Nameplates
    resourceManager->GetResourceTexture("media/BOctopus.png", false);
    resourceManager->GetResourceTexture("media/BDragon.png", false);
    resourceManager->GetResourceTexture("media/BGorila.png", false);
    resourceManager->GetResourceTexture("media/BPinguino.png", false);
    resourceManager->GetResourceTexture("media/BTiburon.png", false);
    resourceManager->GetResourceTexture("media/hudTotemBack.png", true);
    resourceManager->GetResourceTexture("media/hudTotemLeft.png", true);
    resourceManager->GetResourceTexture("media/hudTotemRight.png", true);
    resourceManager->GetResourceTexture("media/hudTotem.png", true);

    // Particulas
    resourceManager->GetResourceTexture("media/particleTriangleBlack.png", true);
    resourceManager->GetResourceTexture("media/particleTriangleGreen.png", true);
    resourceManager->GetResourceTexture("media/particleStarBlack.png", true);
    resourceManager->GetResourceTexture("media/particleStarGreen.png", true);
    resourceManager->GetResourceTexture("media/particleYellowTriangle.png", true);
    resourceManager->GetResourceTexture("media/particleYellowStar.png", true);
    resourceManager->GetResourceTexture("media/particleTriangleGrey.png", true);
    resourceManager->GetResourceTexture("media/particleStarMarron.png", true);
    resourceManager->GetResourceTexture("media/particleTriangleBrown.png", true);
    resourceManager->GetResourceTexture("media/particleRedTriangle.png", true);
    resourceManager->GetResourceTexture("media/particleRedStar.png", true);
}

void RenderFacadeClover::FacadeInitSelectCharacter() {
    std::string file = "media/menu/loading_screen.png";
    auto w = device->GetScreenWidth();
    auto scale = 0.5f;
    if ( w > 1600 ) { scale = 0.75f; }
    else if (w < 600 ) { scale = 0.25f; }

    // --- Pantalla de carga
    //Que se cargen poco a poco cuando se van usando o tarda mucho
    FacadeBeginScene();
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    //Frase
    int64_t time = Utils::getMicrosSinceEpoch();
    int indx = time % (tipsTexts.size()-1);
    device->RenderText2D(tipsTexts.at(indx), device->GetScreenWidth()/2 - 125.0f*scale, device->GetScreenHeight()/2,0.5f, scale,glm::vec3(1.0f,1.0f,1.0f));
    FacadeEndScene();

    FacadeReleaseMeshesInGame();
    
    if(device->GetNodeByID(0))
        return;

    auto cam = device->AddCamera(device->GetRootNode(),10);
    auto shaderCam = resourceManager->GetResourceShader("CLEngine/src/Shaders/lightMapping.vert","CLEngine/src/Shaders/lightMapping.frag");
    cam->SetShaderProgramID(shaderCam->GetProgramID());
    auto cameraEntity = static_cast<CLCamera*>(cam->GetEntity());
    cameraEntity->SetCameraTarget(glm::vec3(0.0f, 0.0, 0.0));
    cam->SetTranslation(glm::vec3(40.0f,12.0f,0.0f));
    cam->SetRotation(glm::vec3(0.0f));

    CLNode* mesh = nullptr;
    auto shader = resourceManager->GetResourceShader("CLEngine/src/Shaders/basicShader.vert","CLEngine/src/Shaders/basicShader.frag");

    //Penguin
    // cargamos el recurso en memoria (o lo obtenemos si ya estaba cargado)
    auto animationPen = resourceManager->GetResourceAnimation("media/animations/penguin/selection/selectionpenguin_000001.obj", 10, false);
    mesh = device->AddMesh(smgr,0);
    // Creamos un objeto clanimation
    shared_ptr<CLAnimation> animPen = make_shared<CLAnimation>("selecPen", animationPen, false);
    // Se lo añadimos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->AddAnimation(animPen->name, animPen);
    // Se lo activamos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->ActivateAnimation(animPen->name);
    mesh->SetScalation(glm::vec3(2.0f));
    mesh->SetTranslation(glm::vec3(0.0f,-14.0f,-20.0f));
    mesh->SetRotation(glm::vec3(10.0f,-50.0f, 5.0f));
    mesh->SetShaderProgramID(shader->GetProgramID());

    //Tiger
    auto animationTig = resourceManager->GetResourceAnimation("media/animations/baxter/selection/selectionbaxter_000001.obj", 10, false);
    mesh = device->AddMesh(smgr,1);
    shared_ptr<CLAnimation> animTig = make_shared<CLAnimation>("selecTig", animationTig, false);
    // Se lo añadimos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->AddAnimation(animTig->name, animTig);
    // Se lo activamos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->ActivateAnimation(animTig->name);
    mesh->SetScalation(glm::vec3(2.0f));
    mesh->SetTranslation(glm::vec3(0.0f,-14.0f,-20.0f));
    mesh->SetRotation(glm::vec3(10.0f,-50.0f, 5.0f));
    mesh->SetShaderProgramID(shader->GetProgramID());


    FacadeBeginScene();
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    //Frase
    time = Utils::getMicrosSinceEpoch();
    indx = time % (tipsTexts.size()-1);
    device->RenderText2D(tipsTexts.at(indx), device->GetScreenWidth()/2 - 125.0f*scale, device->GetScreenHeight()/2,0.5f, scale,glm::vec3(1.0f,1.0f,1.0f));
    FacadeEndScene();

    //Shark
    auto animationSha = resourceManager->GetResourceAnimation("media/animations/sharky/selection/selectionsharky_000001.obj", 10, false);
    mesh = device->AddMesh(smgr,2);
    shared_ptr<CLAnimation> animSha = make_shared<CLAnimation>("selecSha", animationSha, false);
    // Se lo añadimos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->AddAnimation(animSha->name, animSha);
    // Se lo activamos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->ActivateAnimation(animSha->name);
    mesh->SetScalation(glm::vec3(2.0f));
    mesh->SetTranslation(glm::vec3(0.0f,-14.0f,-20.0f));
    mesh->SetRotation(glm::vec3(10.0f,-50.0f, 5.0f));
    mesh->SetShaderProgramID(shader->GetProgramID());

    //Gorila
    auto animationKong = resourceManager->GetResourceAnimation("media/animations/kong/selection/selectionkong_000001.obj", 10, false);
    mesh = device->AddMesh(smgr,3);
    shared_ptr<CLAnimation> animKon = make_shared<CLAnimation>("selecKon", animationKong, false);
    // Se lo añadimos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->AddAnimation(animKon->name, animKon);
    // Se lo activamos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->ActivateAnimation(animKon->name);
    mesh->SetScalation(glm::vec3(2.0f));
    mesh->SetTranslation(glm::vec3(0.0f,-14.0f,-20.0f));
    mesh->SetRotation(glm::vec3(10.0f,-50.0f, 5.0f));
    mesh->SetShaderProgramID(shader->GetProgramID());


    FacadeBeginScene();
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
    //Frase
    time = Utils::getMicrosSinceEpoch();
    indx = time % (tipsTexts.size()-1);
    device->RenderText2D(tipsTexts.at(indx), device->GetScreenWidth()/2 - 125.0f*scale, device->GetScreenHeight()/2,0.5f, scale,glm::vec3(1.0f,1.0f,1.0f));
    FacadeEndScene();

    //Dragon
    auto animationDra = resourceManager->GetResourceAnimation("media/animations/dragon/selection/selectiondragon_000001.obj", 10, false);
    mesh = device->AddMesh(smgr,4);
    shared_ptr<CLAnimation> animDra = make_shared<CLAnimation>("selecDra", animationDra, false);
    // Se lo añadimos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->AddAnimation(animDra->name, animDra);
    // Se lo activamos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->ActivateAnimation(animDra->name);
    mesh->SetScalation(glm::vec3(2.0f));
    mesh->SetTranslation(glm::vec3(0.0f,-14.0f,-20.0f));
    mesh->SetRotation(glm::vec3(10.0f,-50.0f, 5.0f));
    mesh->SetShaderProgramID(shader->GetProgramID());


    //Octopus
    auto animationCyb = resourceManager->GetResourceAnimation("media/animations/cyberoctopus/selection/selectionoctopus_000001.obj", 10, false);
    mesh = device->AddMesh(smgr,5);
    shared_ptr<CLAnimation> animOct = make_shared<CLAnimation>("selecOct", animationCyb, false);
    // Se lo añadimos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->AddAnimation(animOct->name, animOct);
    // Se lo activamos al nodo
    static_cast<CLMesh*>(mesh->GetEntity())->ActivateAnimation(animOct->name);
    mesh->SetScalation(glm::vec3(2.0f));
    mesh->SetTranslation(glm::vec3(0.0f,-14.0f,-20.0f));
    mesh->SetRotation(glm::vec3(10.0f,-50.0f, 5.0f));
    mesh->SetShaderProgramID(shader->GetProgramID());


    //IMGs
    //Que se cargen poco a poco cuando se van usando o tarda mucho

}

void RenderFacadeClover::FacadeInitGameOptions() {

    //Que se cargen poco a poco cuando se van usando o tarda mucho
    
}

void RenderFacadeClover::FacadeInitTournamentOptions() {
    //Que se cargen poco a poco cuando se van usando o tarda mucho
}

void RenderFacadeClover::FacadeInitControler() {
    //Que se cargen poco a poco cuando se van usando o tarda mucho
}

void RenderFacadeClover::FacadeInitCredits() {
    if(!creditsAnimation){
        creditsAnimation = make_unique<Animation2D>("media/animacionCreditos/creditos.jpg",2291,60);
        creditsAnimation->Start();
        creditsAnimation->Restart();
    }else{
        creditsAnimation->Start();
        creditsAnimation->Restart();
    }
    
    

    

}

void RenderFacadeClover::FacadeInitPause() {
    
    //Que se cargen poco a poco cuando se van usando o tarda mucho
}

void RenderFacadeClover::FacadeInitEndRace() {
    timeAnimationEnd = system_clock::now();
    numShowPanel = 0;
}

void RenderFacadeClover::FacadeInitEndTournament() {
    InputFacadeManager::GetInstance()->GetInputFacade()->PressedA();
    timeAnimationEnd = system_clock::now();
    numShowPanel = 0;
    // calculate tournament points
    //GameValues::GetInstance()->GetTotalPoints()[0] = GameValues::GetInstance()->GetRanking().find()
    auto rank = GameValues::GetInstance()->GetRanking();
    auto seconds = GameValues::GetInstance()->GetSeconds();
    auto idPlayer = GameValues::GetInstance()->GetCharacter();
    auto idOther = GameValues::GetInstance()->GetIACharacters();
    auto pointsTotal = GameValues::GetInstance()->GetTotalPoints();
    auto pointsRank = GameValues::GetInstance()->GetPosPoints();

    auto antTime = -1;
    auto posPoints = 0;
    for(auto it = rank.begin(); it != rank.end(); ++it) {

        auto it2 = seconds.find(it->first);
        if ( it2 != seconds.end()) {
            auto actualTime = it2->second;
            //cout << "TIEMPO ANTERIOR: " << antTime << endl;
            //cout << "TIEMPO ACTUAL: " << actualTime << endl;
            if (antTime != -1 && antTime != actualTime) { posPoints++; }
            //cout << "POSICION DE PUNTOS: " << posPoints << endl;
            antTime = actualTime;

            if(it->second == idPlayer){
                pointsTotal[0] += pointsRank[posPoints/*it->first-1*/];
            }else{
                for(size_t i = 0; i<idOther.size(); i++){
                    if(it->second == idOther[i]){
                        pointsTotal[i+1] += pointsRank[posPoints/*it->first-1*/];
                    }
                }
            }
            //cout << "PONGO " << pointsRank[posPoints] << " PUNTOS" << endl;
            //cout << "------------------------" << endl;
        }
    }

    //for(auto it = rank.begin(); it != rank.end(); ++it){
    //    cout << "Pos: " << it->first << "  Jugador: " << it->second << "\n";
    //}

    //for(auto p : pointsTotal){
    //    cout << "Puntos: " << p << "\n";
    //}

    std::multimap<uint16_t, uint16_t, std::greater<uint16_t>> rankPoints;
    uint8_t playersSaved = 0;
    for(auto point : pointsTotal){
        if(playersSaved>=GameValues::GetInstance()->GetNumPlayers())
            break;

        if(playersSaved==0)
            rankPoints.insert(std::pair<uint16_t,uint16_t>(point,idPlayer));
        else
            rankPoints.insert(std::pair<uint16_t,uint16_t>(point,idOther[playersSaved-1]));
        playersSaved++;
    }
    
    /*for(auto it = rankPoints.begin(); it != rankPoints.end(); ++it){
        cout << "Puntos: " << it->first << "  Jugador: " << it->second << "\n";
    }*/

    GameValues::GetInstance()->SetTotalPoints(pointsTotal);
    GameValues::GetInstance()->SetRankingPoints(rankPoints);
}

void RenderFacadeClover::FacadeInitLobbyMulti() {
    FacadeInitSelectCharacter();
}

//TODO: No se usa
void RenderFacadeClover::FacadeInitHUD() {

}

void RenderFacadeClover::FacadeInitSettings() {
    
    //Que se cargen poco a poco cuando se van usando o tarda mucho

}


//////////////////////
//  CHECK INPUTS    //
//////////////////////


void RenderFacadeClover::FacadeCheckInputSingle() {
    //inputShowTable = InputFacadeManager::GetInstance()->GetInputFacade()->ShowTable(inputShowTable);
    InputFacadeManager::GetInstance()->GetInputFacade()->ShowTable(showTableMinimap);
}

vector<Constants::InputTypes> RenderFacadeClover::FacadeCheckInputMulti() {
    vector<Constants::InputTypes> inputs;
    InputFacadeManager::GetInstance()->GetInputFacade()->ShowTable(showTableMinimap);

    return inputs;
}

void RenderFacadeClover::FacadeCheckInputIntro() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputIntro();
}

void RenderFacadeClover::FacadeCheckInputMenu() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputMenu(inputMenu, maxInputMenu);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputSelectCharacter() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputSelectCharacter(inputSC, maxInputSC);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputGameOptions() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputGameOptions(inputGO, maxInputGO, option);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
    multiplayer = false;
}

void RenderFacadeClover::FacadeCheckInputTournamentOptions() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputTournamentOptions(inputTO, maxInputTO, optionTO);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
    multiplayer = false;
}

void RenderFacadeClover::FacadeCheckInputControler() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputController();
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputCredits() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputCredits();
}

void RenderFacadeClover::FacadeCheckInputPause() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputPause(inputPause, maxInputPause);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputEndRace() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputEndRace(inputER, maxInputER, menuER);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputEndTournament() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputEndTournament(inputET, maxInputET, menuET);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputLobbyMultiConnecting() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputLobbyMultiConnecting();
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputLobbyMultiWait() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputLobbyMultiWait();
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputLobbyMultiExit() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputLobbyMultiExit();
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

void RenderFacadeClover::FacadeCheckInputLobbyMultiSelChar() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputLobbyMultiSelChar(inputSC, maxInputSC);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
    multiplayer = true;
}

void RenderFacadeClover::FacadeCheckInputSettings() {
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputSettings(inputSettings, maxInputSettings, optionSettings);
    gamepadConnected = InputFacadeManager::GetInstance()->GetInputFacade()->IsConectedGamepad();
}

bool RenderFacadeClover::FacadeCheckShowHUD() {
    inputShowHUD = InputFacadeManager::GetInstance()->GetInputFacade()->ShowHud(inputShowHUD);
    return inputShowHUD;
}

//TODO: Deberia ser un evento
void RenderFacadeClover::ResetInputCharacter() {
    inputSC = 0;
    GameValues::GetInstance()->SetCharacter(0);
}

//TODO: Deberia ser un evento
void RenderFacadeClover::ResetInputGameOptions() {
    option = 0;
    inputGO[0] = 1;
    GameValues::GetInstance()->SetGameTime(180);
    inputGO[1] = 1;
    GameValues::GetInstance()->SetTimeTotem(45);
    inputGO[2] = 1;
    GameValues::GetInstance()->SetNumPlayers(4);
    inputGO[3] = 1;
    GameValues::GetInstance()->SetDifficultAI(DifficultyAI::NORMAL);
    //GameValues::GetInstance()->SetTimeTotem(45);
}
void RenderFacadeClover::ResetInputTournamentOptions() {
    optionTO = 0;
    inputTO[0] = 1;
    GameValues::GetInstance()->SetGameTime(180);
    inputTO[1] = 1;
    GameValues::GetInstance()->SetTimeTotem(45);
    inputGO[2] = 1;
    GameValues::GetInstance()->SetNumPlayers(4);
    inputGO[3] = 1;
    GameValues::GetInstance()->SetDifficultAI(DifficultyAI::NORMAL);
    inputTO[4] = 1;
    GameValues::GetInstance()->SetNumBattles(4);
    //GameValues::GetInstance()->SetTimeTotem(45);
}
 

//////////////
//  DRAW    //
//////////////

// TODO: No se usa
void RenderFacadeClover::FacadeDraw() const{

}

void RenderFacadeClover::FacadeDrawHUD(Entity* car, ManCar* manCars, Entity* globalClock, ManHUDEvent* manHud, ManGameRules* manGR, bool ingame) {

    if (!inputShowHUD) { return; }

    device->SetEnableDepthTest(false);
    
    std::string cadena;
    std::string sprite;
    CTotem* cTotem;
    CCar* cCar;
    CTransformable* cTrans;
    bool totemCatch = false;
    auto w = device->GetScreenWidth();
    auto h = device->GetScreenHeight();


    //MINIMAPA
    auto widthMM = 0;           auto heightMM = 0;
    auto posXMiniMap = 0;       auto posYMiniMap = 0;

    if ((showTableMinimap == Constants::ShowTableMinimap::BOTH || showTableMinimap == Constants::ShowTableMinimap::ONLYMAP)) {
        cadena = "media/Minimapa240v2.png";
        auto minimapTexture = resourceManager->GetResourceTexture(cadena);
        if(minimapTexture) {
            widthMM = minimapTexture->GetWidth();
            heightMM = minimapTexture->GetHeight();
            posXMiniMap = w - (widthMM+50);    
            posYMiniMap = h - (heightMM+50);
        }
        device->DrawImage2D(posXMiniMap, posYMiniMap, widthMM, heightMM, 0.9f, cadena, true);
    }


    // POSITION TOTEM HUD
    ////////////////////////////
    //                        //
    //                        //
    // *                    * //
    //                        //
    // *         *          * //
    ////////////////////////////
    if(ingame){
        auto anchoSpriteTotem = 100.0;
        auto altoSpriteTotem = 100.0;
        auto posRefY = h/2 - (altoSpriteTotem/2);
        auto frustum_ = device->GetActiveCamera()->GetFrustum();
        bool backTotem  = frustum_.IsOutBack(manGR->GetPosTotem());
        bool leftTotem  = frustum_.IsOutLeft(manGR->GetPosTotem());
        bool rightTotem = frustum_.IsOutRight(manGR->GetPosTotem());
    
        if(backTotem){
            if(leftTotem && !rightTotem)
                device->DrawImage2D(50, posRefY, 1.f, 0.1f, "media/hudTotemLeft.png", true);
            else if(rightTotem && !leftTotem)
                device->DrawImage2D( (w-50.0f) - anchoSpriteTotem, posRefY , 1.f, 0.1f, "media/hudTotemRight.png", true);
            else
                device->DrawImage2D(w/2 - (anchoSpriteTotem/2) , h - 50 - altoSpriteTotem, 1.f, 0.1f, "media/hudTotemBack.png", true);
        }else{
            if(leftTotem)
                device->DrawImage2D(50, posRefY, 1.f, 0.1f, "media/hudTotemLeft.png", true);
            else if(rightTotem)
                device->DrawImage2D( (w-50.0f) - anchoSpriteTotem, posRefY , 1.f, 0.1f, "media/hudTotemRight.png", true);
        }
    }



    //CURRENT POWERUP
    if(!powerUpAnimation->GetFinished() && currentPowerUp != 0){
        //resourceManager->DeleteResourceTexture(powerUpAnimation->GetCurrentPath());
        powerUpAnimation->Update();
        device->DrawImage2D(50.0f, 25.0f, 150.0f, 150.0f, 0.1f, powerUpAnimation->GetCurrentPath(), true);
    }else{
        device->DrawImage2D(50.0f, 25.0f, 150.0f, 150.0f, 0.1f ,powerUps[currentPowerUp], true);
    }

    // TABLA TIEMPOS
    if (showTableMinimap == Constants::ShowTableMinimap::BOTH || showTableMinimap == Constants::ShowTableMinimap::ONLYTABLE) {

        auto j = 0;
        auto ranking = GameValues::GetInstance()->GetRanking();
        auto seconds = GameValues::GetInstance()->GetSeconds();
        uint8_t positionR = 1;
        int64_t antTime = -1;
        auto rankingSize = (ranking.size() - 1) * 45.0f;
        auto widthRanking = 400.0f;

        for (auto it = ranking.begin(); it != ranking.end(); ++it) {    

            switch (it->second) {
                case static_cast<uint16_t>(mainCharacter::PENGUIN):    sprite = "media/hudPenguin.png";    break;
                case static_cast<uint16_t>(mainCharacter::TIGER):      sprite = "media/hudTiger.png";      break;
                case static_cast<uint16_t>(mainCharacter::SHARK):      sprite = "media/hudShark.png";      break;
                case static_cast<uint16_t>(mainCharacter::GORILLA):    sprite = "media/hudGorilla.png";    break;
                case static_cast<uint16_t>(mainCharacter::DRAGON):     sprite = "media/hudDragon.png";     break;
                case static_cast<uint16_t>(mainCharacter::OCTOPUS):    sprite = "media/hudOctopus.png";    break;
                default:                                                                                   break;
            }

            cadena = "media/ranking.png";
            //device->DrawImage2D(w - 200.0f, 150.0f + j*45.0f, 1.0, 0.9f, cadena, true);
            device->DrawImage2D(50.0f, h - 85.0f - rankingSize + j*45.0f, widthRanking*0.4f, 43.0f, 0.9f, cadena, true);
            
            //device->DrawImage2D(w - 160.0f, 155.0f + j*45.0f, 0.4f, 0.1f*j+0.1f, sprite, true);  //CARITA
            device->DrawImage2D(100.0f, h - 80.0f - rankingSize + j*45.0f, 0.4f, 0.7f, sprite, true);  //CARITA

            auto it2 = seconds.find(it->first);
            if (it2 != seconds.end()) {
                glm::vec3 color = glm::vec3(255.0f,255.0f,255.0f);
                if (antTime != -1 && antTime != it2->second) {
                    ++positionR;
                }
                cadena = std::to_string(positionR) + ".";
                //device->RenderText2D(cadena, w - 190.0f, h - 180.0f - 45.0f*j, 0.1f*j+0.1f, 0.6f, glm::vec3(255.0f,255.0f,255.0f));
                device->RenderText2D(cadena, 65.0f, 55.0f + rankingSize - j*45.0f, 0.5f, 0.6f, glm::vec3(255.0f,255.0f,255.0f));

                antTime = it2->second;
                cadena = std::to_string(antTime);
                //device->RenderText2D(cadena, w - 110.0f, h - 180.0f - 45.0f*j, 0.1f*j+0.1f, 0.6f, color);
                device->RenderText2D(cadena, 148.0f, 55.0f + rankingSize - j*45.0f, 0.5f, 0.6f, color);
            }
            ++j;
        }
    }


    auto i = 8;
    for(const auto& cars : manCars->GetEntities()) {
        cTotem = static_cast<CTotem*>(cars->GetComponent(CompType::TotemComp).get());
        cCar = static_cast<CCar*>(cars->GetComponent(CompType::CarComp).get());
        cTrans = static_cast<CTransformable*>(cars->GetComponent(CompType::TransformableComp).get());
        auto cId = static_cast<CId*>(cars->GetComponent(CompType::IdComp).get());

        if (cCar) {
            switch (cCar->character) {
                case mainCharacter::PENGUIN:    sprite = "media/hudPenguin.png";    break;
                case mainCharacter::TIGER:      sprite = "media/hudTiger.png";      break;
                case mainCharacter::SHARK:      sprite = "media/hudShark.png";      break;
                case mainCharacter::GORILLA:    sprite = "media/hudGorilla.png";    break;
                case mainCharacter::DRAGON:     sprite = "media/hudDragon.png";     break;
                case mainCharacter::OCTOPUS:    sprite = "media/hudOctopus.png";    break;
                default:                                                    break;
            }


            //MARCADOR DE TIEMPO
            auto widthMTotem = 470.0f;
            if (cTotem && cTotem->active) {
                totemCatch = true;
                cadena = "media/indicator_totem.png";
                auto posMarcadorX = w/2 - widthMTotem*0.25f;
                auto posMarcadorY = 32.0f;
                device->DrawImage2D(posMarcadorX, posMarcadorY , 0.5f, 0.2f, cadena, true);

                device->DrawImage2D(posMarcadorX + 160.0f, posMarcadorY + 32.0f, 0.6f, 0.05f, sprite, true);     //CARITA

                int time = cTotem->SEGUNDOS - cTotem->accumulatedTime/1000;
                cadena = std::to_string(time);
                glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

                if(time <= 5) {
                    color = glm::vec3(255.0f, 0.0f, 0.0f);
                }

                device->RenderText2D(cadena, (posMarcadorX + 105.0f), (h - 98.0f), 0.05f, 0.6f, color);
                //break;
            
            }

            // MINIMAPA
            if ( (showTableMinimap == Constants::ShowTableMinimap::BOTH || showTableMinimap == Constants::ShowTableMinimap::ONLYMAP) && manGR && cTrans) {
                
                auto positions = manGR->GetPositionsPlane();
                auto it = positions.find(cId->id);
                if (it != positions.end()) {
                    auto posXPjMM = widthMM - it->second.x * widthMM;
                    auto posYPjMM = it->second.y * heightMM;

                    device->DrawImage2D(posXMiniMap + (posXPjMM - 12), posYMiniMap + (posYPjMM - 12), 0.3f, 0.1f*i, sprite, true);  //CARITA
                } /*else { cout << "NO TENGO VALORES" << endl; }*/
                //cout << "CAR " << cId->id << " CON POS: " << posXPjMM << " - " << posYPjMM << endl;

            }
            --i;
        }  

    }

    //MINIMAPA TOTEM
    auto positionTotem = manGR->GetPositionTotemPlane();
    if((showTableMinimap == Constants::ShowTableMinimap::BOTH || showTableMinimap == Constants::ShowTableMinimap::ONLYMAP)) {
        auto posXTMM = widthMM - positionTotem.x * widthMM;
        auto posYTMM = positionTotem.y * heightMM;
        sprite = "media/totem";
        if (totemCatch) {
            sprite += "Cogido.png";
            device->DrawImage2D(posXMiniMap + (posXTMM - 17), posYMiniMap + (posYTMM - 30), 1.0f, 0.1f, sprite, true);
        } else {
            sprite += "Suelo.png";
            device->DrawImage2D(posXMiniMap + (posXTMM - 15), posYMiniMap + (posYTMM - 15), 1.0f, 0.1f, sprite, true);
        }
    }
    totemCatch = false;

    auto widthMTime = 385.0f;
    // MARCADOR GLOBAL
    if (globalClock) {
        cadena = "media/indicator_tiempo.png";
        device->DrawImage2D(w-(widthMTime*0.5f)-50.0f, 50.0f ,0.5f , 0.2f, cadena, true);
        
        auto cGClock = static_cast<CClock*>(globalClock->GetComponent(CompType::ClockComp).get());
        int time = cGClock->DURATION_TIME/1000 - cGClock->accumulatedTime/1000;
        int min = time/60;
        int seg = time - min*60;
        
        cadena = std::to_string(min) + ":" + std::to_string(seg);
        if (seg < 10) { cadena = std::to_string(min) + ":0" + std::to_string(seg); }
        if (min < 10) { cadena = "0" + cadena; }

        glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
        if(min == 0 && seg <= 30) {
            color = glm::vec3(255.0f, 0.0f, 0.0f);
        }
        device->RenderText2D(cadena, (w - 165.0f), (h - 100.0f), 0.05f, 0.6f, color);
    }
   
    
    //EVENTS
    if (manHud && manHud->IsEventHUDActive()) {
        auto& eventhud = manHud->GetEventShow();
        auto cEventHUD = static_cast<CEventHUD*>(eventhud.get()->GetComponent(CompType::EventHudComp).get());
        if (cEventHUD) {
            switch(cEventHUD->characterEventHUD) {
                case (uint16_t)mainCharacter::PENGUIN:  cadena = "media/hudPenguin.png";        break;
                case (uint16_t)mainCharacter::TIGER:    cadena = "media/hudTiger.png";          break;
                case (uint16_t)mainCharacter::SHARK:    cadena = "media/hudShark.png";          break;
                case (uint16_t)mainCharacter::GORILLA:  cadena = "media/hudGorilla.png";        break;
                case (uint16_t)mainCharacter::DRAGON:   cadena = "media/hudDragon.png";         break;
                case (uint16_t)mainCharacter::OCTOPUS:  cadena = "media/hudOctopus.png";        break;
                default: /*cout << "+++++++ No entiendo este personaje para el evento" << endl;*/   break;
            }
            device->DrawImage2D(w/2 - 175.0f, 170.0f, 0.7f, 0.1f, cadena, true);
            cadena = cEventHUD->spriteTypeEvent;
            device->DrawImage2D(w/2 - 100.0f, 170.0f, 0.7f, 0.1f, cadena, true);
        }
    }

    device->SetEnableDepthTest(true);

}

void RenderFacadeClover::FacadeDrawIntro() {
    if(!introAnimation){
        introAnimation = make_unique<Animation2D>("media/introAnimation/Beast Brawl.jpg",1029,60);
        introAnimation->Start();
    }
    
    resourceManager->DeleteResourceTexture(introAnimation->GetCurrentPath());
    
    introAnimation->Update();
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, introAnimation->GetCurrentPath(), true);
    
    //No podemos hacer animaciones a otra cosa que no sea 60 porque el vsync tiene que estar activado
    if(introAnimation->GetFinished()){
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }
    //device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, "media/introAnimation/Beast Brawl355.jpg", true);

}

void RenderFacadeClover::FacadeDrawMenu() {

    std::string file = "media/menu/mainmenu/main_menu.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    file = "media/menu/mainmenu/elements_menu.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.8f, file, true);

    std::string files[7] = {
        "media/menu/mainmenu/partidarapida_hover.png",
        "media/menu/mainmenu/torneo_hover.png",
        "media/menu/mainmenu/multijugador_hover.png",
        "media/menu/mainmenu/controles_hover.png",
        "media/menu/mainmenu/creditos_hover.png",
        "media/menu/mainmenu/ajustes_hover.png",
        "media/menu/mainmenu/salir_hover.png"
    };

    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.7f, files[inputMenu], true);

}

void RenderFacadeClover::FacadeDrawSelectCharacter() {
    auto nodeSelected = device->GetNodeByID(inputSC);

    if(previousInputSC != inputSC) {
        previousInputSC = inputSC;
        static_cast<CLMesh*>(nodeSelected->GetEntity())->ResetAnimation();
    }

    //Ponemos visible solamente el que esta seleccionado
    for(uint8_t i=0; i<=maxInputSC ; i++){
        auto node = device->GetNodeByID(i);
        node->SetVisible(false);
    }

    nodeSelected->SetVisible(true);
    static_cast<CLMesh*>(nodeSelected->GetEntity())->Animate();

    device->SetEnableDepthTest(false);

    std::string file = "media/menu/character/character_selector.png";
    if (!gamepadConnected) { file = "media/menu/character/character_selector_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    std::string files[6] = {
        "media/menu/character/penguin_selected.png",
        "media/menu/character/baxter_selected.png",
        "media/menu/character/sharky_selected.png",
        "media/menu/character/kong_selected.png",
        "media/menu/character/deacon_selected.png",
        "media/menu/character/octopus_selected.png"
    };
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.8f, files[inputSC], true);


    vector<uint8_t> catched = GameValues::GetInstance()->GetCharacterSel();
    for (uint8_t c : catched ) {
        switch (c) {
            case static_cast<uint8_t>(mainCharacter::PENGUIN):  file = "media/LobbyOnline/penguin_catched.png";    break;
            case static_cast<uint8_t>(mainCharacter::TIGER):    file = "media/LobbyOnline/tiger_catched.png";      break;
            case static_cast<uint8_t>(mainCharacter::SHARK):    file = "media/LobbyOnline/sharky_catched.png";    break;
            case static_cast<uint8_t>(mainCharacter::GORILLA):  file = "media/LobbyOnline/gorila_catched.png";    break;
            case static_cast<uint8_t>(mainCharacter::DRAGON):   file = "media/LobbyOnline/dragon_catched.png";    break;
            case static_cast<uint8_t>(mainCharacter::OCTOPUS):  file = "media/LobbyOnline/octopus_catched.png";    break;
            default: file = "media/LobbyOnline/penguin_catched.png"; break;
        }
        device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.7f, file, true);
    }
        
    device->SetEnableDepthTest(true);

}

void RenderFacadeClover::FacadeDrawGameOptions() {

    std::string name = "";
    auto w = device->GetScreenWidth();
    auto h = device->GetScreenHeight();

    std::string file = "media/menu/gameoptions/settings_match_bg.png";
    if (!gamepadConnected) { file = "media/menu/gameoptions/settings_match_bg_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.9f, file, true);
    
    std::string files[4] = {
        "media/menu/gameoptions/duration_match_",
        "media/menu/gameoptions/duration_totem_",
        "media/menu/gameoptions/players_",
        "media/menu/gameoptions/difficulty_"
    };

    std::string op1[4] = {"2min", "3min", "4min", "5min" };
    std::string op2[3] = {"30s", "45s", "1min"};

    file = files[0] + op1[inputGO[0]];
    if (option == 0) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.8f, file, true);
    file = files[1] + op2[inputGO[1]];
    if (option == 1) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.7f, file, true);

    std::string op3[4] = { "3", "4", "5", "6"};
    std::string op4[3] = { "easy", "normal", "hard" };

    file = files[2] + op3[inputGO[2]];
    if (option == 2 ) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.6f, file, true );

    file = files[3] + op4[inputGO[3]];
    if (option == 3 ) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.5f, file, true );

}

void RenderFacadeClover::FacadeDrawTournamentOptions() {
    
    std::string name = "";
    auto w = device->GetScreenWidth();
    auto h = device->GetScreenHeight();

    std::string file = "media/menu/gameoptions/settings_match_bg.png";
    if (!gamepadConnected) { file = "media/menu/gameoptions/settings_match_bg_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.9f, file, true);
    
    std::string files[5] = {
        "media/menu/gameoptions/tournament/t_duration_match_",
        "media/menu/gameoptions/tournament/t_duration_totem_",
        "media/menu/gameoptions/tournament/t_players_",
        "media/menu/gameoptions/tournament/t_difficulty_",
        "media/menu/gameoptions/tournament/t_matches_"
    };

    std::string op1[4] = {"2min", "3min", "4min", "5min" };
    std::string op2[3] = {"30s", "45s", "1min"};
    std::string op3[4] = { "3", "4", "5", "6"};
    std::string op4[3] = { "easy", "normal", "hard" };
    std::string op5[3] = {"3", "4", "5"};

    file = files[0] + op1[inputTO[0]];
    if (optionTO == 0) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.8f, file, true);

    file = files[1] + op2[inputTO[1]];
    if (optionTO == 1) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.7f, file, true);

    file = files[2] + op3[inputTO[2]];
    if (optionTO == 2 ) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.6f, file, true );
    
    file = files[3] + op4[inputTO[3]];
    if (optionTO == 3 ) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.5f, file, true );

    file = files[4] + op5[inputTO[4]];
    if (optionTO == 4) { file += "_hover"; }
    file += ".png";
    device->DrawImage2D(0.0f, 0.0f, w, h, 0.4f, file, true);


    //file = files[0] + op1[inputTO[0]];
    //if (optionTO == 0) { file += "_hover"; }
    //file += ".png";
    //device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.8f, file, true);
    
}

void RenderFacadeClover::FacadeDrawControler() {
    std::string file = "media/menu/controls/controls_xbox.png";
    if ( !gamepadConnected ) { file = "media/menu/controls/controls_pc.png"; }
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

void RenderFacadeClover::FacadeDrawPause() {
    std::string file = "media/menu/pause/pause_menu.png";
    if (!gamepadConnected) { file = "media/menu/pause/pause_menu_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    file = "media/menu/pause/pause_elements.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.8f, file, true);

    std::string files[2] = {
        "media/menu/pause/pause_continue_hover.png",
        "media/menu/pause/pause_exit_hover.png"
    };

    Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.7f, files[inputPause], true);
}

void RenderFacadeClover::FacadeDrawEndRace() {

    device->SetEnableDepthTest(false);

    auto w = device->GetScreenWidth();
    auto h = device->GetScreenHeight();
    std::string name = "Mr Penguin";

    auto scale = 0.75f;
    if (h > 1000) { scale = 1.0f; }
    else if (h < 475 ) { scale = 0.25; }
    else if (h < 675) { scale = 0.5; }

    std::string file = "media/menu/endrace/finish_menu_bg.png";
    if ( !gamepadConnected ) { file = "media/menu/endrace/finish_menu_bg_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    file = "media/menu/endrace/finish_title.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.85f, file, true);


    auto rank = GameValues::GetInstance()->GetRanking();
    auto secondsRank = GameValues::GetInstance()->GetSeconds();
    uint8_t i = 1;
    auto posX = w/2 - 541*scale;
    auto posY = h/2 - rank.size()*50.0f*scale;
    auto posYText = h/2 + rank.size()*50.0f*scale - 65.0f*scale;
    auto antTime = -1;
    auto positionPoints = 0;

    file = "media/menu/endrace/finish_menu_time.png";
    device->DrawImage2D(w/2 + 415.0f*scale, posY, 0.7f*scale, 0.8f, file, true);
    
    for(auto it = rank.begin(); it != rank.end(); ++it) {

        if(i>numShowPanel) { break; }

        auto itS = secondsRank.find(it->first);
        if ( itS != secondsRank.end()) {
            auto actualTime = itS->second;
            if (antTime != -1 && antTime != actualTime) { positionPoints++; }
            antTime = actualTime;
        }

        file = "media/menu/endrace/position";
        file += std::to_string(positionPoints+1) + ".png";
        device->DrawImage2D(posX, posY + (i*100.0f)*scale, 1.0*scale, 0.7f, file, true);
        switch (it->second) {
            case (uint16_t)mainCharacter::PENGUIN:  file = "media/hudPenguin.png";      name = "Mr Penguin";            break;
            case (uint16_t)mainCharacter::TIGER:    file = "media/hudTiger.png";        name = "Mrs Baxter";            break;
            case (uint16_t)mainCharacter::SHARK:    file = "media/hudShark.png";        name = "Captain Sharky";        break;
            case (uint16_t)mainCharacter::GORILLA:  file = "media/hudGorilla.png";      name = "Kaiser Kong";           break;
            case (uint16_t)mainCharacter::DRAGON:   file = "media/hudDragon.png";       name = "Deacon Dragon";         break;
            case (uint16_t)mainCharacter::OCTOPUS:  file = "media/hudOctopus.png";      name = "Ciber Octopus";         break;
            default: /*cout << "+++++++ No entiendo este personaje para el evento" << endl;*/   break;
        }
        device->DrawImage2D(posX + 190.0f*scale, posY + (i*100.0f)*scale + 5.0f*scale, 1.0f*scale, 0.6f, file, true);  //CARITA
        device->RenderText2D(name, posX + 275.0f*scale, posYText - (i*100.0f)*scale, 0.5f, 1.0*scale, glm::vec3(255.0f, 255.0f, 255.0f));

        
        if((positionPoints+1) == 1) {
            file = "media/menu/endrace/crown.png";
            device->DrawImage2D(posX - 50.0f*scale, posY + (i*100.0f)*scale - 65.0f*scale, 0.9*scale, 0.6f, file, true);
        }

        auto it2 = secondsRank.find(it->first);
        if (it2 != secondsRank.end()){
            if (it2->second < 10) { file = "0" + std::to_string(it2->second); }
            else { file = std::to_string(it2->second); }
            device->RenderText2D(file, posX + 950.0f*scale, posYText - (i*100.0f)*scale, 0.4f, 1.25*scale, glm::vec3(255.0f,255.0f,255.0f));
        }

        ++i;
    }
    if(duration_cast<milliseconds>(system_clock::now() - timeAnimationEnd).count() > msChange && numShowPanel<=GameValues::GetInstance()->GetNumPlayers()){
        timeAnimationEnd = system_clock::now();
        numShowPanel++;
    }

    if (menuER) {
        file = "media/menu/endrace/finish_menu_options_bg.png";
        Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.4f, file, true);
        
        if ( !multiplayer ) {
            file = "media/menu/endrace/finish_menu_options_elements.png";
            Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.5f, file, true);
            std::string files[3] = {
                "media/menu/endrace/finish_option1_hover.png",
                "media/menu/endrace/finish_option2_hover.png",
                "media/menu/endrace/finish_option3_hover.png"
            };
            Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.2f, files[inputER], true);
        } else {
            file = "media/menu/endrace/online/o_finish_menu_options_elements.png";
            Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.5f, file, true);
            std::string files[2] = {
                "media/menu/endrace/online/o_finish_option1_hover.png",
                "media/menu/endrace/online/o_finish_option2_hover.png"
            };
            Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.2f, files[inputER], true);
        }
    }
    device->SetEnableDepthTest(true);
}

void RenderFacadeClover::FacadeDrawEndTournament() {

    auto w = device->GetScreenWidth();
    auto h = device->GetScreenHeight();

    auto scale = 0.75f;
    if (h > 1000) { scale = 1.0f; }
    else if (h < 475 ) { scale = 0.25; }
    else if (h < 675) { scale = 0.5; }

    std::string name = "Mr Penguin";
    std::string file = "media/menu/endrace/finish_menu_bg.png";
    if ( !gamepadConnected ) { file = "media/menu/endrace/finish_menu_bg_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    auto rank = GameValues::GetInstance()->GetRanking();
    auto secondsRank = GameValues::GetInstance()->GetSeconds();
    auto posPoints = GameValues::GetInstance()->GetPosPoints();
    uint8_t i = 1;
    auto posX = w/2 - 541*scale;
    auto posY = h/2 - rank.size()*50.0f*scale;
    auto posYText = h/2 + rank.size()*50.0f*scale - 65.0f*scale;

    if(menuET == 0){
        // numCarreras
        file = "media/menu/endrace/tournament/t_ncarrers.png";
        device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.85f, file, true);
        file = std::to_string(GameValues::GetInstance()->GetActualBattle()) + "/" + std::to_string(GameValues::GetInstance()->GetNumBattles());
        device->RenderText2D(file, w - 225.0f*scale, h - 125.0f*scale, 0.8f, 1.0f, glm::vec3(0.0f,0.0f,0.0f));

        file = "media/menu/endrace/finish_menu_time.png";
        device->DrawImage2D(w/2 + 415.0f*scale, posY, 0.7f*scale, 0.7f, file, true);
        file = "media/menu/endrace/tournament/pts.png";
        device->DrawImage2D(w/2 + 225.0f*scale, posY + 20.0f*scale, 1.0f*scale, 0.65f, file, true);

        auto antTime = -1;
        auto positionPoints = 0;
        for(auto it = rank.begin(); it != rank.end(); ++it) {

            if(i>numShowPanel) { break; }

            auto itS = secondsRank.find(it->first);
            if ( itS != secondsRank.end()) {
                auto actualTime = itS->second;
                if (antTime != -1 && antTime != actualTime) { positionPoints++; }
                antTime = actualTime;
            }

            file = "media/menu/endrace/position";
            file += std::to_string(positionPoints+1) + ".png";
            device->DrawImage2D(posX, posY + (i*100.0f)*scale, 1.0*scale, 0.8f, file, true);
            switch (it->second) {
                case (uint16_t)mainCharacter::PENGUIN:  file = "media/hudPenguin.png";      name = "Mr Penguin";            break;
                case (uint16_t)mainCharacter::TIGER:    file = "media/hudTiger.png";        name = "Mrs Baxter";            break;
                case (uint16_t)mainCharacter::SHARK:    file = "media/hudShark.png";        name = "Captain Sharky";        break;
                case (uint16_t)mainCharacter::GORILLA:  file = "media/hudGorilla.png";      name = "Kaiser Kong";           break;
                case (uint16_t)mainCharacter::DRAGON:   file = "media/hudDragon.png";       name = "Deacon Dragon";         break;
                case (uint16_t)mainCharacter::OCTOPUS:  file = "media/hudOctopus.png";      name = "Ciber Octopus";         break;
                default: /*cout << "+++++++ No entiendo este personaje para el evento" << endl;*/   break;
            }
            device->DrawImage2D(posX + 190.0f*scale, posY + (i*100.0f)*scale + 5.0f*scale, 1.0f*scale, 0.7f, file, true);  //CARITA
            device->RenderText2D(name, posX + 275.0f*scale, posYText - (i*100.0f)*scale, 0.65f, 1.0*scale, glm::vec3(255.0f, 255.0f, 255.0f));

            // puntos

            int points = posPoints[/*it->first-1*/positionPoints];
            if (points < 10) { file = "+0" + std::to_string(points); }
            else { file = "+" + std::to_string(points); }
            device->RenderText2D(file, posX + 750.0f*scale, posYText - (i*100.0f)*scale, 0.5f, 1.25*scale, glm::vec3(255.0f,255.0f,255.0f));

            // tiempo
            auto it2 = secondsRank.find(it->first);
            if (it2 != secondsRank.end()){
                if (it2->second < 10) { file = "0" + std::to_string(it2->second); }
                else { file = std::to_string(it2->second); }
                device->RenderText2D(file, posX + 950.0f*scale, posYText - (i*100.0f)*scale, 0.3f, 1.25*scale, glm::vec3(255.0f,255.0f,255.0f));
            }
            ++i;
        }
        if(duration_cast<milliseconds>(system_clock::now() - timeAnimationEnd).count() > msChange && numShowPanel<=GameValues::GetInstance()->GetNumPlayers()){
            timeAnimationEnd = system_clock::now();
            numShowPanel++;
        }
    } else if(menuET==1) {

        // numCarreras
        file = "media/menu/endrace/tournament/t_ncarrers.png";
        device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.85f, file, true);
        file = std::to_string(GameValues::GetInstance()->GetActualBattle()) + "/" + std::to_string(GameValues::GetInstance()->GetNumBattles());
        device->RenderText2D(file, w - 225.0f*scale, h - 125.0f*scale, 0.8f, 1.0f, glm::vec3(0.0f,0.0f,0.0f));

        file = "media/menu/endrace/tournament/pts_total.png";
        device->DrawImage2D(w/2 + 395.0f*scale, posY + 30.0f*scale, 0.7f*scale, 0.7f, file, true);

        // Puntos totales
        auto rankPoints = GameValues::GetInstance()->GetRankingPoints();
        std::string file = "media/menu/endrace/finish_menu_bg.png";
        device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

        if( GameValues::GetInstance()->GetNumBattles() == GameValues::GetInstance()->GetActualBattle() ) {
            file = "media/menu/endrace/finish_title.png";
            device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.85f, file, true);
        }

        auto antPoints = -1;
        auto positionPoints = 0;
        //cout << "-----------------------" << endl;
        for(auto it = rankPoints.begin(); it != rankPoints.end(); ++it) {
            if(i>numShowPanel) { break; }

            //cout << "ANTPOINT: " << antPoints << endl;
            auto actualPoints = it->first;
            //cout << "ACTUAL POINTS: " << actualPoints << endl;
            if (antPoints != -1 && antPoints != actualPoints) { positionPoints++; }
            antPoints = actualPoints;
            //cout << "POSICION: " << positionPoints+1 << endl;
            
            file = "media/menu/endrace/position";
            file += std::to_string(positionPoints+1) + ".png";
            device->DrawImage2D(posX, posY + (i*100.0f)*scale, 1.0*scale, 0.8f, file, true);
            switch (it->second) {
                case (uint16_t)mainCharacter::PENGUIN:  file = "media/hudPenguin.png";      name = "Mr Penguin";            break;
                case (uint16_t)mainCharacter::TIGER:    file = "media/hudTiger.png";        name = "Mrs Baxter";            break;
                case (uint16_t)mainCharacter::SHARK:    file = "media/hudShark.png";        name = "Captain Sharky";        break;
                case (uint16_t)mainCharacter::GORILLA:  file = "media/hudGorilla.png";      name = "Kaiser Kong";           break;
                case (uint16_t)mainCharacter::DRAGON:   file = "media/hudDragon.png";       name = "Deacon Dragon";         break;
                case (uint16_t)mainCharacter::OCTOPUS:  file = "media/hudOctopus.png";      name = "Ciber Octopus";         break;
                default: /*cout << "+++++++ No entiendo este personaje para el evento" << endl;*/   break;
            }
            device->DrawImage2D(posX + 190.0f*scale, posY + (i*100.0f)*scale + 5.0f*scale, 1.0f*scale, 0.7f, file, true);  //CARITA
            device->RenderText2D(name, posX + 275.0f*scale, posYText - (i*100.0f)*scale, 0.65f, 1.0*scale, glm::vec3(255.0f, 255.0f, 255.0f));

            if((positionPoints+1) == 1) {
                file = "media/menu/endrace/crown.png";
                device->DrawImage2D(posX - 50.0f*scale, posY + (i*100.0f)*scale - 65.0f*scale, 0.9*scale, 0.6f, file, true);
            }

            // puntos
            if (it->first < 10) { file = "0" + std::to_string(it->first); }
            else { file = std::to_string(it->first); }
            device->RenderText2D(file, posX + 950.0f*scale, posYText - (i*100.0f)*scale, 0.3f, 1.25*scale, glm::vec3(255.0f,255.0f,255.0f));
            ++i;
        }
        if(duration_cast<milliseconds>(system_clock::now() - timeAnimationEnd).count() > msChange && numShowPanel<=GameValues::GetInstance()->GetNumPlayers()){
            timeAnimationEnd = system_clock::now();
            numShowPanel++;
        }

    }else if(menuET == 2){
        file = "media/menu/endrace/finish_menu_options_bg.png";
        device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.3f, file, true);

        if( GameValues::GetInstance()->GetNumBattles() != GameValues::GetInstance()->GetActualBattle() ) {
            file = "media/menu/endrace/tournament/t_finish_menu_options_elements.png";
            device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.2f, file, true);
            
            std::string files[2] = {
                "media/menu/endrace/tournament/t_finish_option1_hover.png",
                "media/menu/endrace/tournament/t_finish_option2_hover.png"
            };
            device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, files[inputET], true);
        } else {
            file = "media/menu/endrace/finish_menu_options_elements.png";
            device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.2f, file, true);
            std::string files[3] = {
                "media/menu/endrace/finish_option1_hover.png",
                "media/menu/endrace/finish_option2_hover.png",
                "media/menu/endrace/finish_option3_hover.png"
            };
            device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, files[inputET], true);
        }
    }
}

void RenderFacadeClover::FacadeDrawCredits() {
    resourceManager->DeleteResourceTexture(creditsAnimation->GetCurrentPath());
    
    creditsAnimation->Update();
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, creditsAnimation->GetCurrentPath(), true);
    
    //No podemos hacer animaciones a otra cosa que no sea 60 porque el vsync tiene que estar activado
    if(creditsAnimation->GetFinished()){
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    }
}

void RenderFacadeClover::FacadeDrawLobbyMultiConnecting() {
    std::string file = "media/LobbyOnline/online_main.png";
    if ( !gamepadConnected ) { file = "media/LobbyOnline/online_main_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

void RenderFacadeClover::FacadeDrawLobbyMultiWait() {
    std::string file = "media/LobbyOnline/online_waiting.png";
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

void RenderFacadeClover::FacadeDrawLobbyMultiSelChar() {
    FacadeDrawSelectCharacter();
}

void RenderFacadeClover::FacadeDrawLobbyMultiExit() {
    std::string file = "media/LobbyOnline/online_disconect.png";
    if ( !gamepadConnected ) { file = "media/LobbyOnline/online_disconect_keyboard.png"; }
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.1f, file, true);
}

void RenderFacadeClover::FacadeDrawSettings() {

    device->SetEnableDepthTest(false);

    std::string file = "media/menu/settings/settings_bg.png";
    if ( !gamepadConnected ) { file = "media/menu/settings/settings_bg_keyboard.png";}
    device->DrawImage2D(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    std::string files[5] = {
        "media/menu/settings/game_sfx_",
        "media/menu/settings/game_music_",
        "media/menu/settings/particles_",
        "media/menu/settings/vegetation_",
        "media/menu/settings/shadows_"
    };

    std::string op1[4] = { "low", "med2", "med1", "high" };
    std::string op2[2] = { "desactivated", "activated" };

    file = files[0] + op1[inputSettings[0]];
    if (optionSettings == 0) { file += "_hover"; };
    file += ".png";
    Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    file = files[1] + op1[inputSettings[1]];
    if (optionSettings == 1) { file += "_hover"; };
    file += ".png";
    Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    file = files[2] + op2[inputSettings[2]];
    if (optionSettings == 2) { file += "_hover"; };
    file += ".png";
    Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    file = files[3] + op2[inputSettings[3]];
    if (optionSettings == 3) { file += "_hover"; };
    file += ".png";
    Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    file = files[4] + op2[inputSettings[4]];
    if (optionSettings == 4) { file += "_hover"; };
    file += ".png";
    Draw2DImage(0.0f, 0.0f, device->GetScreenWidth(), device->GetScreenHeight(), 0.9f, file, true);

    device->SetEnableDepthTest(true);

}




/**
 * Limpia la pantalla y vacia los buffers de pantalla
 */
void RenderFacadeClover::FacadeBeginScene() const{
    device->BeginScene();
}

/**
 * Dibuja los objetos del arbol
 */
void RenderFacadeClover::FacadeDrawAll() const{
    device->DrawObjects();
}

/**
 * Acaba la escena, intercambiando buffers y liberando los eventos de glfw de teclado
 */
void RenderFacadeClover::FacadeEndScene() const{
    device->PollEvents();
    device->EndScene();
}

void RenderFacadeClover::FacadeDeviceDrop() {

}

void RenderFacadeClover::FacadeAddSkybox(string right,string left,string top,string bottom,string front,string back){
    device->AddSkybox(right, left, top, bottom, front, back);
}

void RenderFacadeClover::FacadeAddShadowMapping(unsigned int lightId){
    device->AddShadowMapping(lightId);
}

void RenderFacadeClover::FacadeAddGrass(float _width, float _height, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass){
    device->AddGrass(_width, _height, _position, _scale, realistGrass);
}


void RenderFacadeClover::FacadeAddGrass(float radious, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass){
    device->AddGrass(radious, _position, _scale, realistGrass);  
}



//DEBUG dibuja las aristas entre los nodos del grafo
void RenderFacadeClover::FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const{
    if (!showDebug) return;  //Si no esta activado debug retornamos

    //Recorremos todos los WayPoints del manager
    for (const auto& way : manWayPoints->GetEntities()) {
        auto cWayPoint = static_cast<CWayPoint*>(way->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdge = static_cast<CWayPointEdges*>(way->GetComponent(CompType::WayPointEdgesComp).get());

        //Vamos a dibujar varias lineas para formar un "circulo"
        auto centre = cWayPoint->position;
        //La primera posicion es para el primer cuadrante angle 0
        auto radious = cWayPoint->radious;
        glm::vec3 lastPoint = glm::vec3(centre.x + radious,centre.y, centre.z);
        float angle = 0;
        float angleIncrement = 18; //Si quieres aumentar la precision debes bajar el numero y que siga siendo multiplo de 360

        while(angle<=360){
            angle += angleIncrement;
            float radians = (angle*3.1415) / 180.0;
            auto newPoint = glm::vec3(centre.x + (cos(radians) * radious), centre.y, centre.z + (sin(radians) * radious));

            Draw3DLine(lastPoint,newPoint);
            lastPoint = newPoint;
        }
        //Recorremos el componente CWayPointEdges->edges para ir arista a arista
        if(cWayPointEdge){
            for (Edge e : cWayPointEdge->edges) {
                //Cogemos la posicion de la arista que apunta e->to
                auto cWayPoint2 = static_cast<CWayPoint*>(manWayPoints->GetEntities()[e.to]->GetComponent(CompType::WayPointComp).get());

                //Usamos un color u otro en funcion de la distancia
                if (e.cost < 300) {
                    Draw3DLine(cWayPoint->position, cWayPoint2->position, 0.0,0.0,255.0);
                } else if (e.cost >= 300 && e.cost < 500) {
                    Draw3DLine(cWayPoint->position, cWayPoint2->position,0.0,255.0,0.0);
                } else if (e.cost >= 500) {
                    Draw3DLine(cWayPoint->position, cWayPoint2->position, 255.0,0.0,0.0);
                }
            }
        }//else{
           // cout << "1111111111111111111111111111111111111111111111111:  " << cWayPoint->id << endl; 
        //}
    }
}

void RenderFacadeClover::FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const{
    if(!showAIDebug) return;


    /*
		   /7--------/6
		  / |       / |
		 /  |      /  |
		4---------5   |
		|  /3- - -|- -2
		| /       |  /
		|/        | /
		0---------1/

        width  = Z
        depth  = X
        height = Y

	*/

    glm::vec3 point0;
    glm::vec3 point1;
    glm::vec3 point2;
    glm::vec3 point3;
    glm::vec3 point4;
    glm::vec3 point5;
    glm::vec3 point6;
    glm::vec3 point7;
    //Dibujamos el cuadrado que engloba a cada NavMesh

    for(const auto& navMesh : manNavMesh->GetEntities()){
        auto cTransformable = static_cast<CTransformable*>(navMesh->GetComponent(CompType::TransformableComp).get());
        auto cDimensions    = static_cast<CDimensions*>(navMesh->GetComponent(CompType::DimensionsComp).get());

        point0 = glm::vec3(cTransformable->position.x - (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z - (cDimensions->depth/2));

        point1 = glm::vec3(cTransformable->position.x - (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z + (cDimensions->depth/2));

        point2 = glm::vec3(cTransformable->position.x + (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z + (cDimensions->depth/2));
                            
        point3 = glm::vec3(cTransformable->position.x + (cDimensions->width/2),
                            cTransformable->position.y - (cDimensions->height/2),
                            cTransformable->position.z - (cDimensions->depth/2));

        point4 = glm::vec3(point0.x,point0.y + cDimensions->height, point0.z);
        point5 = glm::vec3(point1.x,point1.y + cDimensions->height, point1.z);
        point6 = glm::vec3(point2.x,point2.y + cDimensions->height, point2.z);
        point7 = glm::vec3(point3.x,point3.y + cDimensions->height, point3.z);

        Draw3DLine(point0,point1,0,0,0);
        Draw3DLine(point1,point2,0,0,0);
        Draw3DLine(point2,point3,0,0,0);
        Draw3DLine(point3,point0,0,0,0);
        Draw3DLine(point4,point0,0,0,0);
        Draw3DLine(point4,point5,0,0,0);
        Draw3DLine(point4,point7,0,0,0);
        Draw3DLine(point5,point1,0,0,0);
        Draw3DLine(point5,point6,0,0,0);
        Draw3DLine(point6,point2,0,0,0);
        Draw3DLine(point6,point7,0,0,0);
        Draw3DLine(point7,point3,0,0,0);

    }


    //Ahora vamos a hacer el debug desde nuestra posicion al CPosDestination
    if(idCarAIToDebug==-1){
        //Significa que lo hacemos para todos los coches
        for(const auto& carAI : manCars->GetEntities()){
            if (static_cast<Car*>(carAI.get())->GetTypeCar() == TypeCar::CarAI){
                auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
                auto cTransformableCar = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());

                Draw3DLine(cPosDestination->position,cTransformableCar->position);
                //Ahora vamos a dibujar su CPath
                FacadeDrawAIDebugPath(carAI.get(),manWayPoint);

                //Ahora por ultimo en la esquina superior derecha escribimos strings con datos
            }
        }
    }else{
        //Si tenemos seleccionado un coche en concreto dibujamos solo el suyo
        
        auto carAI = manCars->GetEntities()[idCarAIToDebug+1]; //Cogemos el coche que vamos a debugear
        if (static_cast<Car*>(carAI.get())->GetTypeCar() == TypeCar::CarAI){
            auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
            auto cTransformableCar = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
            auto cDimensions = static_cast<CDimensions*>(carAI->GetComponent(CompType::DimensionsComp).get());
            auto cCurrentNavMesh = static_cast<CCurrentNavMesh*>(carAI->GetComponent(CompType::CurrentNavMeshComp).get());
            // auto CBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());

            Draw3DLine(cPosDestination->position,cTransformableCar->position);
            //Ahora vamos a dibujar su CPath
            FacadeDrawAIDebugPath(carAI.get(),manWayPoint);

            int yPos = 650;
            int yIncrement = -25;
            //Ahora por ultimo en la esquina superior derecha escribimos strings con datos
            auto cCar = static_cast<CCar*>(carAI->GetComponent(CompType::CarComp).get());
            string text = "Pos - Rot - Scale:(" +
                                        to_string(cTransformableCar->position.x) + " | " +
                                        to_string(cTransformableCar->position.y) + " | " +
                                        to_string(cTransformableCar->position.z) + ")";

            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "(" +    to_string(cTransformableCar->rotation.x) + " | " +
                            to_string(cTransformableCar->rotation.y) + " | " +
                            to_string(cTransformableCar->rotation.z) + ")";
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "(" +    to_string(cTransformableCar->scale.x) + " | " +
                            to_string(cTransformableCar->scale.y) + " | " +
                            to_string(cTransformableCar->scale.z) + ")"; 
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "Dimensions: " + to_string(cDimensions->width) + " | " + to_string(cDimensions->height) + " | " + to_string(cDimensions->depth); 
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "Speed: " + to_string(cCar->speed);
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "Destination: " + to_string(cPosDestination->position.x) + " | " + to_string(cPosDestination->position.y) + " | " + to_string(cPosDestination->position.z);
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());
            auto cPathAux = stack<int>(cBrainAI->stackPath);
            text = "Path: ";
            while(!cPathAux.empty()){
                auto idWaypoint = cPathAux.top();
                text += to_string(idWaypoint) + " - ";
                cPathAux.pop();
            }
            
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "Current NavMesh: " + to_string(cCurrentNavMesh->currentNavMesh);
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "Target NavMesh: " + to_string(cBrainAI->targetNavMesh);
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            text = "Tipo de IA: " + cBrainAI->movementType;
            device->RenderText2D(text,200,yPos,0.05,0.3,glm::vec3(0.0,0.0,0.0));
            yPos += yIncrement;

            
        }
           
    }

}

void RenderFacadeClover::FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const{
    auto cBrainAI = static_cast<CBrainAI*>(carAI->GetComponent(CompType::BrainAIComp).get());

    auto cPathAux = stack<int>(cBrainAI->stackPath);

    auto lastWaypoint = -1;
    if(!cPathAux.empty()){
        //Hago esto para dibujar la primera linea desde CPosDestination hsata el primer nodo
        auto cPosDestination = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());

        auto idWaypoint = cPathAux.top();
        auto node = manWayPoint->GetEntities()[idWaypoint];
        auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
        Draw3DLine(cPosDestination->position,cWayPoint->position);
        cPathAux.pop();

        //Ahora dibujamos el resto de path si queda
        while(!cPathAux.empty()){
            lastWaypoint = cWayPoint->id; //Me tengo que ir guardando el nodo anterior para dibujar desde ese al proximo
            auto lastNode = manWayPoint->GetEntities()[lastWaypoint];
            auto idWaypoint = cPathAux.top();
            auto node = manWayPoint->GetEntities()[idWaypoint];

            auto cWayPointLast = static_cast<CWayPoint*>(lastNode->GetComponent(CompType::WayPointComp).get());
            auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
            Draw3DLine(cWayPointLast->position,cWayPoint->position);
            cPathAux.pop();
        }
    }
}

void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2) const {
    Draw3DLine(pos1, pos2, 255, 0, 0);
}

void RenderFacadeClover::Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const {
    device->Draw3DLine(pos1.x,pos1.y,-pos1.z, pos2.x,pos2.y,-pos2.z, CLE::CLColor(r,g,b,255.0));
}

void RenderFacadeClover::DeleteEntity(Entity* entity) {
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    device->DeleteNode(cId->id);
}

void RenderFacadeClover::FacadeSetVisibleEntity(Entity* entity, bool visible){
    auto cId = static_cast<CId*>(entity->GetComponent(CompType::IdComp).get());
    auto node = device->GetNodeByID(cId->id);
    node->SetVisible(visible);
}


void RenderFacadeClover::FacadeDrawBoundingPlane(Entity* entity) const {
    if (!showDebug) return;  //Si no esta activado debug retornamos

    CBoundingPlane *plane = static_cast<CBoundingPlane*>(entity->GetComponent(CompType::CompBoundingPlane).get());
    
    vec3 &a = plane->a;
    vec3 &b = plane->b;
    vec3 &c = plane->c;
    vec3 &d = plane->d;
    
    Draw3DLine(a, b);
    Draw3DLine(b, c);
    Draw3DLine(c, d);
    Draw3DLine(d, a);
}

void RenderFacadeClover::FacadeDrawBoundingGround(Entity* entity) const {
    if (!showDebug) return;  //Si no esta activado debug retornamos

    CBoundingPlane *plane = static_cast<CBoundingPlane*>(entity->GetComponent(CompType::CompBoundingPlane).get());
    
    vec3 &a = plane->a;
    vec3 &b = plane->b;
    vec3 &c = plane->c;
    vec3 &d = plane->d;
    
    Draw3DLine(a, b, 0, 0, 0);
    Draw3DLine(b, c, 0, 0, 0);
    Draw3DLine(c, d, 0, 0, 0);
    Draw3DLine(d, a, 0, 0, 0);
}

void RenderFacadeClover::FacadeDrawBoundingOBB(Entity* entity) const {
    if (!showDebug) return;  //Si no esta activado debug retornamos

    CBoundingOBB *obb = static_cast<CBoundingOBB*>(entity->GetComponent(CompType::CompBoundingOBB).get());
    
    for(auto& obbActual : obb->planes){
        vec3 &a = obbActual.get()->a;
        vec3 &b = obbActual.get()->b;
        vec3 &c = obbActual.get()->c;
        vec3 &d = obbActual.get()->d;

        Draw3DLine(a, b, 0, 0, 200);
        Draw3DLine(b, c, 0, 0, 200);
        Draw3DLine(c, d, 0, 0, 200);
        Draw3DLine(d, a, 0, 0, 200);
    }
}

void RenderFacadeClover::FacadeDrawBoundingBox(Entity* entity, bool colliding) const{

}


void RenderFacadeClover::FacadeAddSphereOnObject(Entity* entity){
    
}

void RenderFacadeClover::CleanScene() {
    device->Clear();
}

void RenderFacadeClover::FacadeUpdateViewport(){
    device->UpdateViewport();
}


//////////////////////////
//  BORRANDO TEXTURAS
//////////////////////////

void RenderFacadeClover::FacadeReleaseSelectCharacter(){
    

    CleanScene();

    resourceManager->DeleteResourceTexture("media/menu/character/character_selector.png");
    resourceManager->DeleteResourceTexture("media/menu/character/character_selector_keyboard.png");
    resourceManager->DeleteResourceTexture("media/menu/character/kong_selected.png");
    resourceManager->DeleteResourceTexture("media/menu/character/sharky_selected.png");
    resourceManager->DeleteResourceTexture("media/menu/character/penguin_selected.png");
    resourceManager->DeleteResourceTexture("media/menu/character/baxter_selected.png");
    resourceManager->DeleteResourceTexture("media/menu/character/deacon_selected.png");
    resourceManager->DeleteResourceTexture("media/menu/character/octopus_selected.png");    

    resourceManager->DeleteResourceAnimation("media/animations/penguin/selection/selectionpenguin_000001.obj", 15);
    resourceManager->DeleteResourceAnimation("media/animations/baxter/selection/selectionbaxter_000001.obj", 15);
    resourceManager->DeleteResourceAnimation("media/animations/sharky/selection/selectionsharky_000001.obj", 15);
    resourceManager->DeleteResourceAnimation("media/animations/kong/selection/selectionkong_000001.obj", 15);
    resourceManager->DeleteResourceAnimation("media/animations/dragon/selection/selectiondragon_000001.obj", 15);
    resourceManager->DeleteResourceAnimation("media/animations/cyberoctopus/selection/selectionoctopus_000001.obj", 15);
}

void RenderFacadeClover::FacadeReleaseMeshesInGame(){
    //cout << "Liberadas las mallas ingame" << endl;
    resourceManager->DeleteResourceAnimation("media/animations/sharky/win/victorysharky_000001.obj", 18);
    resourceManager->DeleteResourceAnimation("media/animations/kong/win/victorykong_000001.obj", 18);
    resourceManager->DeleteResourceAnimation("media/animations/cyberoctopus/win/victoryoctopus_000001.obj", 18);
    resourceManager->DeleteResourceAnimation("media/animations/dragon/win/victorydragon_000001.obj", 18);
    resourceManager->DeleteResourceAnimation("media/animations/penguin/win/victorypenguin_000001.obj", 18);
    resourceManager->DeleteResourceAnimation("media/animations/baxter/win/victorybaxter_000001.obj", 18);
    resourceManager->DeleteResourceAnimation("media/animations/penguin/right/turnrightpenguin_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/penguin/left/turnleftpenguin_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/baxter/right/turnrightbaxter_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/baxter/left/turnleftbaxter_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/kong/right/turnrightkong_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/kong/left/turnleftkong_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/sharky/right/turnrightsharky_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/sharky/left/turnleftsharky_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/dragon/right/turnrightdragon_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/dragon/left/turnleftdragon_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/cyberoctopus/right/turnrightoctopus_000001.obj", 10);
    resourceManager->DeleteResourceAnimation("media/animations/cyberoctopus/left/turnleftoctopus_000001.obj", 10);
}

void RenderFacadeClover::FacadeReleaseOptions(){
    
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/settings_match_bg.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/settings_match_bg_keyboard.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_2min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_3min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_4min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_5min_hover.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_2min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_3min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_4min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_match_5min.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_totem_1min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_totem_30s_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_totem_45s_hover.png");
    
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_totem_1min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_totem_30s.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/duration_totem_45s.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_3_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_3.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_4_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_4.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_5_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_5.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_6_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/players_6.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/difficulty_easy_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/difficulty_easy.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/difficulty_hard_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/difficulty_hard.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/difficulty_normal_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/difficulty_normal.png");

}

void RenderFacadeClover::FacadeReleaseSettings(){
    resourceManager->DeleteResourceTexture("media/menu/settings/settings_bg.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/settings_bg_keyboard.png");

    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_high_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_low_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_med1_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_med2_hover.png");

    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_high.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_low.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_med1.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_music_med2.png");

    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_high_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_low_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_med1_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_med2_hover.png");

    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_high.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_low.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_med1.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/game_sfx_med2.png");

    resourceManager->DeleteResourceTexture("media/menu/settings/particles_activated_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/particles_activated.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/particles_desactivated_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/particles_desactivated.png");

    resourceManager->DeleteResourceTexture("media/menu/settings/vegetation_activated_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/vegetation_activated.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/vegetation_desactivated_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/vegetation_desactivated.png");

    resourceManager->DeleteResourceTexture("media/menu/settings/shadows_activated_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/shadows_activated.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/shadows_desactivated_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/settings/shadows_desactivated.png");

}

void RenderFacadeClover::FacadeReleaseStateInGame(){
    //Hud
    resourceManager->DeleteResourceTexture("media/Minimapa240v2.png");
    resourceManager->DeleteResourceTexture("media/hudPenguin.png");
    resourceManager->DeleteResourceTexture("media/hudTiger.png");
    resourceManager->DeleteResourceTexture("media/hudShark.png");
    resourceManager->DeleteResourceTexture("media/hudGorilla.png");
    resourceManager->DeleteResourceTexture("media/hudDragon.png");
    resourceManager->DeleteResourceTexture("media/hudOctopus.png");
    resourceManager->DeleteResourceTexture("media/hudTotem.png");
    resourceManager->DeleteResourceTexture("media/1.png");
    resourceManager->DeleteResourceTexture("media/2.png");
    resourceManager->DeleteResourceTexture("media/3.png");
    resourceManager->DeleteResourceTexture("media/BOctopus.png");
    resourceManager->DeleteResourceTexture("media/BDragon.png");
    resourceManager->DeleteResourceTexture("media/BGorilla.png");
    resourceManager->DeleteResourceTexture("media/BPinguino.png");
    resourceManager->DeleteResourceTexture("media/BTiburon.png");
    resourceManager->DeleteResourceTexture("media/BTigre.png");
    resourceManager->DeleteResourceTexture("media/stoleHUD.png");
    resourceManager->DeleteResourceTexture("media/loseHUD.png");
    resourceManager->DeleteResourceTexture("media/catchHUD.png");
    resourceManager->DeleteResourceTexture("media/nonepowerup.png");
    resourceManager->DeleteResourceTexture("media/robojorobo.png");
    resourceManager->DeleteResourceTexture("media/nitro.png");
    resourceManager->DeleteResourceTexture("media/pudin.png");
    resourceManager->DeleteResourceTexture("media/escudomerluzo.png");
    resourceManager->DeleteResourceTexture("media/telebanana.png");
    resourceManager->DeleteResourceTexture("media/melonmolon.png");
    resourceManager->DeleteResourceTexture("media/totemCogido.png");
    resourceManager->DeleteResourceTexture("media/totemSuelo.png");
    resourceManager->DeleteResourceTexture("media/hudTotemBack.png");
    resourceManager->DeleteResourceTexture("media/hudTotemLeft.png");
    resourceManager->DeleteResourceTexture("media/hudTotemRight.png");
    resourceManager->DeleteResourceTexture("media/ranking.png");
    resourceManager->DeleteResourceTexture("media/indicator_tiempo.png");
    resourceManager->DeleteResourceTexture("media/indicator_totem.png");
    
    //Particulas
    resourceManager->DeleteResourceTexture("media/particleTriangleBlack.png");
    resourceManager->DeleteResourceTexture("media/particleTriangleGreen.png");
    resourceManager->DeleteResourceTexture("media/particleStarBlack.png");
    resourceManager->DeleteResourceTexture("media/particleStarGreen.png");
    resourceManager->DeleteResourceTexture("media/particleYellowTriangle.png");
    resourceManager->DeleteResourceTexture("media/particleYellowStar.png");
    resourceManager->DeleteResourceTexture("media/particleTriangleGrey.png");
    resourceManager->DeleteResourceTexture("media/particleStarMarron.png");
    resourceManager->DeleteResourceTexture("media/particleTriangleBrown.png");
    resourceManager->DeleteResourceTexture("media/particleRedTriangle.png");
    resourceManager->DeleteResourceTexture("media/particleRedStar.png");

}

void RenderFacadeClover::FacadeReleaseMenu(){
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/main_menu.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/main_menu_keyboard.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/elements_menu.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/partidarapida_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/multijugador_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/controles_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/creditos_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/ajustes_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/mainmenu/salir_hover.png");

}

void RenderFacadeClover::FacadeReleaseTournament(){
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/settings_match_bg.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/settings_match_bg_keyboard.png");
    
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_difficulty_easy_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_difficulty_easy.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_difficulty_hard_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_difficulty_hard.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_difficulty_normal_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_difficulty_normal.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_2min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_2min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_3min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_3min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_4min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_4min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_5min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_match_5min.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_totem_1min_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_totem_1min.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_totem_30s_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_totem_30s.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_totem_45s_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_duration_totem_45s.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_matches_3_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_matches_3.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_matches_4_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_matches_4.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_matches_5_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_matches_5.png");

    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_3_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_3.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_4_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_4.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_5_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_5.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_6_hover.png");
    resourceManager->DeleteResourceTexture("media/menu/gameoptions/tournament/t_players_6.png");






}




////////////////////////////////////////////////////////////////////////////
////////////////////////////  CLASE ANIMATION2D  ///////////////////////////
////////////////////////////////////////////////////////////////////////////

RenderFacadeClover::Animation2D::Animation2D(std::string _path, uint16_t _numFrames, uint16_t _fps){
    path = _path;
    numFrames = _numFrames;
    fps = _fps;
    timeBetweenFrames = 1.0/(float)fps;

    std::string delimiter = ".";

    size_t pos = 0;

    string auxPath = _path;
    while ((pos = auxPath.find(delimiter)) != std::string::npos) {

        auxPath.erase(0, pos + delimiter.length());
    }
    extension = "." + auxPath;

    auxPath = _path;
    while ((pos = auxPath.find(delimiter)) != std::string::npos) {

        auxPath.erase(pos, pos + delimiter.length());
    }

    path = auxPath;
}

void RenderFacadeClover::Animation2D::Update(){
    if(finished) return;
    float time = duration_cast<milliseconds>(system_clock::now() - timeStart).count();
    string newPath = path;

    //Cambiamos de frame
    if(time >= timeBetweenFrames*100){
        //Borramos el frame anterior

        //cout << "Nuevo Frame\n";
        
        if(actualFrame >= numFrames){
            finished = true;
            return;
        }else{ 

            int numFramesDigits = to_string(numFrames-1).length();
            int actualFrameDigits = to_string(actualFrame).length();

            int numOfZeros = numFramesDigits - actualFrameDigits;
            for(int i = 0; i<numOfZeros; ++i){
                newPath.append("0");
            }
            newPath.append(to_string(actualFrame));

            
        }

        timeStart = system_clock::now();
        actualFrame++;

    }else{
        //cout << "MISMO Frame\n";

    }

    currentPath = newPath;
}

void RenderFacadeClover::Animation2D::Start(){
    timeStart = system_clock::now();
    started = true;
    finished = false;
}

void RenderFacadeClover::Animation2D::Restart(){
    currentPath = path;
    actualFrame = 0;
    finished    = false;
}

void RenderFacadeClover::SetCamTarget(glm::vec3 pos) {
    auto cameraEntity = static_cast<CLCamera*>(camera1->GetEntity());
    pos.z *= -1; // se invierte pq en opengl este eje se invierte
    cameraEntity->SetCameraTarget(pos);
}

void RenderFacadeClover::Draw2DImage(float x_, float y_, int width_, int height_, float depth_, string file_, bool vertically_) const {
    device->DrawImage2D(x_, y_, width_, height_, depth_, file_, vertically_);
}

std::tuple<int, int> RenderFacadeClover::GetScreenSize() {
    return std::tuple<int, int>(device->GetScreenWidth(), device->GetScreenHeight());
}