#include "CLResourceManager.h"

using namespace CLE;

CLResourceManager::CLResourceManager() {
    //TO-DO: Reservar un minimo de espacio
    //meshes = new std::vector<CLResourceMesh*>();
    //shaders = new std::vector<CLResourceShader*>();
}

CLResourceManager::~CLResourceManager() {
    
}

CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file) {
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
        if (resource->LoadFile(file)) {
            meshes.push_back(resource);
        }
    }

    return resource.get();
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string file, GLenum type) {
    shared_ptr<CLResourceShader> resource = NULL;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (!file.compare(shaders[i]->GetName())) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        cout << "Creo un shader" << endl;
        resource = make_shared<CLResourceShader>();
        resource->SetName(file);
        resource->SetShaderType(type);
        if (resource->LoadFile(file)) {
            shaders.push_back(resource);
        }
    }

    return resource.get();
}
