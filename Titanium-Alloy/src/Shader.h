#pragma once

#include <glad/gl.h>

#include <string>
#include <assert.h>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_LocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Todo: Replace with functions: SetUniform4f, SetUniform1i e.t.c.
	template<typename T>
	void SetUniform(const std::string& name, const T& data)
	{
		static_assert(false);
	}

	template<>
	void SetUniform<glm::fvec4>(const std::string& name, const glm::fvec4& data)
	{
		glUniform4f(GetLocation(name), data.x, data.y, data.z, data.w);
	}

	template<>
	void SetUniform<int>(const std::string& name, const int& value)
	{
		glUniform1i(GetLocation(name), value);
	}

	template<>
	void SetUniform<glm::mat4>(const std::string& name, const glm::mat4& data)
	{
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &data[0][0]);
	}

private:
	void AddShader(unsigned int type, const std::string& source);
	int GetLocation(const std::string& name);
};