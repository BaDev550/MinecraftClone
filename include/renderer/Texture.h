#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture() {}
    ~Texture();

	void setTexture(const std::string& path);
    void bind(unsigned int unit = 0) const;
    void unbind() const;
    void deleteTexture() const;

    unsigned char* heightMap_data;
    void loadHeightMap(const std::string& path, int& height, int& width);
    void getHeightMapHeight(int x, int y, int& updateHeight);
private:
    unsigned int textureID;
    int width, height, channels;
};

