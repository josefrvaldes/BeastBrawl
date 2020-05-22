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
 
 
 #include "CLResourceMaterial.h"

using namespace CLE;

bool CLResourceMaterial::LoadFile(string file, bool vertically){

    Assimp::Importer importer;

    // Importamos el fichero.
    scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace 
                                | aiProcess_OptimizeMeshes);

    // Error de carga 
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        //cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return false;
    }
    //cout << "Leida el material: " << file << endl;
    processNode(scene->mRootNode, scene);
    return true;
}


void CLResourceMaterial::processNode(aiNode *node, const aiScene *scene){
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        materials.push_back(processMaterial(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Material CLResourceMaterial::processMaterial(aiMesh *mesh, const aiScene *scene)
{
    Material mat;
    
    // process material
    if(mesh->mMaterialIndex >= 0)
    { 
        // process materials 
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        mat = loadMaterial(material); 
        
        
        
    }   
    return mat;

}  


Material CLResourceMaterial::loadMaterial(aiMaterial* mat) {
    //Ruben del futuro:
    //Una vez tienes estos valores se los mandas al fragment por el struct material y ya estaria yo creo
    Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;


    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.shininess = shininess;

    return material;
}

void CLResourceMaterial::Draw(GLuint shaderID){
    glUseProgram(shaderID);

    for(auto mat : materials){

        glUniform3fv(glGetUniformLocation(shaderID, "material.ambient"),1, glm::value_ptr(mat.ambient));
        glUniform3fv(glGetUniformLocation(shaderID, "material.diffuse"),1, glm::value_ptr(mat.diffuse));
        glUniform3fv(glGetUniformLocation(shaderID, "material.specular"),1, glm::value_ptr(mat.specular));
        glUniform1i(glGetUniformLocation(shaderID, "material.shininess"), mat.shininess);

    }
}