#include "repch.h"
#include "OpenGLShader.h"
#include <fstream>
#include <glad/glad.h>


namespace RealEngine {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		RE_CORE_ASSERT(false, "Unknown Shader type");	
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// asset/sshader/Texture.glsl
		auto LastSlash = path.find_last_of("/\\");
		LastSlash = LastSlash == std::string::npos ? 0 : LastSlash + 1;
		auto LastDot = path.rfind('.');
		auto count = LastDot == std::string::npos ? path.size() - LastSlash : LastDot - LastSlash;
		m_Name = path.substr(LastSlash, count);


	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}


	void OpenGLShader::SetInt(const std::string& name, const int value) {
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value) {
		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values) {
		UploadUniformFloat2(name, values);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values) {
		UploadUniformFloat3(name, values);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values) {
		UploadUniformFloat4(name, values);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
		UploadUniformMat3(name, matrix);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
		UploadUniformMat4(name, matrix);
	}



	void OpenGLShader::UploadUniformInt(const std::string& name, const int value) 
	{
		GLint Location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(Location, value);
	}
	
	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) 
	{
		GLint Location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(Location, value);
	}
	
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) 
	{
		GLint Location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(Location, values.x, values.y);
	}
	
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) 
	{
		GLint Location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(Location, values.x, values.y, values.z);
	}
	
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) 
	{
		GLint Location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(Location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) 
	{
		GLint Location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniformMatrix3fv(Location, 1, GL_FALSE, glm::value_ptr(matrix));

	}
	
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) 
	{
		GLint Location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(matrix));

	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::ifstream in(path, std::ios::in | std::ios::binary);
		std::string result;
		if (in) {
			in.seekg(0, std::ios::end); // go the the end of the file
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);	// go the the begining of the file
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			RE_CORE_ERROR("Could not open file {0}", path);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string source)
	{
		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			RE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			RE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSource[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSource;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{

		GLuint program = glCreateProgram();

		RE_CORE_ASSERT(shaderSources.size() <= 2, "We only support two shaders for now!");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDsIndex = 0;

		for (auto& kv : shaderSources) {

			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			
			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);


			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				RE_CORE_ERROR("{0}", infoLog.data());
				RE_CORE_ASSERT(false, "Shader Compilation failure!");

				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDsIndex++] = (shader);
		}


		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
	

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.

			for(auto id:glShaderIDs)
				glDeleteShader(id);

			// Use the infoLog as you see fit.

			RE_CORE_ERROR("{0}", infoLog.data());
			RE_CORE_ASSERT(false, "Shader Link failure!");


			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;

	}



}