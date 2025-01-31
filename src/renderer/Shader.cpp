#include <renderer/Shader.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::~Shader()
{
	glDeleteProgram(shaderID);
}

void Shader::setupShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);

    int success;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::Compile: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
	glUseProgram(shaderID);
}

void Shader::setInt(const std::string& name, const int& value) const
{
    use();
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    use();
	glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	use();
	glUniform2fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Compile: " << infoLog << std::endl;
    }
    return shader;
}

std::string Shader::readFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
