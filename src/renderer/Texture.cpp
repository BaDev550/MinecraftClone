#include <renderer/Texture.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::setTexture(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	int format = channels == 4 ? GL_RGBA : GL_RGB;

    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else {
        std::cout << "Failed to load texture: " << path << std::endl;
        textureID = 0;
    }
}

void Texture::loadHeightMap(const std::string& path, int& height, int& width)
{
    stbi_set_flip_vertically_on_load(true);
    heightMap_data = stbi_load(path.c_str(), &this->width, &this->height, &channels, 0);
}

void Texture::getHeightMapHeight(int x, int y, int& updateHeight)
{
    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++) {
            unsigned char* texel = heightMap_data + (j + width * i) * channels;
            unsigned char yCoord = texel[0];

            std::cout << round((int)yCoord * yScale - yShift) << std::endl;
            updateHeight = round((int)yCoord * yScale - yShift);
        }
    }
}

void Texture::bind(unsigned int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::deleteTexture() const {
    glDeleteTextures(1, &textureID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
