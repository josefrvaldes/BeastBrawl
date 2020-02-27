#include "CLResourceMesh.h"

// MESH -----------------------------------------------------------------------------

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices  = indices;
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int),&this->indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    //glEnableVertexAttribArray(2);	
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    // vertex tangent
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // // vertex bitangent
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}


// CLRESOURCEMESH -------------------------------------------------------------------

using namespace CLE;

/**
 * 
 * @param file - Ruta del archivo a leer.
 */
bool CLResourceMesh::LoadFile(std::string file) {
    Assimp::Importer importer;

    // Importamos el fichero.
    scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace );

    // Error de carga
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return false;
    }
    cout << "Creo que he creado una malla\n";
    processNode(scene->mRootNode, scene);
    return true;
}  

/**
 * Recorremos los nodos para recuperar todos la informacion de la malla.
 * @param node - 
 * @param scene - 
 */
void CLResourceMesh::processNode(aiNode *node, const aiScene *scene){
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        vecMesh.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

/**
 * Almacenamos los valores que recoge assimp de cada submalla.
 * @param mesh -
 * @param scene -
 */
Mesh CLResourceMesh::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures; 

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vecAux; // we declare a placeholder vecAux since assimp uses its own vecAux class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
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
        // if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        // {
        //     glm::vec2 vec;
        //     // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
        //     // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
        //     vec.x = mesh->mTextureCoords[0][i].x; 
        //     vec.y = mesh->mTextureCoords[0][i].y;
        //     vertex.texCoords = vec;
        // }
        // else
        //     vertex.texCoords = glm::vec2(0.0f, 0.0f);

        //tangent
        if(mesh->HasTangentsAndBitangents()){
            vecAux.x = mesh->mTangents[i].x;
            vecAux.y = mesh->mTangents[i].y;
            vecAux.z = mesh->mTangents[i].z;
            vertex.tangent = vecAux;
            //bitangent
            vecAux.x = mesh->mBitangents[i].x;
            vecAux.y = mesh->mBitangents[i].y;
            vecAux.z = mesh->mBitangents[i].z;
            vertex.bitangent = vecAux;
        }
        
        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        //...
    }   

    return Mesh(vertices, indices, textures);
}  

void CLResourceMesh::Draw(glm::mat4) {

    for(auto& mesh : vecMesh){
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //mesh.Draw();
    }
}