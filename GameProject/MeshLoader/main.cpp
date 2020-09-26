
// Std. Includes
#include <string>
#include <cstdlib>
#include <algorithm>
#include <random>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "GameObject.h"
#include "Player.h"
#include "Object.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <GLUT/GLUT.h>

//#include <ft2build.h>
//#include FT_FREETYPE_H

#define GLFW_KEY_SPACE 32
#define CARROT_AMOUNT 50
#define OBSTACLE_AMOUNT 50


// Properties
const GLuint WIDTH = 500, HEIGHT = 500;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
//void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement(Player &player);
void DoLightStuff(const Shader &shader, Player player, float brightness);


//GLfloat x = 0.0f, y = 0.0f, z = 30.0f;
GLfloat x = 0.0f, y = 14.0f, z = 40.0f;

// Camera
Camera camera( glm::vec3( x, y, z ) );

//glm::vec3 lightPos( 1.2f, 1.0f, 2.0f );


bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
enum Difficulty{
    EASY,
    NORMAL,
    HARD,
    OMG
};

Difficulty difficulty(EASY);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
//    //---------------------Freetype--------------------
//    FT_Library ft;
//    if (FT_Init_FreeType(&ft))
//        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//
//    FT_Face face;
//    if (FT_New_Face(ft, "resources/arial.ttf", 0, &face))
//        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//
//    FT_Set_Pixel_Sizes(face, 0, 48);
//
//    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
//        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//
//    // Disable byte-alignment restriction
//       glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//       // Load first 128 characters of ASCII set
//       for (GLubyte c = 0; c < 128; c++)
//       {
//           // Load character glyph
//           if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//           {
//               std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//               continue;
//           }
//           // Generate texture
//           GLuint texture;
//           glGenTextures(1, &texture);
//           glBindTexture(GL_TEXTURE_2D, texture);
//           glTexImage2D(
//               GL_TEXTURE_2D,
//               0,
//               GL_RED,
//               face->glyph->bitmap.width,
//               face->glyph->bitmap.rows,
//               0,
//               GL_RED,
//               GL_UNSIGNED_BYTE,
//               face->glyph->bitmap.buffer
//           );
//           // Set texture options
//           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//           // Now store character for later use
//           Character character = {
//               texture,
//               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//               GLuint(face->glyph->advance.x)
//           };
//           Characters.insert(std::pair<GLchar, Character>(c, character));
//       }
//       glBindTexture(GL_TEXTURE_2D, 0);
//       // Destroy FreeType once we're finished
//       FT_Done_Face(face);
//       FT_Done_FreeType(ft);

       /*
       // Configure VAO/VBO for texture quads
       glGenVertexArrays(1, &VAO);
       glGenBuffers(1, &VBO);
       glBindVertexArray(VAO);
       glBindBuffer(GL_ARRAY_BUFFER, VBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
       glBindBuffer(GL_ARRAY_BUFFER, 0);
       glBindVertexArray(0);
    */
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
//    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "resources/shaders/modelLoading.vs", "resources/shaders/modelLoading.frag" );
  
    Shader shader2( "resources/shaders/core.vs", "resources/shaders/core.frag" );
    
    std::vector<vector<float> > object_positions;
    if(difficulty == EASY){
        for(int i = 0; i < CARROT_AMOUNT+OBSTACLE_AMOUNT; i++){
           float rx = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/6)), ry = -0.25, rz = -200 + i*9;//-80 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/80));
           vector<float> pos;
           pos.push_back(rx);
           pos.push_back(ry);
           pos.push_back(rz);
           object_positions.push_back(pos);
       }
    }else if(difficulty == NORMAL){
        for(int i = 0; i < CARROT_AMOUNT+OBSTACLE_AMOUNT; i++){
            float rx = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/6)), ry = -0.25, rz = -200 + i*7;//-80 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/80));
            vector<float> pos;
            pos.push_back(rx);
            pos.push_back(ry);
            pos.push_back(rz);
            object_positions.push_back(pos);
        }
    }else if(difficulty == HARD){
        for(int i = 0; i < CARROT_AMOUNT+OBSTACLE_AMOUNT; i++){
            float rx = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/6)), ry = -0.25, rz = -200 + i*5;//-80 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/80));
            vector<float> pos;
            pos.push_back(rx);
            pos.push_back(ry);
            pos.push_back(rz);
            cout << rz << endl;
            object_positions.push_back(pos);
        }
    }else if(difficulty == OMG){
        for(int i = 0; i < CARROT_AMOUNT+OBSTACLE_AMOUNT; i++){
            float rx = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/6)), ry = -0.25, rz = -200 + i*3;//-80 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/80));
            vector<float> pos;
            pos.push_back(rx);
            pos.push_back(ry);
            pos.push_back(rz);
            object_positions.push_back(pos);
        }
    }
    
    // Load models
