#pragma once

#include "CLResource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <stddef.h>     /* offsetof */

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    //glm::vec2 texCoords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh{
    

    public:
        Mesh() {};
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        ~Mesh(){};

        /*  Mesh Data  */
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        unsigned int VAO, VBO, EBO;  

};

namespace CLE {
    class CLResourceMesh : public CLResource {
        public:
            CLResourceMesh(){};
            ~CLResourceMesh(){};

            void Draw(glm::mat4) override;
            bool LoadFile(std::string) override;
            vector<Mesh> GetMesh() { return vecMesh; }

        private:
            void processNode(aiNode *node, const aiScene *scene);
            Mesh processMesh(aiMesh *mesh, const aiScene *scene);

            vector<Mesh> vecMesh;
            const aiScene *scene;

            
    };
}