// Framework include
#include "Voxel/Voxel.h"

// GLM include for vectors and matrices
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Constants for the screensize
const int WIDTH = 1280;
const int HEIGHT = 720;

// Game structure definition, could be moved to main.h
struct Game
{
    Voxel::Window* window;
    Voxel::Camera* camera;
    Voxel::Renderer* renderer;
    bool first_mouse = false;
    glm::vec2 last_mouse = glm::vec2(0, 0);
};


// Game variable containing all variables needed to run the game
Game game;

// Callback function definitions (needed, because otherwise we would not be able to assign them
// with window->SetCallback )
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

int main(void)
{
    static glm::ivec2 size = glm::ivec2(WIDTH, HEIGHT);

    // Window creation
    game.window = new Voxel::Window(WIDTH, HEIGHT, "Engine");

    // Callback functions
    game.window->SetCallback(Voxel::Window::CallbackType::ERROR, error_callback);
    game.window->SetCallback(Voxel::Window::CallbackType::KEY, key_callback);
    game.window->SetCallback(Voxel::Window::CallbackType::SCROLL, scroll_callback);
    game.window->SetCallback(Voxel::Window::CallbackType::CURSOR, cursor_callback);

    // Camera creation
    game.camera = new Voxel::Camera(&size, 500.0f);
    game.camera->SetPosition(glm::vec3(5, 5, 15));

    // Renderer creation
    game.renderer = new Voxel::Renderer(game.camera);
    game.renderer->Init();

    // Creating a mesh to draw with the renderer
    Voxel::Mesh* m = new Voxel::Mesh(10, 10, 10);
    m->GetTransForm()->Translate(glm::vec3(-10, -10, -10));
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                m->AddCube(glm::ivec3(i, j, k), glm::vec4(0.1f * i, 0.1f * j, 0.1f * k, 1.0f));
            }
        }
    }

    // Testing out cub files
    Voxel::Mesh* cub = Voxel::CubLoader::LoadMeshFromFile("plants/bush.cub");

    // Game loop
    while (!game.window->ShouldClose())
    {
        // Clearing the screen and updating the renderer
        game.window->Clear();
        game.renderer->Update();

        // Render/Draw the mesh
        game.renderer->Render(m, Voxel::RenderType::VOXEL);
        game.renderer->Render(cub, Voxel::RenderType::VOXEL);

        // Updating the window with the drawn screen
        game.window->Update();
    }

    // Cleanup.
    delete game.camera;
    delete game.renderer;
    delete game.window;
    exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        game.window->Close();

    float cameraSpeed = 0.1f;
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
        game.camera->Move(Voxel::Camera::Direction::LEFT, cameraSpeed);
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
        game.camera->Move(Voxel::Camera::Direction::RIGHT, cameraSpeed);
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
        game.camera->Move(Voxel::Camera::Direction::FORWARD, cameraSpeed);
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
        game.camera->Move(Voxel::Camera::Direction::BACKWARD, cameraSpeed);
    if (key == GLFW_KEY_LEFT_SHIFT)
        game.camera->Move(Voxel::Camera::Direction::DOWN, cameraSpeed);
    if (key == GLFW_KEY_SPACE)
        game.camera->Move(Voxel::Camera::Direction::UP, cameraSpeed);
}

void cursor_callback(GLFWwindow* w, double xPos, double yPos)
{
    const static float s_Sensitivity = 0.1f;
    if (game.first_mouse)
    {
        game.last_mouse = glm::vec2(xPos, yPos);
        game.first_mouse = false;
    }

    glm::vec2 offset = glm::vec2(xPos - game.last_mouse.x, game.last_mouse.y - yPos); // Y is reversed. (Y goes from top to bottom)
    game.last_mouse = glm::vec2(xPos, yPos);

    offset *= s_Sensitivity;

    game.camera->Mouse(offset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    game.camera->Scroll(yOffset);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}