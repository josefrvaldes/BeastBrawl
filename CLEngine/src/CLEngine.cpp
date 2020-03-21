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
CLEngine::CLEngine (const unsigned int w, const unsigned int h, const string& title) : width(w), height(h) {
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

    window = glfwCreateWindow( w, h, title.c_str(), nullptr, nullptr );
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

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void CLEngine::DrawObjects(){
    smgr->DrawSkybox();
    smgr->CalculateViewProjMatrix();
    //smgr->CalculateLights();
    smgr->DFSTree(glm::mat4(1.0f));
}

void CLEngine::DrawImage2D(float _x, float _y, float _width, float _height, float _depth, string& file, bool vertically){
    if(!hudShader){
        auto resourceShader = CLResourceManager::GetResourceManager()->GetResourceShader("CLEngine/src/Shaders/spriteShader.vert", "CLEngine/src/Shaders/spriteShader.frag");
        hudShader = resourceShader->GetProgramID();
    }

    float nXLeft    =     (2.0f * _x)/width - 1.0f;
    float nYUp      =     -1.0f * (((2.0f * _y)/height) - 1.0f);
    float nXRight   =     ((2.0f * _width) / width) + nXLeft;
    float nYDown    =     -1.0f * (((2.0f * _height) / height)) + nYUp;

    float vertices[] = {                    // TEXT CORDS
        nXRight,    nYUp,       _depth,       1.0f, 1.0f,         // top right
        nXRight,    nYDown,     _depth,       1.0f, 0.0f,         // bottom right
        nXLeft,     nYDown,     _depth,       0.0f, 0.0f,         // bottom left
        nXLeft,     nYUp,       _depth,       0.0f, 1.0f          // top left
    };

    unsigned int indices[] = {
            0, 3, 1,
            3, 2, 1
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    texture = static_cast<CLResourceTexture*>(CLResourceManager::GetResourceManager()->GetResourceTexture(file, vertically))->GetTextureID();

    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram(hudShader);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

