#pragma once
#include <string>
#include <glm/glm.hpp>
class Shader {
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    bool isCompiled() const; // Add this method
};