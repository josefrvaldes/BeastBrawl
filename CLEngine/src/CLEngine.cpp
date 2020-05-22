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
    const std::string f = "fonts/Plump.ttf";
    LoadFont(f);

    smgr = make_unique<CLNode>();

    //Vamos a leer todos los shaders al iniciar
    CLResourceManager* resourceManager = CLResourceManager::GetResourceManager();

    shaders.emplace_back(resourceManager->GetResourceShader("CLEngine/src/Shaders/shadowMappingShader.vert", "CLEngine/src/Shaders/shadowMappingShader.frag")->GetProgramID());
    shaders.emplace_back(resourceManager->GetResourceShader("CLEngine/src/Shaders/cartoonShader.vert", "CLEngine/src/Shaders/cartoonShader.frag")->GetProgramID());
    shaders.emplace_back(resourceManager->GetResourceShader("CLEngine/src/Shaders/lightMapping.vert", "CLEngine/src/Shaders/lightMapping.frag")->GetProgramID());
    shaders.emplace_back(resourceManager->GetResourceShader("CLEngine/src/Shaders/basicShader.vert", "CLEngine/src/Shaders/basicShader.frag")->GetProgramID());

}

/**
 * Destruye la ventana de GLFW y libera la informacion.
 */
CLEngine::~CLEngine() {
    TerminateImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    //cout << ">>>>> GLFW OFF" << endl;
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
        //cout << "- No se ha podido inicializar GLFW" << endl;
        exit(EXIT_FAILURE);
    }
    //cout << ">>>>> GLFW ON" << endl;

    // Minima version de OpenGL, si la maquina no puede correr minimo a la indicada, falla la creacion de la ventana.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( w, h, title.c_str(), nullptr, nullptr );
    if (!window) {
        //cout << "    > La ventana no se ha podido crear" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //cout << "    > Ventana creada" << endl;


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
 * Activa o desactiva el test de profundidad
 */
void CLEngine::SetEnableDepthTest(bool b) {
    if (b)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
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
            //std::cout << "ERROR::FREETYTPE: No se ha podio cargar el glifo" << std::endl;
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


void CLEngine::DrawDepthMap(const glm::mat4& lightSpaceMatrix){
    //auto light = GetNodeByID(GetShadowMapping()->GetID());

    // 1. Se renderiza con el shadowMap
    glCullFace(GL_FRONT);
    RenderDepthMap(*GetShadowMapping(), GetDepthShader(), lightSpaceMatrix);
    glCullFace(GL_BACK);

    // 2. then render scene as normal with shadow mapping (using depth map)
    UpdateViewport();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



void CLEngine::DrawObjects(){
    glm::mat4 lightSpaceMatrix;
    if(shadowMapping && shadowsActivate){
        auto light = GetNodeByID(GetShadowMapping()->GetID());
        glm::mat4 lightProjection, lightView;
        float near_plane = 1.0f, far_plane = Constants::FAR_PLANE;
        lightProjection = glm::ortho(-float(1500), float(1500),  -float(1500), float(1500), near_plane, far_plane);
        lightView = glm::lookAt(light->GetGlobalTranslation(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        DrawDepthMap(lightSpaceMatrix);
    }

    DrawSkybox();
    CalculateViewProjMatrix(lightSpaceMatrix);
    CalculateLights();
    glm::mat4 VPmatrix = projection*view;
    smgr->DFSTree(glm::mat4(1.0f),GetActiveCamera(), VPmatrix);

    if(grassActivate)
        DrawGrass();
}


void CLEngine::DrawImage2D(float _x, float _y, float _width, float _height, float _depth, string file, bool vertically){
    if(!hudShader){
        auto resourceShader = CLResourceManager::GetResourceManager()->GetResourceShader("CLEngine/src/Shaders/spriteShader.vert", "CLEngine/src/Shaders/spriteShader.frag");
        hudShader = resourceShader->GetProgramID();
        shaders.push_back(hudShader);
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
    auto resourceTexture = CLResourceManager::GetResourceManager()->GetResourceTexture(file, vertically);
    if(!resourceTexture){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        return;
    }
    texture = static_cast<CLResourceTexture*>(resourceTexture)->GetTextureID();

    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram(hudShader);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}



void CLEngine::DrawImage2D(float _x, float _y, float scale, float _depth, string file, bool vertically){
    if(!hudShader){
        auto resourceShader = CLResourceManager::GetResourceManager()->GetResourceShader("CLEngine/src/Shaders/spriteShader.vert", "CLEngine/src/Shaders/spriteShader.frag");
        hudShader = resourceShader->GetProgramID();
        shaders.push_back(hudShader);
    }
    auto resourceTexture = CLResourceManager::GetResourceManager()->GetResourceTexture(file, vertically);
    if(!resourceTexture) {
        return;
    }

    float w = resourceTexture->GetWidth()*scale;
    float h = resourceTexture->GetHeight()*scale;

    float nXLeft    =     (2.0f * _x)/width - 1.0f;
    float nYUp      =     -1.0f * (((2.0f * _y)/height) - 1.0f);
    float nXRight   =     ((2.0f * w) / width) + nXLeft;
    float nYDown    =     -1.0f * (((2.0f * h) / height)) + nYUp;

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
    if(!resourceTexture){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        return;
    }
    texture = static_cast<CLResourceTexture*>(resourceTexture)->GetTextureID();

    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram(hudShader);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
void CLEngine::RenderText2D(std::string text, GLfloat x, GLfloat y, GLfloat depth, GLfloat scale, glm::vec3 color) {

    glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
    glUseProgram(textShader);
    glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
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

void CLEngine::DisableCursor(){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
/**
 * Mira si se han actualizado los valores de anchura y altura de la ventana y actualiza el viewport.
 */
void CLEngine::UpdateViewport(){
    glfwGetFramebufferSize(window, &width, &height);
    // glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
    // cout << "width["<<width<<"] height["<<height<<"]" << endl;
}


void CLEngine::CloseWindow(){
    glfwSetWindowShouldClose(window, true);

}

void CLEngine::Clear(){
    if (smgr) {
        for (const auto& c : smgr->GetChilds()) {
            smgr->RemoveChild(c.get());
        }
        RemoveLightsAndCameras();
    }
    sysGrassVector.clear();
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
void CLEngine::RenderDepthMap(CLShadowMapping& shadowMap, CLResourceShader* depthShader, const glm::mat4& lightSpaceMatrix){
    // crear las matrices de transformacion del cubemap
    //float aspect = (float)shadowMap.SHADOW_WIDTH/(float)shadowMap.SHADOW_HEIGHT;
    //float near = 1.0f;
    //float far = Constants::FAR_PLANE;
    //glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far); 
    //// view Matrix
    //std::vector<glm::mat4> shadowTransforms;
    //shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    //shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    //shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    //shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    //shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    //shadowTransforms.push_back(shadowProj * glm::lookAt(posLight, posLight + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));


    //glUseProgram(simpleDepthShader);
    //glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    //glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader, "model"), 1, GL_FALSE, glm::value_ptr(shadowMap->));

    glUseProgram(depthShader->GetProgramID());

    // renderizar escena para el depth cubemap
    glViewport(0, 0, shadowMap.SHADOW_WIDTH,  shadowMap.SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER,  shadowMap.depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    //for (unsigned int i = 0; i < 6; ++i){
    //    string name = "shadowMatrices[" + std::to_string(i) + "]";
    //    glUniformMatrix4fv(glGetUniformLocation(depthShader->GetProgramID(), name.c_str()), 1, GL_FALSE, &(shadowTransforms[i])[0][0]);
    //}
    //glUniform1f(glGetUniformLocation(depthShader->GetProgramID(), "far_plane"), far); 
    //glUniform3fv(glGetUniformLocation(depthShader->GetProgramID(), "lightPos"), 1, &posLight[0]); 
    
    
    smgr->DFSTree(glm::mat4(1.0f), GetActiveCamera(), depthShader->GetProgramID(), lightSpaceMatrix);
        //shadowMap.Draw(simpleDepthShader);
        //RenderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



//Methods
void CLEngine::AddShader(const string vertex, const string fragment){
    shaders.emplace_back(resourceManager->GetResourceShader(vertex, fragment)->GetProgramID());
    
}

void CLEngine::AddShader(const string vertex, const string fragment, const string geometry){
    shaders.emplace_back(resourceManager->GetResourceShader(vertex, fragment,geometry)->GetProgramID());
    
}

CLNode* CLEngine::AddGroup(CLNode* parent,unsigned int id){
    shared_ptr<CLNode> node = make_shared<CLNode>();
    parent->AddChild(node);

    return node.get();
}

CLNode* CLEngine::AddMesh(CLNode* parent,unsigned int id){
    
    shared_ptr<CLEntity> e = make_shared<CLMesh>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);


    return node.get();
}

CLNode* CLEngine::AddMesh(CLNode* parent,unsigned int id,string mesh){
    auto node = AddMesh(parent, id);
    auto resourceMesh = CLResourceManager::GetResourceManager()->GetResourceMesh(mesh);
    static_cast<CLMesh*>(node->GetEntity())->SetMesh(resourceMesh);

    return node;
}

CLNode* CLEngine::AddPointLight(CLNode* parent,unsigned int id){
    
    shared_ptr<CLEntity> e = make_shared<CLPointLight>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    pointLights.push_back(node.get());
    return node.get();
}

CLNode* CLEngine::AddPointLight(CLNode* parent,unsigned int id,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
    auto node = AddPointLight(parent, id);
    static_cast<CLPointLight*>(node->GetEntity())->SetLightAttributes(intensity,ambient,diffuse,specular,constant,linear,quadratic);
    return node;
}

CLNode* CLEngine::AddDirectLight(CLNode* parent,unsigned int id){
    
    shared_ptr<CLEntity> e = make_shared<CLDirectLight>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    directLights.push_back(node.get());
    return node.get();
}

CLNode* CLEngine::AddDirectLight(CLNode* parent,unsigned int id,glm::vec3 direction,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
    auto node = AddDirectLight(parent, id);
    static_cast<CLDirectLight*>(node->GetEntity())->SetLightAttributes(direction,intensity,ambient,diffuse,specular,constant,linear,quadratic);
    return node;
}

CLNode* CLEngine::AddSpotLight(CLNode* parent,unsigned int id){
    
    shared_ptr<CLEntity> e = make_shared<CLSpotLight>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    spotLights.push_back(node.get());

    return node.get();
}

CLNode* CLEngine::AddSpotLight(CLNode* parent,unsigned int id,glm::vec3 direction,float cutOff,float outerCutOff,glm::vec3 intensity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic){
    auto node = AddSpotLight(parent, id);
    static_cast<CLSpotLight*>(node->GetEntity())->SetLightAttributes(direction,cutOff,outerCutOff,intensity,ambient,diffuse,specular,constant,linear,quadratic);
    
    return node;
}

CLNode* CLEngine::AddCamera(CLNode* parent,unsigned int id){
   
    shared_ptr<CLEntity> e = make_shared<CLCamera>(id);
    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    cameras.push_back(node.get());

    return node.get(); 
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id, unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/particleSystem.vert", "CLEngine/src/Shaders/particleSystem.frag","CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);
    }


    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }
    node->SetIgnoreFrustrum(true);
    return node.get();
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id,unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,glm::vec3 offset, glm::vec3 orientation, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/particleSystem.vert", "CLEngine/src/Shaders/particleSystem.frag","CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);

    }


    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,offset,orientation,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }
    node->SetIgnoreFrustrum(true);
    return node.get();
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id,unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,float radious, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/particleSystem.vert", "CLEngine/src/Shaders/particleSystem.frag","CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);

    }

    

    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,radious,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }
    node->SetIgnoreFrustrum(true);

    return node.get();
}

CLNode* CLEngine::AddParticleSystem(CLNode* parent,unsigned int id,unsigned int nParticles,glm::vec3 velocity,vector<string> textures,int width,int height,int spawnDelay,int particlesToSpawn,int lifeSpan,float radious,glm::vec3 orientation, std::uint_fast8_t flags){
    if(particleSystemShader == 0){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/particleSystem.vert", "CLEngine/src/Shaders/particleSystem.frag","CLEngine/src/Shaders/particleSystem.geom");
        particleSystemShader = resourceShader->GetProgramID();
        shaders.push_back(particleSystemShader);

    }

    

    shared_ptr<CLEntity> e = make_shared<CLParticleSystem>(id,nParticles,velocity,textures,width,height,spawnDelay,particlesToSpawn,lifeSpan,radious,orientation,flags);

    shared_ptr<CLNode> node = make_shared<CLNode>(e);
    parent->AddChild(node);
    node->SetShaderProgramID(particleSystemShader);

    //Configuraciones especificas de un particlesystem
    if(auto particleSystem = dynamic_cast<CLParticleSystem*>(e.get())){
        particleSystem->SetCLNode(node.get());
    }
    node->SetIgnoreFrustrum(true);
    return node.get();
}


void CLEngine::AddSkybox(string right, string left, string top, string bottom, string front, string back){
    if(!skyboxShader){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/skybox.vert", "CLEngine/src/Shaders/skybox.frag");
        skyboxShader = resourceShader->GetProgramID();
        shaders.push_back(skyboxShader);

    }
    skybox = make_unique<CLSkybox>(right, left, top, bottom, front, back);
    
}

void CLEngine::AddShadowMapping(GLuint lightId){
    if(!simpleDepthShader){
        auto rm = CLResourceManager::GetResourceManager();
        depthShadder = rm->GetResourceShader("CLEngine/src/Shaders/simpleDepthShader.vert", "CLEngine/src/Shaders/simpleDepthShader.frag"/*, "CLEngine/src/Shaders/simpleDepthShader.geom"*/);
        simpleDepthShader = depthShadder->GetProgramID();
        shaders.push_back(simpleDepthShader);
        
    }
    shadowMapping = make_unique<CLShadowMapping>(lightId);
}

CLNode* CLEngine::AddBillBoard(CLNode* parent,unsigned int id,string& file, bool vertically, float width_, float height_){
    auto rm = CLResourceManager::GetResourceManager();
    if(!billboardShader){
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/billboard.vert", "CLEngine/src/Shaders/billboard.frag", "CLEngine/src/Shaders/billboard.geom");
        billboardShader = resourceShader->GetProgramID();
        shaders.push_back(billboardShader);
        
    }

    

    CLResourceTexture* texture = rm->GetResourceTexture(file, vertically);
    auto entity = make_shared<CLBillboard>(id,texture,width_,height_);
    auto node = make_shared<CLNode>(entity);

    parent->AddChild(node);
    node->SetShaderProgramID(billboardShader);


    return node.get();
}

void CLEngine::AddGrass(float _width, float _height, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass){
    if(!grassShader){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/grassShader.vert", "CLEngine/src/Shaders/grassShader.frag", "CLEngine/src/Shaders/grassShader.geom");
        grassShader = resourceShader->GetProgramID();
        shaders.push_back(grassShader);
    }
    sysGrassVector.emplace_back(make_unique<CLGrassSystem>(_width, _height, _position, _scale, realistGrass));
}

void CLEngine::AddGrass(float radious, const glm::vec3& _position, const glm::vec3& _scale, bool realistGrass){
    if(!grassShader){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/grassShader.vert", "CLEngine/src/Shaders/grassShader.frag", "CLEngine/src/Shaders/grassShader.geom");
        grassShader = resourceShader->GetProgramID();
        shaders.push_back(grassShader);
    }
    sysGrassVector.emplace_back(make_unique<CLGrassSystem>(radious, _position, _scale, realistGrass));
}

bool CLEngine::RemoveChild(CLNode* child){
    return smgr->RemoveChild(child);
}

bool CLEngine::HasChild(CLNode* child){
    return smgr->HasChild(child);
}

CLNode* CLEngine::GetNodeByID(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, smgr.get());
    return node;
}


CLNode* CLEngine::GetNodeByIDAux(unsigned int id, CLNode* node, CLNode* root){
 
    if(node!=nullptr) return node; //Caso base, ha encontrado ya al nodo que busca
    if(root->GetChilds().size()>0){
        //Tiene hijos
        for(auto& nodo : root->GetChilds()){
            if(nodo->GetEntity() && nodo->GetEntity()->GetID() == id){
                node = nodo.get();
                return node;
            }else{
                node = GetNodeByIDAux(id, node, nodo.get());

            }
        }

    }

    return node;
}


// devolver datos de la camara
float CLEngine::GetFovActualCamera(){
    return static_cast<CLCamera*>(GetActiveCamera())->GetCameraFov();
}
glm::vec3 CLEngine::GetTargetActualCamera(){
    return static_cast<CLCamera*>(GetActiveCamera())->GetCameraTarget();
}
glm::vec3 CLEngine::GetPositionActualCamera(){
    return GetActiveCameraNode()->GetGlobalTranslation();
}

float CLEngine::GetBoundingSizeById(unsigned int id){
    CLNode* node = GetNodeByID(id);
    return node->CalculateBoundingBox();
}

void CLEngine::SetParticlesVisibility(bool mode){
    smgr->SetParticlesActivated(mode);
}


// Comprueba si el cubo del octree se ve en la camara del jugador
bool CLEngine::OctreeIncamera(float size, const glm::vec3& pos){
    glm::vec3 pos2 = glm::vec3(pos.x, pos.y, -pos.z);
    glm::vec3 pointBox[]{
        {pos2 - glm::vec3(size, size, size)},
        {pos2 - glm::vec3(size, size, -size)},
        {pos2 - glm::vec3(size, -size, size)},
        {pos2 - glm::vec3(size, -size, -size)},
        {pos2 - glm::vec3(-size, size, size)},
        {pos2 - glm::vec3(-size, size, -size)},
        {pos2 - glm::vec3(-size, -size, size)},
        {pos2 - glm::vec3(-size, -size, -size)}
    };

    CLCamera* camera = GetActiveCamera();
    CLNode* nodeCam = GetNodeByID(camera->GetID());

    float vectorToTargetX = camera->GetCameraTarget().x - nodeCam->GetGlobalTranslation().x;
    float vectorToTargetZ = camera->GetCameraTarget().z - nodeCam->GetGlobalTranslation().z;
    float valueCentralDegree = glm::degrees( atan2(vectorToTargetZ, vectorToTargetX) );
    if (valueCentralDegree < 0) valueCentralDegree += 360;

    float minVision = valueCentralDegree - camera->GetCameraFov();
    if (minVision < 0) minVision += 360;

    float maxVision = valueCentralDegree + camera->GetCameraFov();
    if (maxVision >= 360) maxVision -= 360;

    // comprobamos si algun punto del cubo se encuentra dentro de la camara
    for(unsigned int i=0; i<8; i++){
        float vectorToBoxX = pointBox[i].x - nodeCam->GetGlobalTranslation().x;
        float vectorToBoxZ = pointBox[i].z - nodeCam->GetGlobalTranslation().z;
        float valueDegreeBox = glm::degrees( atan2(vectorToBoxZ, vectorToBoxX) );
        if (valueDegreeBox < 0) valueDegreeBox += 360;

        if(minVision>maxVision){
            if(minVision<valueDegreeBox || maxVision>valueDegreeBox)
                return true;
        }else{
            if(minVision<valueDegreeBox && maxVision>valueDegreeBox)
                return true;
        }
    }

    // no se ve en caso que que ningun punto este dentro de la camara
    return false;


    /*
    glm::vec3 pos2 = glm::vec3(pos.x, pos.y, -pos.z);
    CalculateViewProjMatrix();
    auto& frustum_m = GetActiveCamera()->GetFrustum();
    CLE::CLFrustum::Visibility frusVisibility = frustum_m.IsInsideSomewhere(pos2, size);
    if(frusVisibility == CLE::CLFrustum::Visibility::Invisible)
       return false;
    else
       return true;
    */
}



bool CLEngine::DeleteNode(unsigned int id){
    CLNode* node = nullptr;
    node = GetNodeByIDAux(id, node, smgr.get());
    if(!node) return false;
    auto father = node->GetFather();
    father->RemoveChild(node);
    return true;
}

bool CLEngine::DeleteNode(CLNode* node){
    if(!node) return false;
    auto father = node->GetFather();
    father->RemoveChild(node);
    return true;
}


void CLEngine::DrawSkybox(){
    if(skybox.get()){
        glDepthMask(GL_FALSE);
        glUseProgram(skyboxShader);

        glm::mat4 view2 = glm::mat4(glm::mat3(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(view2));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        skybox->Draw(skyboxShader);
    }
}

void CLEngine::DrawGrass(){
    for(const auto& sysGrass : sysGrassVector){
        // frustum
        CLE::CLFrustum::Visibility frusVisibility = CLE::CLFrustum::Visibility::Invisible;
        auto& frustrum_m = GetActiveCamera()->GetFrustum();
        frusVisibility = frustrum_m.IsInside(sysGrass->GetPosition(), sysGrass->GetSize());
        
        if(frusVisibility == CLE::CLFrustum::Visibility::Completly && glm::distance(sysGrass->GetPosition(), GetActiveCameraNode()->GetGlobalTranslation()) < 1200.0)
            sysGrass->Draw(grassShader, projection, view);
    }
}


CLCamera* CLEngine::GetActiveCamera(){
    for(auto camera : cameras){
        auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
        if(entityCamera->IsActive()){
            return entityCamera;
        }
    }
    return nullptr;
}

CLNode* CLEngine::GetActiveCameraNode(){
    for(auto camera : cameras){
        auto entityCamera = static_cast<CLCamera*>(camera->GetEntity());
        if(entityCamera->IsActive()){
            return camera;
        }
    }
    return nullptr;
}


const void CLEngine::Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
    Draw3DLine(x1,y1,z1,x1,y2,z1,CLColor(255.0,0.0,0.0,255.0));
}

const void CLEngine::Draw3DLine(float x1, float y1, float z1, float x2, float y2, float z2,CLColor color) const{

    if(!debugShader){
        auto rm = CLResourceManager::GetResourceManager();
        auto resourceShader = rm->GetResourceShader("CLEngine/src/Shaders/debugShader.vert", "CLEngine/src/Shaders/debugShader.frag");
        debugShader = resourceShader->GetProgramID();
    }
    float line[] = {
        x1, y1, z1,
        x2, y2, z2
    };

    
    //glEnable(GL_LINE_SMOOTH);
    glLineWidth(lineWidth);
    //glHint(GL_LINE_SMOOTH_HINT,  GL_NICEST);

    unsigned int VBOLine, VAOLine;
    glGenBuffers(1, &VBOLine);
    glGenVertexArrays(1, &VAOLine);
    glBindVertexArray(VAOLine);
    glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), 0);
    glBindVertexArray(0);


    glUseProgram(debugShader);

    glm::vec4 clcolor(color.GetRedNormalized(),color.GetGreenNormalized(),color.GetBlueNormalized(),color.GetAlphaNormalized());
    glUniformMatrix4fv(glGetUniformLocation(debugShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(debugShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4fv(glGetUniformLocation(debugShader, "clcolor"),1, glm::value_ptr(clcolor));

    glBindVertexArray(VAOLine);
    glDrawArrays(GL_LINE_LOOP, 0,2); 
    glUseProgram(0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAOLine);
    glDeleteBuffers(1, &VBOLine);

} 

// Mi idea ahora es pasarle todas las luces a todos los shaders que las vayan a usar
void CLEngine::CalculateLights(){
    GLuint i = 0;

    for(auto shader : shaders){
        glUseProgram(shader);
        for(auto pointLight : pointLights){
            auto pointLightEntity = static_cast<CLPointLight*>(pointLight->GetEntity());
            
            string number = to_string(i); 
            //Tenemos que mirar que luz es la del shadowmapping
            if(shadowMapping && pointLight->GetEntity()->GetID() == shadowMapping->GetID()){
                glUniform1i(glGetUniformLocation(shader,"id_luz_shadowMapping"),i);    

            }
            glUniform1i(glGetUniformLocation(shader,"num_Point_Lights"),pointLights.size());    
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].position").c_str()),1,glm::value_ptr(pointLight->GetGlobalTranslation()));
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].ambient").c_str()), 1,glm::value_ptr(pointLightEntity->GetAmbient()));
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(pointLightEntity->GetDiffuse()));
            glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].specular").c_str()), 1, glm::value_ptr(pointLightEntity->GetSpecular()));
            glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].constant").c_str()), pointLightEntity->GetConstant());
            glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].linear").c_str()), pointLightEntity->GetLinear());
            glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].quadratic").c_str()), pointLightEntity->GetQuadratic());


            i++;
        }

        i = 0;
        for(auto directLight : directLights){
            auto directLightEntity = static_cast<CLDirectLight*>(directLight->GetEntity());
            
            string number = to_string(i); 
            
            glUniform1i(glGetUniformLocation(shader,"num_Direct_Lights"),directLights.size());    
            glUniform3fv(glGetUniformLocation(shader, ("directLights[" + number + "].position").c_str()),1,glm::value_ptr(directLight->GetGlobalTranslation()));
            glUniform3fv(glGetUniformLocation(shader, ("directLights[" + number + "].direction").c_str()),1,glm::value_ptr(directLightEntity->GetDirection()));
            glUniform3fv(glGetUniformLocation(shader, ("directLights[" + number + "].ambient").c_str()), 1,glm::value_ptr(directLightEntity->GetAmbient()));
            glUniform3fv(glGetUniformLocation(shader, ("directLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(directLightEntity->GetDiffuse()));
            glUniform3fv(glGetUniformLocation(shader, ("directLights[" + number + "].specular").c_str()), 1, glm::value_ptr(directLightEntity->GetSpecular()));
            glUniform1f(glGetUniformLocation(shader, ("directLights[" + number + "].constant").c_str()), directLightEntity->GetConstant());
            glUniform1f(glGetUniformLocation(shader, ("directLights[" + number + "].linear").c_str()), directLightEntity->GetLinear());
            glUniform1f(glGetUniformLocation(shader, ("directLights[" + number + "].quadratic").c_str()), directLightEntity->GetQuadratic());


            i++;
        }

        i = 0;
        for(auto spotLight : spotLights){
            auto spotLightEntity = static_cast<CLSpotLight*>(spotLight->GetEntity());
            
            string number = to_string(i); 

            glUniform1i(glGetUniformLocation(shader,"num_Spot_Lights"),spotLights.size());  
            glUniform3fv(glGetUniformLocation(shader, ("spotLights[" + number + "].position").c_str()),1,glm::value_ptr(spotLight->GetGlobalTranslation()));
            glUniform3fv(glGetUniformLocation(shader, ("spotLights[" + number + "].direction").c_str()),1,glm::value_ptr(spotLightEntity->GetDirection()));
            glUniform1f(glGetUniformLocation(shader, ("spotLights[" + number + "].cutOff").c_str()),spotLightEntity->GetCutOff());
            glUniform1f(glGetUniformLocation(shader, ("spotLights[" + number + "].outerCutOff").c_str()),spotLightEntity->GetOuterCutOff());
            glUniform3fv(glGetUniformLocation(shader, ("spotLights[" + number + "].ambient").c_str()), 1,glm::value_ptr(spotLightEntity->GetAmbient()));
            glUniform3fv(glGetUniformLocation(shader, ("spotLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(spotLightEntity->GetDiffuse()));
            glUniform3fv(glGetUniformLocation(shader, ("spotLights[" + number + "].specular").c_str()), 1, glm::value_ptr(spotLightEntity->GetSpecular()));
            glUniform1f(glGetUniformLocation(shader, ("spotLights[" + number + "].constant").c_str()), spotLightEntity->GetConstant());
            glUniform1f(glGetUniformLocation(shader, ("spotLights[" + number + "].linear").c_str()), spotLightEntity->GetLinear());
            glUniform1f(glGetUniformLocation(shader, ("spotLights[" + number + "].quadratic").c_str()), spotLightEntity->GetQuadratic());


            i++;
        }
    }
    
}

void CLEngine::RemoveLightsAndCameras() {
    cameras.clear();
    pointLights.clear();
    shadowMapping = nullptr;
    skybox = nullptr;
}


void CLEngine::CalculateViewProjMatrix(const glm::mat4& lightSpaceMatrix){
    auto cam = GetActiveCameraNode();
    auto entityCamera = GetActiveCamera();

    projection = entityCamera->CalculateProjectionMatrix();
    view = glm::lookAt(cam->GetGlobalTranslation(),entityCamera->GetCameraTarget(),entityCamera->GetCameraUp());
    glm::mat4 viewProjection = projection*view;
    glm::vec3 camPos = GetActiveCameraNode()->GetGlobalTranslation();

    glm::vec3 directionShadowLight(0.0);
    if(shadowMapping && shadowsActivate){
        auto light = GetNodeByID(GetShadowMapping()->GetID());
        directionShadowLight = glm::normalize(light->GetGlobalTranslation() - glm::vec3(0.0));
    }

    for(auto shader : shaders){
        glUseProgram(shader);

        glUniform3fv(glGetUniformLocation(shader,"viewPos"),1,glm::value_ptr(cam->GetGlobalTranslation()));
        glUniformMatrix4fv(glGetUniformLocation(shader, "VPMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjection));
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, glm::value_ptr(camPos));
        glUniform1i(glGetUniformLocation(shader, "activeShadows"), shadowsActivate); 
        //glUniform1f(glGetUniformLocation(shader, "far_plane"), Constants::FAR_PLANE);
        glUniformMatrix4fv(glGetUniformLocation(shader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform3fv(glGetUniformLocation(shader, "lightShadowDir"), 1, glm::value_ptr(directionShadowLight));
    }

    

    //auto modelView = /*cam->GetTransformationMat() **/ view;
    entityCamera->CalculateFrustum(projection,view);
}


void CLEngine::SetOctreeVisibleById(unsigned int id, bool v){
    CLNode* node = GetNodeByID(id);
    node->SetOctreeVisible(v);
}