//    Model ourModel( "resources/models/model1/nanosuit.obj" );
    Model ourModel("resources/models/model3/Grass_Block.obj");
    Model carrotModel("resources/models/model2/Carrot.obj");
    Model crateModel("resources/models/model5/Cube.obj");
    Model obstacleModel("resources/models/model2/Poplar_Tree.obj");
//    Model enemyModel("resources/models/model6/zombie_hand.obj");
    
    
    //-----------------------------------------------------------------------
    Object *obj1 = new Object(ourModel, glm::vec3( 3.1f, 0.1f,200.f ),true,true);
    
    Player *player = new Player(crateModel, glm::vec3(0.006f, 0.006f, 0.006f));
    vector<Object> objects;
    int real_carrot_amount=0, real_object_amount=0;
    for(int i = 0; i < CARROT_AMOUNT + OBSTACLE_AMOUNT; i++){
        int j = rand()%2;
        if(j == 0){
            Object carrot(carrotModel, glm::vec3( 0.5f, 0.5f, 0.5f ),false);
            objects.push_back(carrot);
            real_carrot_amount++;
        }
        else if(j == 1){
            Object obs(obstacleModel, glm::vec3( 0.5f, 0.5f, 0.5f ));
            objects.push_back(obs);
            real_object_amount++;
        }
    }
    cout << "real_object_amount=" << real_object_amount << "\nreal_carrot_amount=" << real_carrot_amount << endl;
    
    //-----------------------------------------------------------------------

    
    
    shader.use();
    glUniform1i( glGetUniformLocation( shader.Program, "material.diffuse" ), 0 );
    glUniform1i( glGetUniformLocation( shader.Program, "material.specular" ), 1 );
    
//    obstacles[0].Draw(shader, glm::vec3(0.0f,2.0f,0.0f));
    player->Draw(shader,glm::vec3(0.0f,1.0f,75.0f),camera.GetPosition());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    float near=1.f, far=50.f;
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, near, far );
//    glm::mat4 projection = glm::ortho( 0.0f, 800.0f, 0.0f, 600.0f );
    string title;
    float brightness=1.0f;
    // Game loop
    bool gameover = false, gamestart = false;
    while(!glfwWindowShouldClose(window)){
        title = "Score=" + to_string(player->GetScore()) + " Life=" + to_string(player->GetLife());
        glfwSetWindowTitle(window, title.c_str());
//         Set frame time
//        brightness-=0.001f;
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
//        cout << "DeltaTime=" << deltaTime << endl;
        // Check and call events
        glfwPollEvents();
        DoMovement(*player);
        glDepthMask(GL_TRUE);  // disable writes to Z-Buffer
        glEnable(GL_DEPTH_TEST);  // disable depth-testing
        if(currentFrame > 5){
            gamestart = true;
        }
        // Clear the colorbuffer
//        glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //-----------------------------------------------------------
        
        shader.use( );
        
        DoLightStuff(shader,*player,brightness);

        glm::mat4 view = camera.GetViewMatrix( );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr(obj1->GetTranslation()));

        // Draw the surface
        
        obj1->Draw(shader,glm::vec3( 0.0f, -0.5f, 0.0f ),camera.GetPosition());
        
//        glm::vec3 cam_pos = camera.GetPosition();
        glm::vec3 obj_pos = player->GetPosition();

        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr(player->GetTranslation()));
        //Draw the crate
        player->Draw(shader,obj_pos,camera.GetPosition());
        
