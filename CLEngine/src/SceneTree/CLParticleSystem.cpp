#include "CLParticleSystem.h"
#include "CLNode.h"
using namespace CLE;


CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong particlesNumber, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan, std::uint_fast8_t _flags) : CLEntity(idEntity){
    nParticles        = particlesNumber;
    speedDirection    = _speedDirection;
    clTexture         = CLResourceManager::GetResourceManager()->GetResourceTexture(texture,false);
    width             = _width;
    height            = _height;
    spawnDelay        = _spawnDelay;
    nParticlesToSpawn = _nParticlesToSpawn;
    lifeSpan          = _lifeSpan;
    spawnType         = SpawnType::Point;
    flags             = _flags;

    particles.reserve(nParticles);

}

//Line, Square y Cube, depende del valor de _offset
CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,glm::vec3 _offset, glm::vec3 _orientation, std::uint_fast8_t _flags) 
: CLParticleSystem(idEntity,_nParticles,_speedDirection,texture,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan,_flags){

    offset = _offset;
    orientation = _orientation;
    if((offset.x != 0 && !offset.y && !offset.z) || (!offset.x && offset.y != 0 && !offset.z) || (!offset.x && !offset.y && offset.z != 0)){

        //Si solo tiene 1 valor es linea
        spawnType = SpawnType::Line;
        cout << "Soy una linea\n";

    }else if((offset.x != 0 && offset.y != 0 && !offset.z) || (offset.x != 0 && !offset.y && offset.z != 0) || (!offset.x && offset.y != 0 && offset.z != 0)){

        //Si tiene 2 valores es Cuadrado
        spawnType = SpawnType::Square;
        cout << "Soy un cuadrado\n";
    }else if( offset.x != 0 && offset.y != 0 && offset.z != 0){

        //Si tiene 3 valores es Cubo
        spawnType = SpawnType::Cube;
        cout << "Soy un cubo\n";
    }else{
        spawnType = SpawnType::Point;
        cout << "Soy un punto\n";

    }
}

//Circle
CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, glm::vec3 _orientation, std::uint_fast8_t _flags)
: CLParticleSystem(idEntity,_nParticles,_speedDirection,texture,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan,_flags){

    //No lo estoy controlando porque es mi propio motor pero lo suyo es que solo pase 2 ejes en el _orientation
    spawnType = SpawnType::Circle;
    radious = _radious;
    orientation = _orientation;
    cout << "Soy un circulo\n";

}

//Sphere
CLParticleSystem::CLParticleSystem(unsigned int idEntity, ulong _nParticles, glm::vec3 _speedDirection,string texture,uint16_t _width, uint16_t _height,float _spawnDelay,unsigned int _nParticlesToSpawn,float _lifeSpan,float _radious, std::uint_fast8_t _flags)
: CLParticleSystem(idEntity,_nParticles,_speedDirection,texture,_width,_height,_spawnDelay,_nParticlesToSpawn,_lifeSpan,_flags){

    spawnType = SpawnType::Sphere;
    radious = _radious;
    cout << "Soy una esfera\n";

}



void CLParticleSystem::Draw(GLuint shaderID) {
    //En este Draw que llama al resto de draws podriamos settear cosas generales para todos las particulas
    Update(); //Este Update se encarga de cosas como spawnear nuevas particulas

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDepthMask(GL_FALSE);
    glUseProgram(shaderID);

    glm::mat4 viewProjection = node->GetProjectionMatrix()*node->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "VPMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjection));

    glm::vec3 camPos = node->GetActiveCameraNode()->GetGlobalTranslation();
    glUniform3fv(glGetUniformLocation(shaderID, "cameraPosition"), 1, glm::value_ptr(camPos));


    for(auto& particle : particles){
        particle->Draw(shaderID);
    }
}

void CLParticleSystem::Update(){
    //Comprobamos si ha pasado el tiempo de spawn
    auto diff = duration_cast<milliseconds>(system_clock::now() - timeStart).count();

    if(diff > spawnDelay){
        timeStart = system_clock::now();

        for(unsigned int i = 0; i<nParticlesToSpawn && particles.size()+nParticlesToSpawn<=nParticles ; ++i){
            particles.emplace_back(make_shared<CLParticle>(this));
        }

        //Vamos a comprobar si tenemos que revivir alguna particula porque han activado de nuevo el loop
        if(loop){
            int particlesToRespawn = nParticlesToSpawn; //Solo vamos a revivir por iteracion a nParticlesToSpawn
            for(auto& particle : particles){
                if(particle->GetParticleDead()){
                    particle->ReviveParticle();
                    particlesToRespawn--;
                }

                if(particlesToRespawn == 0) break;
            }
        }
    }


}

// -----------------------------------------
// ------------Clase CLParticle-------------
// -----------------------------------------

