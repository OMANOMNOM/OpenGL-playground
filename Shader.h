#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_rendererID;
	std::string m_filePath;
	std::unordered_map<std::string, int> m_uniformLocationCache;
public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniform
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);


private:
	unsigned int CompileShader(const std::string& source, unsigned int type);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int GetUniformLocation(const std::string& name);
};