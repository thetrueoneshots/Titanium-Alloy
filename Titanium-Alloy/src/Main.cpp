// Framework include
#include "Voxel/Voxel.h"

// GLM include for vectors and matrices
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Constants for the screensize
const int g_Width = 1280;
const int g_Height = 720;
const char* g_Title = "Engine window";

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
Game g_Game;

// Callback function definitions (needed, because otherwise we would not be able to assign them
// with window->SetCallback )
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

int main(void)
{
    static glm::ivec2 size = glm::ivec2(g_Width, g_Height);

    // Window creation
    g_Game.window = new Voxel::Window(g_Width, g_Height, g_Title);

    // Callback functions
    g_Game.window->SetCallback(Voxel::Window::CallbackType::ERROR, error_callback);
    g_Game.window->SetCallback(Voxel::Window::CallbackType::KEY, key_callback);
    g_Game.window->SetCallback(Voxel::Window::CallbackType::SCROLL, scroll_callback);
    g_Game.window->SetCallback(Voxel::Window::CallbackType::CURSOR, cursor_callback);

    // Camera creation
    g_Game.camera = new Voxel::Camera(&size, 500.0f);
    g_Game.camera->SetPosition(glm::vec3(5, 5, 15));

    // Renderer creation
    g_Game.renderer = new Voxel::Renderer(g_Game.camera);
    g_Game.renderer->Init();

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

    Voxel::Animation a(*m->GetTransForm(), 5.0f);
    Voxel::Transform temp = *m->GetTransForm();
    temp.Translate(glm::vec3(0, 10, 0));
    temp.SetScale(0.01f);
    temp.SetRotation(glm::vec3(glm::radians(180.0f), glm::radians(90.0f), glm::radians(360.0f)));
    a.InsertFrame({ 1.0f, temp });

    temp.Translate(glm::vec3(10, 0, 10));
    temp.SetScale(0.05f);
    temp.SetRotation(glm::vec3(0, 0, 0));
    a.InsertFrame({ 2.0f, temp });

    temp.Translate(glm::vec3(-10, 10, 10));
    a.InsertFrame({ 3.0f, temp });
    a.Play();

    // Testing out cub files
    std::vector<std::string> files = {
        "plants/corn.cub",
        //"plants/cactus1.cub",  // Bugged for some reason, adding too many blocks to the mesh
        //"plants/buckhorn.cub", // Bugged for some reason, not adding any blocks to the mesh
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
    while (!g_Game.window->ShouldClose())
    {
        // Frametime and fps calculation
        static int count = 0;
        static double time = 0.0f;

        double curr_time = g_Game.window->GetTime();
        double frametime = curr_time - g_Game.last_time;
        g_Game.last_time = curr_time;

        time += frametime;
        count++;
    
        if (time > 1.0f)
        {
            // Showing fps on the screen
            char buffer[250];
            sprintf_s(buffer, 250, "%s, Fps: %d", g_Title, count);
            g_Game.window->SetTitle(buffer);

            time = 0.0f;
            count = 0;
        }
        a.UpdateTime(frametime);
        Voxel::Transform t = a.GetTransform();
        m->GetTransForm()->SetRotation(t.GetRotation());
        m->GetTransForm()->SetTranslation(t.GetTranslation());
        m->GetTransForm()->SetScale(t.GetScale());

        // Clearing the screen and updating the renderer
        g_Game.window->Clear();
        g_Game.renderer->Update();

        // Render/Draw the mesh
        g_Game.renderer->Render(m);

        for (const auto& mesh : meshes)
        {
            g_Game.renderer->Render(mesh);
        }

        // Updating the window with the drawn screen
        g_Game.window->Update();
    }

    // Cleanup meshes
    delete m;
    for (const auto& mesh : meshes)
    {
        delete mesh;
    }

    // Cleanup framework
    delete g_Game.camera;
    delete g_Game.renderer;
    delete g_Game.window;
    exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        g_Game.window->Close();

    float cameraSpeed = 0.1f;
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
        g_Game.camera->Move(Voxel::Camera::Direction::LEFT, cameraSpeed);
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
        g_Game.camera->Move(Voxel::Camera::Direction::RIGHT, cameraSpeed);
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
        g_Game.camera->Move(Voxel::Camera::Direction::FORWARD, cameraSpeed);
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
        g_Game.camera->Move(Voxel::Camera::Direction::BACKWARD, cameraSpeed);
    if (key == GLFW_KEY_LEFT_SHIFT)
        g_Game.camera->Move(Voxel::Camera::Direction::DOWN, cameraSpeed);
    if (key == GLFW_KEY_SPACE)
        g_Game.camera->Move(Voxel::Camera::Direction::UP, cameraSpeed);
}

void cursor_callback(GLFWwindow* w, double xPos, double yPos)
{
    const static float s_Sensitivity = 0.1f;
    if (g_Game.first_mouse)
    {
        g_Game.last_mouse = glm::vec2(xPos, yPos);
        g_Game.first_mouse = false;
    }

    glm::vec2 offset = glm::vec2(xPos - g_Game.last_mouse.x, g_Game.last_mouse.y - yPos); // Y is reversed. (Y goes from top to bottom)
    g_Game.last_mouse = glm::vec2(xPos, yPos);

    offset *= s_Sensitivity;

    g_Game.camera->Mouse(offset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    g_Game.camera->Scroll(yOffset);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}