//        obstacles[0].Draw(shader, glm::vec3(0.0f,0.0f,10.0f));
//        cout << objects.size() << endl;
        
        // Draw the loaded carrots
        for(int i = 0; i < CARROT_AMOUNT+OBSTACLE_AMOUNT; i++){
            glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr(objects[i].GetTranslation()));
            objects[i].Draw(shader, glm::vec3(object_positions[i][0], object_positions[i][1], object_positions[i][2]),camera.GetPosition());
        }
                
        vector<float> surface_mins = obj1->GetModel().GetMins();
//        if(gamestart){
            if(!gameover){
                if(player->GetLife()>-1){
                    player->Collect(objects);
                    player->RunPhysics(*obj1,deltaTime);
//                    player->Move(deltaTime);
                    camera.FollowObject(*player);
                    if(surface_mins[2]*obj1->GetSize().z + 5 > player->GetPosition().z){
                        gameover = true;
                    }
                }
            }
            else{
//                 if(player->GetPosition().z > obj1->GetPosition().z*obj1->GetSize().z)
                brightness -= 0.002;
            }
        if(gameover){
            brightness -=0.002;
        }
//        }
//        
        glPopMatrix();

        glfwSwapBuffers(window);
    }
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement(Player &player)
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] ){
        player.Move(GO_FORWARD,deltaTime);
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] ){
        player.Move(GO_BACKWARD,deltaTime);
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] ){
        if(player.GetPosition().x >= -2.8){
            player.Move(GO_LEFT, deltaTime);
        }
        player.setMovingL(true);
    }
    else{
        player.setMovingL(false);
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]){
        if(player.GetPosition().x <= 2.8){
            player.Move(GO_RIGHT, deltaTime);
        }
        player.setMovingR(true);
    }
    else{
        player.setMovingR(false);
    }
    
    if(keys[GLFW_KEY_SPACE]){
        player.Move(GO_UP, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( (key >= 0 && key < 1024) )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

void DoLightStuff(const Shader &shader, Player player,float brightness){
    GLint lightPosLoc = glGetUniformLocation( shader.Program, "light.position" );
    GLint lightSpotdirLoc = glGetUniformLocation( shader.Program, "light.direction" );
    GLint lightSpotCutOffLoc = glGetUniformLocation( shader.Program, "light.cutOff" );
    GLint lightSpotOuterCutOffLoc = glGetUniformLocation( shader.Program, "light.outerCutOff" );
    GLint viewPosLoc = glGetUniformLocation( shader.Program, "viewPos" );
//    glUniform3f( lightPosLoc, player->GetPosition( ).x, player->GetPosition( ).y, player->GetPosition( ).z);
    glUniform3f( lightPosLoc, player.GetPosition().x, player.GetPosition().y + 5, player.GetPosition().z);
    glUniform3f( lightSpotdirLoc, 0.0f, 5.0f, 0.0f);
    glUniform1f( lightSpotCutOffLoc, glm::cos( glm::radians( 90.0f ) ) );
    glUniform1f( lightSpotOuterCutOffLoc, glm::cos( glm::radians( 180.0f ) ) );
//    glUniform3f( viewPosLoc, player->GetPosition( ).x, player->GetPosition( ).y, player->GetPosition( ).z);
    glUniform3f( viewPosLoc, player.GetPosition().x, player.GetPosition().y, player.GetPosition().z);
        // Set lights properties
    glUniform3f( glGetUniformLocation( shader.Program, "light.ambient" ),   0.5f, 0.5f, 0.5f );
        // We set the diffuse intensity a bit higher; note that the right lighting conditions differ with each lighting method and environment.
        // Each environment and lighting type requires some tweaking of these variables to get the best out of your environment.
        
    glUniform3f( glGetUniformLocation( shader.Program, "light.diffuse" ), 5.0f, 5.0f, 5.0f );
    glUniform3f( glGetUniformLocation( shader.Program, "light.specular" ), 5.0f, 5.0f, 5.0f );
    glUniform1f( glGetUniformLocation( shader.Program, "light.constant" ), 1.0f );
    glUniform1f( glGetUniformLocation( shader.Program, "light.linear" ), 0.09 );
    glUniform1f( glGetUniformLocation( shader.Program, "light.quadratic" ), 0.0042 ); //0.032
        // Set material properties
    glUniform1f( glGetUniformLocation( shader.Program, "material.shininess" ), 2.0f );
    glUniform2f( glGetUniformLocation( shader.Program, "brightness" ), brightness , 0.0f );

}


void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        
        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 16);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
