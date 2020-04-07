#include "CLEngine.h"
#include "../../../src/Constants.h"

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
    const std::string f = "fonts/arial.ttf";
    LoadFont(f);
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


/**
 *
 */
 void CLEngine::LoadFont(const std::string &font) {
    //SHADER
    if (!textShader) {
        auto resourceShader = CLResourceManager::GetResourceManager()->GetResourceShader("CLEngine/src/Shaders/textShader.vert", "CLEngine/src/Shaders/textShader.frag");
        textShader = resourceShader->GetProgramID();
    }

    glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
    glUseProgram(textShader);
    glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Fallo al cargar la libreria Free Type" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: No se ha podido cargar la fuente. ¿La tienes?" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Carga los primetos 128 caracteres ASCII
    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: No se ha podio cargar el glifo" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
        };
        characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Destruye FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Configuramos los VAO/VBO para los quads de cada textura
    glGenVertexArrays(1, &VAOText);
    glGenBuffers(1, &VBOText);
    glBindVertexArray(VAOText);
    glBindBuffer(GL_ARRAY_BUFFER, VBOText);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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


void CLEngine::DrawDepthMap(){
    auto light = smgr->GetNodeByID(smgr->GetShadowMapping()->GetID());

    // 1. Se renderiza con el shadowMap
    //glCullFace(GL_FRONT);
    RenderDepthMap(*smgr->GetShadowMapping(), smgr->GetDepthShader(), light->GetGlobalTranslation());
    //glCullFace(GL_BACK);

    // 2. then render scene as normal with shadow mapping (using depth map)
    UpdateViewport();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



void CLEngine::DrawObjects(){
    DrawDepthMap();

    smgr->DrawSkybox();
    smgr->CalculateViewProjMatrix();
    //smgr->CalculateLights();
    smgr->DFSTree(glm::mat4(1.0f));
    smgr->DrawBillBoard();
    // smgr->DrawGrass();

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
 * Renderizamos el texto por pantalla
 * @param text - Cadena de caracteres a renderizar
 * @param x - Posicion en X
 * @param y - Posicion en Y
 * @param depth - Profundidas del render
 * @param scale - Tamanyo de la letra
 * @param color - Color
 */
void CLEngine::RenderText2D(std::string& text, GLfloat x, GLfloat y, GLfloat depth, GLfloat scale, glm::vec3& color) {

    glUseProgram(textShader);
    glUniform3f(glGetUniformLocation(textShader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOText);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        //cout << "YPOS: " << ypos << endl;

        // Update VBO for each character
        GLfloat vertices[6][5] = {
                { xpos,     ypos,       depth,   0.0f, 1.0f },
                { xpos + w, ypos,       depth,   1.0f, 1.0f },
                { xpos,     ypos + h,   depth,   0.0f, 0.0f },

                { xpos,     ypos + h,   depth,   0.0f, 0.0f },
                { xpos + w, ypos,       depth,   1.0f, 1.0f },
                { xpos + w, ypos + h,   depth,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBOText);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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

void CLEngine::SetTitle(string &&t) {
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

void CLEngine::Clear(){
    if (smgr) {
        for (const auto& c : smgr->GetChilds()) {
            smgr->RemoveChild(c.get());
        }
        smgr->RemoveLightsAndCameras();
    }
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
    float far = Constants::FAR_PLANE;
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
