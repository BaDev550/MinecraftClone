#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader() {}
    ~Shader();

	void setupShader(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;
private:
    unsigned int shaderID;

    unsigned int compileShader(unsigned int type, const std::string& source);
    std::string readFile(const std::string& filepath);
};

