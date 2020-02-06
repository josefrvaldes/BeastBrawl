#pragma once

#include "CLEntity.h"
#include "../ResourceManager/CLResourceMesh.h"
#include <stddef.h>     /* offsetof */

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
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

namespace CLE {

    class CLMesh : public CLEntity {
        public:
            CLMesh();
            CLMesh(unsigned int idEntity) : CLEntity(idEntity) {};
            ~CLMesh() {};

            void SetMesh(CLResourceMesh* m)         { mesh = m; }

            //GETTERS
            CLResource* GetMesh() const             { return mesh; }
            vector<Vertex> GetVertices() { return vertices; };
            vector<unsigned int> GetIndices() { return indices; };
            vector<Texture> GetTextures() { return textures; };

            void Draw(glm::mat4&);


        private:
            CLResource* mesh;

            /*  Mesh Data  */
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> textures;
            unsigned int VAO, VBO, EBO;

    };
}