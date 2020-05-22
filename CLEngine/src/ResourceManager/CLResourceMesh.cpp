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
 
 
 #include "CLResourceMesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// MESH -----------------------------------------------------------------------------

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Seleccion del Vertex Array porque vamos a trabajar con el VBO (el VBO esta en el VAO).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Como en el struct Vertex esta todo seguido podemos pasarselo entero al glBufferData
    // Leemos la informacion de los triangulos.
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    // Leemos la informacion de los indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

// CLRESOURCEMESH -------------------------------------------------------------------
#include "../SceneTree/CLShadowMapping.h"

using namespace CLE;

bool CLResourceMesh::LoadFile(std::string file, bool flipUV) {
    Assimp::Importer importer;
    auto assimpFlags = aiProcess_Triangulate | /*aiProcess_FlipUVs | */ aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_TransformUVCoords | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_GenUVCoords;

    if (flipUV)
        assimpFlags |= aiProcess_FlipUVs;
    // Importamos el fichero.
    const aiScene* scene = importer.ReadFile(file, assimpFlags);

    // Error de carga
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        //cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return false;
    }
    //cout << "Leida la malla: " << file << endl;
    processNode(scene->mRootNode, scene);
    return true;
}

void CLResourceMesh::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        vecMesh.push_back(processMesh(mesh, scene)); 
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh CLResourceMesh::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    //vector<Material> materials;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vecAux;  // we declare a placeholder vecAux since assimp uses its own vecAux class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vecAux.x = mesh->mVertices[i].x;
        vecAux.y = mesh->mVertices[i].y;
        vecAux.z = mesh->mVertices[i].z;
        vertex.position = vecAux;
        // normals
        vecAux.x = mesh->mNormals[i].x;
        vecAux.y = mesh->mNormals[i].y;
        vecAux.z = mesh->mNormals[i].z;
        vertex.normal = vecAux;
        // texture coordinates
        if (mesh->mTextureCoords[0])  // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);


        //Texture coords
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0) {
        // // process materials
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];


        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        //3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

vector<Texture> CLResourceMesh::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;  // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip) {  // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

unsigned int CLResourceMesh::TextureFromFile(const char *path, const string &directory, bool gamma) {
    string filename = string(path);

    //Eliminamos las rutas por si en windows hemos modelado dentro de una carpeta
    std::string delimiter = "\\";

    size_t pos = 0;
    std::string token;
    while ((pos = filename.find(delimiter)) != std::string::npos) {
        token = filename.substr(0, pos);
        //std::cout << token << std::endl;
        filename.erase(0, pos + delimiter.length());
    }

    filename = "media/clv_" + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        //std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void CLResourceMesh::Draw(GLuint shaderID) {
    for (auto &mesh : vecMesh) {
        // bind appropriate textures
        // unsigned int diffuseNr  = 1;
        // unsigned int specularNr = 1;
        // unsigned int normalNr   = 1;
        // unsigned int heightNr   = 1;

        // TODO: SI QUIERES ANIMACIONES INTERPOLADAS DESCOMENTA ESTO
        // esto sirve para actualizar las posiciones de los vértices que modifican las animaciones
        // glBindVertexArray(mesh.VAO);
        // glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        // glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

        // // estas líneas son necesarias para las animaciones
        // // vertex animationOffsetPos
        // glEnableVertexAttribArray(3);
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, animationOffsetPos));
        // // vertex animationOffsetNormal
        // glEnableVertexAttribArray(4);
        // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, animationOffsetNormal));
        


        for (unsigned int i = 0; i < mesh.textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);  // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string name = mesh.textures[i].type;
            if (name == "texture_diffuse")
                glUniform1i(glGetUniformLocation(shaderID, "material.diffuse"), i);
            else if (name == "texture_specular")
                glUniform1i(glGetUniformLocation(shaderID, "material.specular"), i);

            else if (name == "texture_normal")
                glUniform1i(glGetUniformLocation(shaderID, "material.normal"), i);

            else if (name == "texture_height")
                glUniform1i(glGetUniformLocation(shaderID, "material.height"), i);

            // now set the sampler to the correct texture unit
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id); 
        }
        //glActiveTexture(GL_TEXTURE1);
        //glUniform1i(glGetUniformLocation(shaderID, "depthMap"), 1);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, CLShadowMapping::depthCubemap);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shaderID, "depthMap"), 1);
        glBindTexture(GL_TEXTURE_2D, CLShadowMapping::depthMap);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }
}

void CLResourceMesh::DrawDepthMap(GLuint shaderID) {
    for (auto &mesh : vecMesh) {
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}