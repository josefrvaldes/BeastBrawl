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

// struct Material {
//     glm::vec3 ambient;
//     glm::vec3 diffuse;
//     glm::vec3 specular;
//     float shininess;
// };

class Mesh{
    

    public:
        Mesh() {};
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        ~Mesh(){};

        /*  Mesh Data  */
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        //vector<Material> materials;
        unsigned int VAO, VBO, EBO;  

};

namespace CLE {
    class CLResourceMesh : public CLResource {
        public:
            CLResourceMesh(){};
            ~CLResourceMesh(){};

            void Draw(GLuint shaderID) override;
            bool LoadFile(std::string) override;
            vector<Mesh> GetvectorMesh() { return vecMesh; }

        private:
            void processNode(aiNode *node, const aiScene *scene);
            Mesh processMesh(aiMesh *mesh, const aiScene *scene);
            vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
            unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
            //Material loadMaterial(aiMaterial* mat); 

            vector<Mesh> vecMesh;
            vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
            string directory;
            bool gammaCorrection;
            const aiScene *scene;

            
    };
}