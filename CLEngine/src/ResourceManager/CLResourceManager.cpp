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
 
 
 #include "CLResourceManager.h"


#include <mutex>
#include "../../../src/Constants.h"

using namespace CLE;

CLResourceManager::CLResourceManager() {
    //TO-DO: Reservar un minimo de espacio
    //meshes = new std::vector<CLResourceMesh*>();
    //shaders = new std::vector<CLResourceShader*>();
    meshes.reserve(300);
    textures.reserve(300);
}

CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file){
    return GetResourceMesh(file,false);
}


CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file, bool flipUV) {
    shared_ptr<CLResourceMesh> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<meshes.size() && search; ++ i) {
        if (!file.compare(meshes[i]->GetName())) {
            resource = meshes[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceMesh>();
        resource->SetName(file);
        if (resource->LoadFile(file, flipUV)) {
            my_mutex.lock();
            meshes.push_back(resource);
            my_mutex.unlock();
        }
    }

    return resource.get();
}

size_t CLResourceManager::GetResourceMeshIndex(const std::string file) {
    for(size_t i = 0; i < meshes.size(); i++) {
        if (meshes[i]->GetName() == file)
            return i;
    }
    return -1;
}


vector<CLResourceMesh*> CLResourceManager::LoadResourceAnimation(const std::string path, uint8_t numKeyFrames, bool flipUV) {
    string folder = path.substr(0, path.find_last_of("/") + 1);
    // string fileName = path.substr(path.find_last_of("/") + 1).substr(3);
    const uint8_t NUMERIC_PART_OF_THE_FILENAME = 11;
    string completeFileName = path.substr(path.find_last_of("/") + 1);
    string fileNameWithoutNumericPart = completeFileName.substr(0, completeFileName.length() - NUMERIC_PART_OF_THE_FILENAME);
    vector<CLResourceMesh*> keyFrames;



    string stringIndex = std::to_string(1);
    // añade ceros al principio para que el fichero no sea 1ojete.obj, sino que sea 001ojete.obj
    string auxIndex = std::string(6 - stringIndex.length(), '0') + stringIndex; 
    string totalPath = folder + fileNameWithoutNumericPart + "_" + auxIndex + ".obj";

    CLResourceMesh *resource = GetResourceMesh(totalPath, flipUV);
    keyFrames.push_back(resource);
    // for (unsigned i = 0; i < thread::hardware_concurrency(); ++i)
    //     tg.create_thread(boost::bind(&boost::asio::io_context::run, &context));

    // ...
    //
    boost::asio::thread_pool tp2{4};
    for(uint8_t i = 2; i <= numKeyFrames; i++) {
        string stringIndex = std::to_string(i);
        // añade ceros al principio para que el fichero no sea 1ojete.obj, sino que sea 001ojete.obj
        string auxIndex = std::string(6 - stringIndex.length(), '0') + stringIndex; 
        string totalPath = folder + fileNameWithoutNumericPart + "_" + auxIndex + ".obj";
        shared_ptr<std::thread> t = make_shared<std::thread>([&, totalPath](){
            boost::asio::post(context,[&, totalPath](){
                CLResourceMesh *resource = GetResourceMesh(totalPath, flipUV);
                my_mutex.lock();
                keyFrames.push_back(resource);
                my_mutex.unlock();
            });
            context.run();
        });
        threads.push_back(t);
        // boost::asio::post(context, [&, totalPath](){
        //     CLResourceMesh *resource = GetResourceMesh(totalPath, flipUV);
        //     keyFrames.push_back(resource);
        // });

        /*boost::asio::post(tp2, [&, totalPath](){
            CLResourceMesh *resource = GetResourceMesh(totalPath, flipUV);
            my_mutex.lock();
            keyFrames.push_back(resource);
            std::cout << "Dentro del lambda keyFrames tiene " << keyFrames.size() << " frames" << endl;
            my_mutex.unlock();
        });*/
    }
    //std::cout << "Antes del join keyFrames tiene " << keyFrames.size() << " frames" << endl;
    //tp2.join();
    for (auto thread : threads)
        thread->join();
    //std::cout << "Después del join keyFrames tiene " << keyFrames.size() << " frames" << endl;
    //std::cout << "Ahora vamos a devolver el array de keyframes" << endl;
    return keyFrames;
}

vector<CLResourceMesh*> CLResourceManager::GetResourceExistingAnimation(const std::string path, uint8_t numKeyFrames, bool flipUV) {
    size_t index = GetResourceMeshIndex(path);
    vector<CLResourceMesh*> outputMeshes;
    if(index >= 0) {
        if(!Constants::ANIM_ACTIVATED)
            numKeyFrames = 1;
        outputMeshes.reserve(numKeyFrames);
        for(size_t i = index; i < index + numKeyFrames; i++) {
            outputMeshes.push_back(meshes[i].get());
        }
    }
    return outputMeshes;
}

bool CLResourceManager::DeleteResourceAnimation(const std::string path, uint8_t numKeyFrames) {
    string folder = path.substr(0, path.find_last_of("/") + 1);
    // string fileName = path.substr(path.find_last_of("/") + 1).substr(3);
    const uint8_t NUMERIC_PART_OF_THE_FILENAME = 11;
    string completeFileName = path.substr(path.find_last_of("/") + 1);
    string fileNameWithoutNumericPart = completeFileName.substr(0, completeFileName.length() - NUMERIC_PART_OF_THE_FILENAME);
    
    uint8_t realKeyFrames = numKeyFrames;
    if(Constants::ANIM_ACTIVATED == 0)
        realKeyFrames = 1;
    
    
    for(uint8_t i = 1; i <= realKeyFrames; i++) {
        string stringIndex = std::to_string(i);
        // añade ceros al principio para que el fichero no sea 1ojete.obj, sino que sea 001ojete.obj
        string auxIndex = std::string(6 - stringIndex.length(), '0') + stringIndex; 
        string totalPath = folder + fileNameWithoutNumericPart + "_" + auxIndex + ".obj";
        DeleteResourceMesh(totalPath);
    }
    return true;
}


vector<CLResourceMesh*> CLResourceManager::GetResourceAnimation(const std::string path, uint8_t numKeyFrames, bool flipUV) {
    string folder = path.substr(0, path.find_last_of("/") + 1);
    // string fileName = path.substr(path.find_last_of("/") + 1).substr(3);
    const uint8_t NUMERIC_PART_OF_THE_FILENAME = 11;
    string completeFileName = path.substr(path.find_last_of("/") + 1);
    string fileNameWithoutNumericPart = completeFileName.substr(0, completeFileName.length() - NUMERIC_PART_OF_THE_FILENAME);
    vector<CLResourceMesh*> keyFrames;

    // for (unsigned i = 0; i < thread::hardware_concurrency(); ++i)
    //     tg.create_thread(boost::bind(&boost::asio::io_context::run, &context));

    // ...
    //
    
    uint8_t realKeyFrames = numKeyFrames;
    if(Constants::ANIM_ACTIVATED == 0)
        realKeyFrames = 1;
    for(uint8_t i = 1; i <= realKeyFrames; i++) {
        string stringIndex = std::to_string(i);
        // añade ceros al principio para que el fichero no sea 1ojete.obj, sino que sea 001ojete.obj
        string auxIndex = std::string(6 - stringIndex.length(), '0') + stringIndex; 
        string totalPath = folder + fileNameWithoutNumericPart + "_" + auxIndex + ".obj";
        CLResourceMesh *resource = GetResourceMesh(totalPath, flipUV);
        keyFrames.push_back(resource);
    }
    return keyFrames;
}

CLResourceTexture* CLResourceManager::GetResourceTexture(const std::string file){
    return GetResourceTexture(file,false);
}


CLResourceTexture* CLResourceManager::GetResourceTexture(const std::string file, bool vertically) {
    shared_ptr<CLResourceTexture> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<textures.size() && search; ++ i) {
        if (!file.compare(textures[i]->GetName())) {
            resource = textures[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceTexture>();
        resource->SetName(file);
        if (resource->LoadFile(file, vertically)) {
            textures.push_back(resource);
        }
    }

    

    return resource.get();
}

CLResourceMaterial* CLResourceManager::GetResourceMaterial(const std::string file) {
    return GetResourceMaterial(file,false);
}

CLResourceMaterial* CLResourceManager::GetResourceMaterial(const std::string file, bool vertically) {
    shared_ptr<CLResourceMaterial> resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<materials.size() && search; ++ i) {
        if (!file.compare(materials[i]->GetName())) {
            resource = materials[i];
            search = false;
        }
    }
    if (!resource) {
        resource = make_shared<CLResourceMaterial>();
        resource->SetName(file);
        if (resource->LoadFile(file, vertically)) {
            materials.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string vertex, const std::string fragment) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!vertex.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        //cout << "Creo el shader: " << vertex <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(vertex);
        //resource->SetShaderType(type);
        if (resource->LoadFile(vertex,fragment)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string vertex, const std::string fragment, const std::string geometry) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!vertex.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        //cout << "Creo el shader: " << vertex <<endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(vertex);
        //resource->SetShaderType(type);
        if (resource->LoadFile(vertex,fragment,geometry)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}


bool CLResourceManager::DeleteResourceTexture(const std::string file){

    for (unsigned int i=0; i<textures.size(); ++ i) {
        if (!file.compare(textures[i]->GetName())) {
            textures.erase(textures.begin()+i);
            return true;
        }
    }

    return false;
}

bool CLResourceManager::DeleteResourceMesh(const std::string file){
    // cout << meshes.size() << endl;
    for (unsigned int i=0; i<meshes.size(); ++ i) {
        if (!file.compare(meshes[i]->GetName())) {
            meshes.erase(meshes.begin()+i);
            // cout << meshes.size() << endl;

            return true;
        }
    }
    return false;
}

bool CLResourceManager::DeleteResourceMaterial(const std::string file){
    for (unsigned int i=0; i<materials.size(); ++ i) {
        if (!file.compare(materials[i]->GetName())) {
            materials.erase(materials.begin()+i);
            return true;
        }
    }

    return false;
}