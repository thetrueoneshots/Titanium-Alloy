#include "Shader.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

static const struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static unsigned int CompileShader(unsigned int type, const std::string& source);
static ShaderProgramSource ParseShader(const std::string& filepath);

Voxel::Shader::Shader(const std::string& filepath)
{
    m_FilePath = filepath;
	m_RendererID = glCreateProgram();

    ShaderProgramSource source = ParseShader(filepath);
    AddShader(GL_VERTEX_SHADER, source.VertexSource);
    AddShader(GL_FRAGMENT_SHADER, source.FragmentSource);

    glLinkProgram(m_RendererID);
    glValidateProgram(m_RendererID);

    Bind();
}

Voxel::Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Voxel::Shader::AddShader(unsigned int type, const std::string& source)
{
    unsigned int s = CompileShader(type, source);
    assert(s != 0);

    glAttachShader(m_RendererID, s);
    glDeleteShader(s);
}

int Voxel::Shader::GetLocation(const std::string& name)
{
    if (m_LocationCache.count(name) != 0)
    {
        return m_LocationCache.find(name)->second;
    }

    int location = glGetUniformLocation(m_RendererID, name.c_str());

    if (location == -1)
    {
        char buffer[250];
        sprintf_s(buffer, "[Error] Unkown uniform name. (%s)", name.c_str());
        std::cout << buffer << std::endl;
    }
    else {
        m_LocationCache.insert_or_assign(name, location);
    }
    return location;
}

void Voxel::Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Voxel::Shader::Unbind() const
{
	glUseProgram(0);
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // Allocate on the stack dynamically and not on the heap.
        char* message = (char*)_malloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        // Todo: Decide to use this or glDetachShader(id).
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };

    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
            else
            {
                std::cout << "[Error] Unknown shader type. Skip until next shader." << std::endl;
                type = ShaderType::NONE;
            }
            continue;
        }

        if (type == ShaderType::NONE) continue;
        ss[(int)type] << line << '\n';
    }

    return { ss[0].str(), ss[1].str() };
}
