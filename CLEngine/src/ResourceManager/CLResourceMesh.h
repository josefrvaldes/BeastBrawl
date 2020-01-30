#pragma once

#include "CLResource.h"

namespace CLE {
    class CLResourceMesh : public CLResource {
        public:
            CLResourceMesh(){}
            ~CLResourceMesh();

            void Draw();
            bool LoadFile(std::string);

            // Como no se para que son las variables, no se si hacen falta todos estos SET y GET
            void SetVertex(float* v)            { vertex = v; }
            void SetNormals(float* n)           { normals = n; }
            void SetTextures(float* t)          { textures = t; }
            void SetVertTriangles(float* vt)    { vertTriangles = vt; }
            void SetNormTriangles(float* nt)    { normTriangles = nt; }
            void SetTextTriangles(float* tt)    { textTriangles = tt; }
            void SetNumTriangles(long n)        { numTriangles = n; }

            float* GetVertex()          const { return vertex; }
            float* GetNormals()         const { return normals; }
            float* GetTextures()        const { return textures; }
            float* GetVertTriangles()   const { return vertTriangles; }
            float* GetNormTriangles()   const { return normTriangles; }
            float* GetTextTriangles()   const { return textTriangles; }
            long   GetNumTriangles()    const { return numTriangles; }
        
        private:

            // No se lo que son estas variables, sinceramente.
            float* vertex;
            float* normals;
            float* textures;

            float* vertTriangles;
            float* normTriangles;
            float* textTriangles;

            long numTriangles { 0 };

    };
}