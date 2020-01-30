#include "CLResourceManager.h"

using namespace CLE;

CLResourceManager::CLResourceManager() {
    //TO-DO: Reservar un minimo de espacio
    //meshes = new std::vector<CLResourceMesh*>();
    //shaders = new std::vector<CLResourceShader*>();
}

CLResourceMesh* CLResourceManager::GetResourceMesh(const std::string file) {
    CLResourceMesh* resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<meshes.size() && search; ++ i) {
        if (file == meshes[i]->GetName()) {
            resource = meshes[i];
            search = false;
        }
    }
    if (!resource) {
        resource = new CLResourceMesh();
        resource->SetName(file);
        if (resource->LoadFile(file)) {
            meshes.push_back(resource);
        }
    }

    return resource;
}

CLResourceShader* CLResourceManager::GetResourceShader(const std::string file) {
    CLResourceShader* resource = nullptr;
    bool search = true;
    for (unsigned int i=0; i<shaders.size() && search; ++ i) {
        if (file == shaders[i]->GetName()) {
            resource = shaders[i];
            search = false;
        }
    }
    if (!resource) {
        resource = new CLResourceShader();
        resource->SetName(file);
        if (resource->LoadFile(file)) {
            shaders.push_back(resource);
        }
    }

    return resource;
}
