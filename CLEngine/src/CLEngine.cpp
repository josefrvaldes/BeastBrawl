#include "CLEngine.h"

using namespace std;
using namespace CLE;

/**
 * Funcion callback de errores de GLFW.
 * @param error - ID del error.
 * @param descripcion - Detalles sobre el error.
 */
static void error(int error, const char* description) {
    cerr << "Error (" << error << "): " << description << endl;
}

/**
 * Constructor que iniciara todas las configuraciones iniciales de OpenGL y la ventana.
 * @param w - Anchura en pixeles de la ventana.
 * @param h - Altura en pixeles de la ventana.
 * @param title - Titulo de la ventana.
 */
CLEngine::CLEngine (const unsigned int w, const unsigned int h, const string& title) {
    CreateGlfwWindow(w, h, title);
    glewInit();
    ImGuiInit();
}

/**
 * Destruye la ventana de GLFW y libera la informacion.
 */
CLEngine::~CLEngine() {
    TerminateImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    cout << ">>>>> GLFW OFF" << endl;
}

/**
 * Inicia, crea y configura  la ventana OpenGL con GLFW.
 * @param w - Anchura en pixeles de la ventana.
 * @param h - Altura en pixeles de la ventana.
 * @param title - Titulo de la ventana.
 */
void CLEngine::CreateGlfwWindow (const unsigned int w, const unsigned int h, const string& title) {

    glfwSetErrorCallback(error);

    if (!glfwInit()) {
        cout << "- No se ha podido inicializar GLFW" << endl;
        exit(EXIT_FAILURE);
    }
    cout << ">>>>> GLFW ON" << endl;

    // Minima version de OpenGL, si la maquina no puede correr minimo a la indicada, falla la creacion de la ventana.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( w, h, title.c_str(), NULL, NULL );
    if (!window) {
        cout << "    > La ventana no se ha podido crear" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    cout << "    > Ventana creada" << endl;


    glfwMakeContextCurrent(window);

    // Por defecto esta a 0, pero parece que eso despercicia ciclos de CPU y GPU. Se recomienda ponerlo a 1.
    glfwSwapInterval(1);

    // Activa el buffer de profundidad o ZBuffer, para que se diferencie que pixel se debe pintar.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
}


CLNode* CLEngine::GetSceneManager(){
    if(!smgr){
        smgr = make_unique<CLNode>();
    }

    return smgr.get();
}

CLResourceManager* CLEngine::GetResourceManager(){
    

    return CLResourceManager::GetResourceManager();
}

/**
 * Actualizacion de CLEngine.
 */
bool CLEngine::Run() {
    return !(glfwWindowShouldClose(window));
}

void CLEngine::PollEvents(){
    glfwPollEvents();
}

/**
 * Limpia la pantalla pintandola de un color.
 */
void CLEngine::BeginScene(){
    glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void CLEngine::DrawObjects(){
    smgr->DrawSkybox();
    smgr->CalculateViewProjMatrix();
    //smgr->CalculateLights();
    smgr->DFSTree(glm::mat4(1.0f));
}

/**
 * Renderiza las cosas de ImGui y cambia el buffer de la ventana. 
 */
void CLEngine::EndScene(){
    
    glfwSwapBuffers(window);

}

/**
 *
 */
void CLEngine::SetTitle(string &t) {
    glfwSetWindowTitle(window, t.c_str());
}

void CLEngine::RenderImgui(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 * Devuelve el tiempo de glfw
 */
double CLEngine::GetTime(){
    return glfwGetTime();
}
/**
 * Mira si se han actualizado los valores de anchura y altura de la ventana y actualiza el viewport.
 */
void CLEngine::UpdateViewport(){
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

//Borrar esto 
void CLEngine::InputClose(){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    
}

void CLEngine::CloseWindow(){
    glfwSetWindowShouldClose(window, true);

}



// -----------------------------------------------------------
//  IMGUI
// -----------------------------------------------------------

void CLEngine::ImGuiInit(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void CLEngine::TerminateImGui(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}





/// --------------------------------
///  SOMBRAS
/// --------------------------------

// Renderiza el depth map
void CLEngine::RenderDepthMap(CLShadowMapping& shadowMap, CLResourceShader* depthShader, glm::vec3 posLight){
    // crear las matrices de transformacion del cubemap
    float aspect = (float)shadowMap.SHADOW_WIDTH/(float)shadowMap.SHADOW_HEIGHT;
    float near = 1.0f;
    float far = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far); 
    // view Matrix
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));


    //glUseProgram(simpleDepthShader);
    //glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    //glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader, "model"), 1, GL_FALSE, glm::value_ptr(shadowMap->));


    // renderizar escena para el depth cubemap
    glViewport(0, 0, shadowMap.SHADOW_WIDTH,  shadowMap.SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER,  shadowMap.depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(depthShader->GetProgramID());



    for (unsigned int i = 0; i < 6; ++i){
        string name = "shadowMatrices[" + std::to_string(i) + "]";
        glUniformMatrix4fv(glGetUniformLocation(depthShader->GetProgramID(), name.c_str()), 1, GL_FALSE, &(shadowTransforms[i])[0][0]);
    }
    glUniform1f(glGetUniformLocation(depthShader->GetProgramID(), "far_plane"), far); 
    glUniform3fv(glGetUniformLocation(depthShader->GetProgramID(), "lightPos"), 1, &posLight[0]); 
    
    
    smgr->DFSTree(glm::mat4(1.0f), depthShader->GetProgramID());
        //shadowMap.Draw(simpleDepthShader);
        //RenderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
