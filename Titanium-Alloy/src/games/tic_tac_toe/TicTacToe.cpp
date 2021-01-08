#include "TicTacToe.h"

static int CheckWinner(char board[3][3]);

void TicTacToe::Init()
{
	std::cout << "## Tic Tac Toe: Init." << std::endl;

    // Init variables
    m_FirstMouse = true;
    m_LastTime = 0.0f;
    m_LastMouse = glm::vec2(0, 0);
    m_Player1 = true;
    ResetGrid();

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

    // Create board
    glm::ivec3 boardSize(5, 5, 2);
    m_Board = new Voxel::Mesh(boardSize);
    for (int i = 0; i < boardSize.x; i++)
    {
        for (int j = 0; j < boardSize.y; j++)
        {
            if (i == 0 || i == boardSize.x - 1 || j == 0 || j == boardSize.y - 1)
            {
                m_Board->GetMeshData()->AddCube(glm::vec3(i, j, 1), glm::vec4(0.5f, 0.5f, 0.5f, 1));
            }
            else
            {
                m_Board->GetMeshData()->AddCube(glm::vec3(i, j, 0), glm::vec4(0.2f, 0.2f, 0.2f, 1));
            }
        }
    }
    m_Board->GetTransForm()->Translate(glm::vec3(-1, -1, -1));
}

void TicTacToe::Cleanup()
{
	std::cout << "## Tic Tac Toe: Cleanup." << std::endl;

    for (const auto& pair : m_Models)
    {
        delete pair.second;
    }
}

void TicTacToe::Update()
{
    int winner = CheckWinner(m_Grid);
    if (winner != 0)
    {
        Voxel::MeshData* data = nullptr;
        switch(winner)
        {
        case 1:
            data = m_Models.at((int)ModelType::WINNER);
            break;
        case 2:
            data = m_Models.at((int)ModelType::LOSER);
            break;
        default:
            data = m_Models.at((int)ModelType::LOSER);
            break;
        }
        
        Voxel::Mesh m(data);
        m.GetTransForm()->Rotate(glm::vec3(glm::radians(90.0f), 0 , glm::radians(180.0f)));
        m.GetTransForm()->Translate(glm::vec3(12, 12, -10));
        m_Renderer->Render(&m);
    }
    else
    {
        m_Renderer->Render(m_Board);

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Voxel::Mesh* m;
                Voxel::Transform t;
                t.Translate(glm::vec3(i, j + 1, 0));
                t.Rotate(glm::vec3(glm::radians(90.0f), 0, 0));
                switch (m_Grid[i][j])
                {
                case 1:
                    m = new Voxel::Mesh(m_Models.at((int)ModelType::CIRCLE), &t);
                    m->ScaleToSize(1.0f);
                    break;
                case 2:
                    m = new Voxel::Mesh(m_Models.at((int)ModelType::CROSS), &t);
                    m->ScaleToSize(1.0f);
                    break;
                case 3:
                    m = new Voxel::Mesh(m_Models.at((int)ModelType::SELECTOR), &t);
                    m->ScaleToSize(1.0f);
                    m->ScaleYToSize(1.0f / 16);
                    break;
                case 0:
                    m = nullptr;
                    break;
                default:
                    m = nullptr;
                    break;
                }

                if (m == nullptr)
                {
                    continue;
                }

                m_Renderer->Render(m);
                delete m;
            }
        }
    }
}

void TicTacToe::ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void TicTacToe::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		m_Window->Close();

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        ResetGrid();
    }

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

void TicTacToe::CursorCallback(GLFWwindow* window, double xPos, double yPos)
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

    Voxel::Line l(m_Camera->GetPosition(), m_Camera->GetNormal());

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Voxel::Box b(glm::vec3(i, j, 0), glm::vec3(i + 1, j + 1, 0));

            if (Voxel::Collider::Collision(l, b))
            {
                if (m_Grid[i][j] == 0)
                {
                    m_Grid[i][j] = 3;
                }
            }
            else if (m_Grid[i][j] == 3)
            {
                m_Grid[i][j] = 0;
            }
        }
    }
}

void TicTacToe::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        Voxel::Line l(m_Camera->GetPosition(), m_Camera->GetNormal());

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Voxel::Box b(glm::vec3(i, j, 0), glm::vec3(i + 1, j + 1, 0));

                if (Voxel::Collider::Collision(l, b))
                {
                    if (m_Grid[i][j] == 1 || m_Grid[i][j] == 2)
                    {
                        continue;
                    }

                    if (m_Player1)
                    {
                        m_Grid[i][j] = 1;
                    }
                    else
                    {
                        m_Grid[i][j] = 2;
                    }

                    m_Player1 = !m_Player1;

                    return;
                }
            }
        }

    }
}

void TicTacToe::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    m_Camera->Scroll(yOffset);
}

void TicTacToe::ResetGrid()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m_Grid[i][j] = 0;
        }
    }
}

static int CheckWinner(char board[3][3])
{
    int winner;

    // any of the rows is same
    for (int i = 0; i <= 2; i++)
    {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0)
        {
            winner = board[i][0];
            return winner;
        }
    }

    // any of the columns is same
    for (int i = 0; i <= 2; i++)
    {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0)
        {
            winner = board[0][i];
            return winner;
        }
    }

    // 1st diagonal is same
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0)
    {
        winner = board[0][0];
        return winner;
    }

    // 2nd diagonal is same
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0)
    {
        winner = board[0][2];
        return winner;
    }

    // if we reached here nobody has won yet

    // if any empty box on board then play on
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            if (board[i][j] != 1 && board[i][j] != 2)
            {
                winner = 0;
                return winner;
            }
        }
    }

    winner = -1; // all boxes full and nobody won so A tie has occurred
    return winner;
}