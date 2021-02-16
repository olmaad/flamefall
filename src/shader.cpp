#include "shader.h"
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

ShaderWrapper::ShaderWrapper(unsigned int type, const std::string& filename) :
	m_filename(filename)
{
	m_shader_id = glCreateShader(type);
}

ShaderWrapper::~ShaderWrapper()
{
	if (m_shader_id)
	{
		glDeleteShader(m_shader_id);
	}
}

GLuint ShaderWrapper::getId() const
{
	return m_shader_id;
}

bool ShaderWrapper::isCompiled() const
{
	GLint compile_result = GL_FALSE;
	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &compile_result);

	return (compile_result == GL_TRUE);
}

void ShaderWrapper::compile()
{
	std::ifstream source_code_file(m_filename, std::ios::in | std::ios::binary);

	if (!source_code_file.is_open())
	{
		std::string message("Can not open shader source code file \"");
		message += m_filename;
		message += "\"";

		throw std::runtime_error(message);
	}

	std::stringstream source_code_stream;
	source_code_stream << source_code_file.rdbuf();

	source_code_file.close();

	std::string source_code(source_code_stream.str());

	const char* const source_code_ptr = source_code.c_str();

	glShaderSource(m_shader_id, 1, &source_code_ptr, NULL);
	glCompileShader(m_shader_id);

	GLint compile_result = GL_FALSE;
	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &compile_result);

	if (compile_result != GL_TRUE)
	{
		GLint compile_log_length = 0;
		glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &compile_log_length);

		std::string compile_log(compile_log_length, 0);
		glGetShaderInfoLog(m_shader_id, compile_log_length, NULL, compile_log.data());

		std::string message("Error compiling shader file \"");
		message += m_filename;
		message += "\": ";
		message += compile_log;

		throw std::runtime_error(message);
	}
}

ShaderProgramWrapper::ShaderProgramWrapper()
{
	m_program_id = glCreateProgram();
}

ShaderProgramWrapper::~ShaderProgramWrapper()
{
	for (auto& shader : m_attached_shaders)
	{
		glDetachShader(m_program_id, shader.getId());
	}

	if (m_program_id)
	{
		glDeleteProgram(m_program_id);
	}
}

GLuint ShaderProgramWrapper::getId() const
{
	return m_program_id;
}

GLuint ShaderProgramWrapper::getUniformId(const char* name) const
{
    return glGetUniformLocation(m_program_id, name);
}

void ShaderProgramWrapper::consumeShader(ShaderWrapper&& shader)
{
	m_attached_shaders.emplace_back(std::move(shader));

	glAttachShader(m_program_id, m_attached_shaders.back().getId());
}

void ShaderProgramWrapper::link()
{
	glLinkProgram(m_program_id);

	GLint link_result = GL_FALSE;
	glGetProgramiv(m_program_id, GL_LINK_STATUS, &link_result);

	if (link_result != GL_TRUE)
	{
		GLint link_log_length = 0;
		glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &link_log_length);

		std::string link_log(link_log_length, 0);
		glGetProgramInfoLog(m_program_id, link_log_length, NULL, link_log.data());

		std::string message("Error linking shaders: ");
		message += link_log;

		throw std::runtime_error(message);
	}
}

void ShaderProgramWrapper::use()
{
    glUseProgram(m_program_id);
}
