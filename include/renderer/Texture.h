#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture(const std::string& filepath);
    ~Texture();

    void bind(unsigned int unit = 0) const;
    void unbind() const;
private:
    unsigned int textureID;
    int width, height, channels;
};

