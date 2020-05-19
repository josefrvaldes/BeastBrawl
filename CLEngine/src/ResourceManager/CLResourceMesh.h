#pragma once

#include "CLResource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <stddef.h>     /* offsetof */


//! Estructura para almacenar datos de un vertice
//! Guardamos datos que recogemos de assimp como la posicion, normal, coordenada de texturas, tangentes y bitangentes
struct Vertex {
    // position
    glm::vec3 position {0.f,0.f,0.f};
    // normal
    glm::vec3 normal {0.f,0.f,0.f};
    // texCoords
    glm::vec2 texCoords {0.f,0.f};
    // offset
    glm::vec3 animationOffsetPos {0.f,0.f,0.f};
    // offset
    glm::vec3 animationOffsetNormal {0.f,0.f,0.f};
};

//! Estructura para almacenar informacion sobre las texturas del modelado
struct Texture {
    unsigned int id;
    string type;
    string path;
};

//! Clase auxiliar para manejar mas facil las submallas de una malla que leemos
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
    //! Clase para gestionar la lectura de mallas
    //! Mediante la utilización de assimp leemos y gestionamos todos los datos que puede ser de utilidad
    class CLResourceMesh : public CLResource {
        public:
            CLResourceMesh(){};
            ~CLResourceMesh(){};

            void Draw(GLuint shaderID) override;
            bool LoadFile(std::string, bool) override;
            void DrawDepthMap(GLuint shaderID);
            //! Devuelve el vector de mallas
            //! @returns vecMeshes Las posibles submallas que pueda tener la malla leida
            vector<Mesh> GetvectorMesh() { return vecMesh; }
            vector<Mesh> &GetvectorMeshRef() { return vecMesh; }
            vector<Mesh> *GetvectorMeshPtr() { return &vecMesh; }

        private:
            void processNode(aiNode *node, const aiScene *scene);
            Mesh processMesh(aiMesh *mesh, const aiScene *scene);
            vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
            unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
            //Material loadMaterial(aiMaterial* mat); 

            vector<Mesh> vecMesh;
            vector<Texture> textures_loaded;
            string directory;
            bool gammaCorrection;
            //const aiScene *scene;

            
    };
}