CLParticleSystem::CLParticle::CLParticle(CLParticleSystem* emitter){
    particleSystem = emitter;   
    position       = CalculateSpawnPosition();
    lifeSpan       = particleSystem->GetLifeSpan();
    velocity       = particleSystem->GetSpeedDirection();
    timeStart      = system_clock::now();

    //Comprobamos si el flag del effecto esta activado
    if(particleSystem->GetFlags() & EFFECT_DIR_ALEATORITY){
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> xDir(-1, 1);
        std::uniform_int_distribution<int> yDir(-1, 1);
        std::uniform_int_distribution<int> zDir(-1, 1);

        int valueX = 0;
        int valueY = 0;
        int valueZ = 0;

        do{
            valueX = xDir(rng);
            valueY = yDir(rng);
            valueZ = zDir(rng);

            xDir(rng);xDir(rng);xDir(rng);
            yDir(rng);yDir(rng);yDir(rng);
            zDir(rng);zDir(rng);zDir(rng);
        }while(!valueX && !valueY && !valueZ);

        velocity.x *= valueX;
        velocity.y *= valueY;
        velocity.z *= valueZ;

        //Reset de numeros aleatorios
        xDir(rng);xDir(rng);xDir(rng);
        yDir(rng);yDir(rng);yDir(rng);
        zDir(rng);zDir(rng);zDir(rng);

    }else{
        velocity = particleSystem->GetSpeedDirection();
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

}


void CLParticleSystem::CLParticle::Draw(GLuint shaderID){
    if(particleDead) return;
    Update();
    
    glUniform1f(glGetUniformLocation(shaderID, "width"), particleSystem->GetWidth());
    glUniform1f(glGetUniformLocation(shaderID, "height"), particleSystem->GetHeight());
    
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, particleSystem->GetTexture()->GetTextureID());
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), &position[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, (void*)0);    
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void CLParticleSystem::CLParticle::Update(){
    //Comprobamos si deberia seguir con vida la particula
    auto diff = duration_cast<milliseconds>(system_clock::now() - timeStart).count();
    if(diff >= lifeSpan){
        if(particleSystem->GetLoop()){
            //la respawneamos en la posicion inicial
            position = CalculateSpawnPosition();
            timeStart = system_clock::now();
        }else{
            //Si no hay loop al llegar a su lifeSpan la desactivamos y reposicionamos
            particleDead = true;
            position = CalculateSpawnPosition();
        }
        
    }else{
        //la movemos
        position.x += velocity.x * Constants::DELTA_TIME;
        position.y += velocity.y * Constants::DELTA_TIME;
        position.z += velocity.z * Constants::DELTA_TIME;
    }
}

glm::vec3 CLParticleSystem::CLParticle::CalculateSpawnPosition(){
    glm::vec3 newPosition(0.0f);
    SpawnType spawnType = particleSystem->GetSpawnType();
    glm::vec3 center = particleSystem->GetCLNode()->GetGlobalTranslation();

    std::random_device rd;
    std::mt19937 rng(rd());


    if(spawnType == SpawnType::Point){
        newPosition = particleSystem->GetCLNode()->GetGlobalTranslation();
    }else if(spawnType == SpawnType::Line || spawnType == SpawnType::Square || spawnType == SpawnType::Cube){
        std::uniform_real_distribution<float> genX(center.x - particleSystem->GetOffset().x, center.x + particleSystem->GetOffset().x);
        std::uniform_real_distribution<float> genY(center.y - particleSystem->GetOffset().y, center.y + particleSystem->GetOffset().y);
        std::uniform_real_distribution<float> genZ(center.z - particleSystem->GetOffset().z, center.z + particleSystem->GetOffset().z);
        float offX = genX(rng);
        float offY = genY(rng);
        float offZ = genZ(rng);

        //Reset de numeros aleatorios
        genX(rng);genX(rng);genX(rng);
        genY(rng);genY(rng);genY(rng);
        genZ(rng);genZ(rng);genZ(rng);


        newPosition = glm::vec3(offX,offY,offZ);
    }else if(spawnType == SpawnType::Sphere || spawnType == SpawnType::Circle){
        float radious = particleSystem->GetRadious();
        glm::vec3 orientation = particleSystem->GetOrientation();

        std::uniform_real_distribution<float> genX(center.x - (radious*orientation.x), center.x + (radious*orientation.x));
        std::uniform_real_distribution<float> genY(center.y - (radious*orientation.y), center.y + (radious*orientation.y));
        std::uniform_real_distribution<float> genZ(center.z - (radious*orientation.z), center.z + (radious*orientation.z));
        float offX = genX(rng);
        float offY = genY(rng);
        float offZ = genZ(rng);

        //Reset de numeros aleatorios
        genX(rng);genX(rng);genX(rng);
        genY(rng);genY(rng);genY(rng);
        genZ(rng);genZ(rng);genZ(rng);
    }else{
        newPosition = particleSystem->GetCLNode()->GetGlobalTranslation();

    }
    
    
    return newPosition;
}

