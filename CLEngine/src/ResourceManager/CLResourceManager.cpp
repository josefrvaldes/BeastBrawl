#include "CLResourceManager.h"

using namespace CLE;

CLResourceManager::CLResourceManager() {
    //TO-DO: Reservar un minimo de espacio
    //meshes = new std::vector<CLResourceMesh*>();
    //shaders = new std::vector<CLResourceShader*>();
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
            meshes.push_back(resource);
        }
    }

    return resource.get();
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
        cout << "Creo el shader: " << vertex <<endl;
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
        cout << "Creo el shader: " << vertex <<endl;
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
    for (unsigned int i=0; i<meshes.size(); ++ i) {
        if (!file.compare(meshes[i]->GetName())) {
            meshes.erase(meshes.begin()+i);
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