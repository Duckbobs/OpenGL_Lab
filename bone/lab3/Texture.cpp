


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "Texture.hpp"

int
mapToGl(int textureUnit) {
    const int lut[] = {
        GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5
    };
    return lut[textureUnit];
}

int
mapToGl(ColorChanel c) {
   int const lut[] = {
      GL_RGB, GL_RGBA
   };
   return lut[static_cast<int>(c)];
}

void
Texture::load2DTexture(std::string path, ColorChanel c) 
{


	int width, height, channel;

	unsigned char * image = stbi_load(path.c_str(), &width, &height, &channel, 0);

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, mapToGl(c), width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void
Texture::activate(int textureUnit) {
    glActiveTexture(mapToGl( textureUnit));
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void
Texture::createDepthTexture(int width, int height) {
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLuint
Texture::getTexture() {
    return m_Texture;
}
