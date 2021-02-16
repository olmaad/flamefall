#ifndef SHADER_H
#define SHADER_H

#include <list>
#include <string>
#include <gl/glew.h>

class ShaderWrapper
{
public:
	ShaderWrapper(unsigned int type, const std::string& filename);
	ShaderWrapper(const ShaderWrapper&) = delete;
	ShaderWrapper(ShaderWrapper&&) = default;
	~ShaderWrapper();

	ShaderWrapper& operator=(const ShaderWrapper&) = delete;

	GLuint getId() const;
	bool isCompiled() const;
	void compile();

private:
	GLuint m_shader_id;
	std::string m_filename;

};

class ShaderProgramWrapper
{
public:
	ShaderProgramWrapper();
	ShaderProgramWrapper(ShaderProgramWrapper&& program) = default;
	~ShaderProgramWrapper();

	GLuint getId() const;
    GLuint getUniformId(const char* name) const;

	void consumeShader(ShaderWrapper&& shader);

	void link();
    void use();

private:
	GLuint m_program_id;
	std::list<ShaderWrapper> m_attached_shaders;

};

#endif // !SHADER_H
