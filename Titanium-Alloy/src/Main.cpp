// Framework include
#include "Voxel/Voxel.h"

// GLM include for vectors and matrices
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Constants for the screensize
const int WIDTH = 1280;
const int HEIGHT = 720;
const char* title = "Engine window";

// Game structure definition, could be moved to main.h
struct Game
{
    Voxel::Window* window = nullptr;
    Voxel::Camera* camera = nullptr;
    Voxel::Renderer* renderer = nullptr;
    bool first_mouse = true;
    double last_time = 0.0f;
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
    game.window = new Voxel::Window(WIDTH, HEIGHT, title);

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
                m->GetMeshData()->AddCube(glm::ivec3(i, j, k), glm::vec4(0.1f * i, 0.1f * j, 0.1f * k, 1.0f));
            }
        }
    }

    Voxel::Animation a(*m->GetTransForm(), 10.0f);
    Voxel::Transform temp = *m->GetTransForm();
    temp.Translate(glm::vec3(0, 10, 0));
    temp.SetScale(0.01f);
    temp.SetRotation(glm::vec3(glm::radians(45.0f), 0, 0));
    a.InsertFrame({ 5.0f, temp });
    a.Play();

    // Testing out cub files
    std::vector<std::string> files = {
        "plants/corn.cub",
        "plants/cactus1.cub",  // Bugged for some reason, adding too many blocks to the mesh
        "test_extre_width.cub",
        "test_extre_height.cub",
        "test_extre_depth.cub",
        "plants/buckhorn.cub", // Bugged for some reason, not adding any blocks to the mesh
        "plants/flower1.cub",
        "extra/barrel.cub",
        "extra/eternal-ember.cub",
        "plants/herb3.cub",
    };

    std::vector<Voxel::Mesh*> meshes;

    for (int i = 0; i < files.size(); i++)
    {
        Voxel::Mesh* cub = Voxel::CubLoader::LoadMeshFromFile(files.at(i));
        if (cub == nullptr)
        {
            continue;
        }
        cub->ScaleToSize(1.0f);
        cub->GetTransForm()->Translate(glm::vec3(3 * i, 0, 0));
        meshes.push_back(cub);
    }

    // Game loop
    while (!game.window->ShouldClose())
    {
        // Frametime and fps calculation
        static int count = 0;
        static double time = 0.0f;

        double curr_time = game.window->GetTime();
        double frametime = curr_time - game.last_time;
        game.last_time = curr_time;

        time += frametime;
        count++;
    
        if (time > 1.0f)
        {
            // Showing fps on the screen
            char buffer[250];
            sprintf_s(buffer, 250, "%s, Fps: %d", title, count);
            game.window->SetTitle(buffer);

            time = 0.0f;
            count = 0;
        }
        
        Voxel::Transform t = a.GetTransform(frametime);
        m->GetTransForm()->SetRotation(t.GetRotation());
        m->GetTransForm()->SetTranslation(t.GetTranslation());
        m->GetTransForm()->SetScale(t.GetScale());

        // Clearing the screen and updating the renderer
        game.window->Clear();
        game.renderer->Update();

        // Render/Draw the mesh
        game.renderer->Render(m, Voxel::RenderType::VOXEL);

        for (const auto& mesh : meshes)
        {
            game.renderer->Render(mesh, Voxel::RenderType::VOXEL);
        }

        // Updating the window with the drawn screen
        game.window->Update();
    }

    // Cleanup meshes
    delete m;
    for (const auto& mesh : meshes)
    {
        delete mesh;
    }

    // Cleanup framework
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