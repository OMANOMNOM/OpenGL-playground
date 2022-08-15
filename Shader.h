#pragma once

#include <string>

class Shader
{
private:
	unsigned int rendererID;
	//caching for unifomrs
public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniform
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int GetUniformLocaiton(const std::string& name);
};