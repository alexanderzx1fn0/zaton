#include "Texture.h"


#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>          // Multiple image fileformats loading functions

Texture::Texture(const char *filename) {

    unsigned char* texture_data = stbi_load( filename, &width, &height, &components, 0);
    if (texture_data == nullptr)
    {
	printf( "Failed to load image %s\n", filename);
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    switch( components ) {
    case 2: {
        format = GL_RGBA;
    }
	case 3: {
	    // RGB
	    format = GL_RGB;
	    break;
	}
	case 4: {
	    // RGBA
	    format = GL_RGBA;
	    break;
	}
	default: {
	    // Unknown format
	    assert(0);
	}
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texture_data);


}


Texture::~Texture() {
    if (id > 0) glDeleteTextures(1, &id);
}

void Texture::bind(int sampler) {
    glActiveTexture(GL_TEXTURE0+sampler);
    glBindTexture(GL_TEXTURE_2D, id);
}
