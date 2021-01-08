#include "Test.h"

void Test::Init()
{
	std::cout << "## Tic Tac Toe: Init." << std::endl;

    // Init variables
    m_FirstMouse = true;
    m_LastTime = 0.0f;
    m_LastMouse = glm::vec2(0, 0);

    // Setup camera position
    m_Camera->SetPosition(glm::vec3(1, 1, 7));


    // Insert all models
    m_Models.insert_or_assign((int)ModelType::CROSS, Voxel::CubLoader::LoadMeshFromFile("tic_tac_toe/cross.cub"));
    m_Models.insert_or_assign((int)ModelType::CIRCLE, Voxel::CubLoader::LoadMeshFromFile("tic_tac_toe/circle.cub"));
    m_Models.insert_or_assign((int)ModelType::LOSER, Voxel::CubLoader::LoadMeshFromFile("tic_tac_toe/lose.cub"));
    m_Models.insert_or_assign((int)ModelType::WINNER, Voxel::CubLoader::LoadMeshFromFile("tic_tac_toe/win.cub"));

    Voxel::MeshData* selector = new Voxel::MeshData(1, 1, 1);
    selector->AddCube(glm::vec3(0, 0, 0), glm::vec4(1, 1, 0, 1));
    m_Models.insert_or_assign((int)ModelType::SELECTOR, selector);

    // Create animation
    animation = new Voxel::Animation(5.0f);
    
    Voxel::Transform t;
    t.Rotate(glm::vec3(glm::radians(360.0f)));

    Voxel::AnimationFrame f = { 5.0f, t };
    animation->InsertFrame(f);
    animation->Play();
}

void Test::Cleanup()
{
	std::cout << "## Tic Tac Toe: Cleanup." << std::endl;

    for (const auto& pair : m_Models)
    {
        delete pair.second;
    }

    delete animation;
}

void Test::Update()
{
    static char buffer[250];
    Voxel::Mesh m(m_Models.at((int)ModelType::CIRCLE));
    m.ScaleToSize(1.0f);
    animation->UpdateTime(m_FrameTime);

    sprintf_s(buffer, 250, "%s, fps: %d", m_Title.c_str(), m_FPS);
    
    m_Window->SetTitle(buffer);

    m_Renderer->AnimatedRender(&m, animation);
}

void Test::ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void Test::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		m_Window->Close();

    float cameraSpeed = 0.1f;
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
        m_Camera->Move(Voxel::Camera::Direction::LEFT, cameraSpeed);
    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
        m_Camera->Move(Voxel::Camera::Direction::RIGHT, cameraSpeed);
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
        m_Camera->Move(Voxel::Camera::Direction::FORWARD, cameraSpeed);
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
        m_Camera->Move(Voxel::Camera::Direction::BACKWARD, cameraSpeed);
    if (key == GLFW_KEY_LEFT_SHIFT)
        m_Camera->Move(Voxel::Camera::Direction::DOWN, cameraSpeed);
    if (key == GLFW_KEY_SPACE)
        m_Camera->Move(Voxel::Camera::Direction::UP, cameraSpeed);
}

void Test::CursorCallback(GLFWwindow* window, double xPos, double yPos)
{
    const static float s_Sensitivity = 0.1f;
    if (m_FirstMouse)
    {
        m_LastMouse = glm::vec2(xPos, yPos);
        m_FirstMouse = false;
    }

    glm::vec2 offset = glm::vec2(xPos - m_LastMouse.x, m_LastMouse.y - yPos); // Y is reversed. (Y goes from top to bottom)
    m_LastMouse = glm::vec2(xPos, yPos);

    offset *= s_Sensitivity;

    m_Camera->Mouse(offset);
}

void Test::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    
}

void Test::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    m_Camera->Scroll(yOffset);
}