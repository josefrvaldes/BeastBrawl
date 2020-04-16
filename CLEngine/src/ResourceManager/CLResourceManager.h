#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "../../include/glew/glew.h"

#include "CLResourceMesh.h"
#include "CLResourceShader.h"
#include "CLResourceTexture.h"
#include "CLResourceMaterial.h"

using namespace std;
namespace CLE {

    //! Clase para gestionar la lectura de recursos
    //! Esta clase engloba todos los metodos para leer diferentes tipos de archivos
    //! @note Es singleton
    class CLResourceManager {
        public:
            ~CLResourceManager(){delete instance;};
            static CLResourceManager* GetResourceManager(){
                if(!instance){
                    instance = new CLResourceManager();
                }
                return instance;
            }

            //! Metodo para leer mallas
            //! @param file Ruta al fichero
            //! @param flipUV Bool para invertir los UV's al leer la textura
            //! @returns Puntero a un CLE::CLResource::CLResourceMesh
            CLResourceMesh* GetResourceMesh(const std::string file, bool flipUV);
            //! Metodo para leer mallas
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceMesh
            CLResourceMesh* GetResourceMesh(const std::string file);
            //! Metodo para leer texturas
            //! @param file Ruta al fichero
            //! @param vertically Bool para poner en vertical la textura
            //! @returns Puntero a un CLE::CLResource::CLResourceTexture
            CLResourceTexture* GetResourceTexture(const std::string file, bool vertically);
            //! Metodo para leer texturas
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceTexture
            CLResourceTexture* GetResourceTexture(const std::string file);
            //! Metodo para leer materiales
            //! @param file Ruta al fichero
            //! @param vertically Bool para invertir
            //! @returns Puntero a un CLE::CLResource::CLResourceMaterial
            CLResourceMaterial* GetResourceMaterial(const std::string file, bool vertically);
            //! Metodo para leer materiales
            //! @param file Ruta al fichero
            //! @returns Puntero a un CLE::CLResource::CLResourceMaterial
            CLResourceMaterial* GetResourceMaterial(const std::string file);
            //! Metodo para leer shaders
            //! @param vertex Ruta al vertex shader
            //! @param fragment Ruta al fragment shader
            //! @returns Puntero a un CLE::CLResource::CLResourceShader
            CLResourceShader* GetResourceShader(const std::string vertex, const std::string fragment);
            //! Metodo para leer shaders
            //! @param vertex Ruta al vertex shader
            //! @param fragment Ruta al fragment shader
            //! @param geometry Ruta al geometry shader
            //! @returns Puntero a un CLE::CLResource::CLResourceShader
            CLResourceShader* GetResourceShader(const std::string vertex, const std::string fragment, const std::string geometry);

        private:
            CLResourceManager();
            inline static CLResourceManager* instance { nullptr };
            vector<shared_ptr<CLResourceMesh>> meshes;
            vector<shared_ptr<CLResourceTexture>> textures;
            vector<shared_ptr<CLResourceMaterial>> materials;
            vector<shared_ptr<CLResourceShader>> shaders;

    };

}