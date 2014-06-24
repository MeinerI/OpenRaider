/*!
 * \file src/TextureManager.cpp
 * \brief Texture registry
 *
 * \author Mongoose
 * \author xythobuz
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "global.h"
#include "utils/pcx.h"
#include "utils/pixel.h"
#include "utils/strings.h"
#include "utils/tga.h"
#include "TextureManager.h"

TextureManager::TextureManager() {
    mTextureIds = NULL;
    mFlags = 0;
    mTextureCount = 0;
    mTextureLimit = 0;
}

TextureManager::~TextureManager() {
    reset();
}

unsigned char *TextureManager::generateColorTexture(const unsigned char rgba[4],
        unsigned int width, unsigned int height) {
    assert(rgba != NULL);
    assert(width > 0);
    assert(height > 0);

    unsigned char *image = new unsigned char[height * width * 4];

    for (unsigned int i = 0; i < (width * height); i++) {
        image[i * 4] = rgba[0];
        image[(i * 4) + 1] = rgba[1];
        image[(i * 4) + 2] = rgba[2];
        image[(i * 4) + 3] = rgba[3];
    }

    return image;
}

int TextureManager::loadColorTexture(const unsigned char rgba[4],
        unsigned int width, unsigned int height) {
    assert(rgba != NULL);
    assert(width > 0);
    assert(height > 0);

    unsigned char *image = generateColorTexture(rgba, width, height);
    int id = loadBuffer(image, width, height, RGBA, 32);
    delete [] image;

    return id;
}

void TextureManager::setFlag(TextureFlag flag) {
    mFlags |= flag;
}

void TextureManager::clearFlag(TextureFlag flag) {
    mFlags &= ~flag;
}

void TextureManager::reset() {
    if (mTextureIds) {
        glDeleteTextures(mTextureLimit, mTextureIds);
        delete [] mTextureIds;
    }

    mTextureIds = NULL;
    mTextureCount = 0;
    mTextureLimit = 0;
}

void TextureManager::disableMultiTexture() {
    mFlags &= ~fUseMultiTexture;

    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
}

void TextureManager::useMultiTexture(float aU, float aV, float bU, float bV) {
    if (!(mFlags & fUseMultiTexture))
        return;

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, aU, aV);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, bU, bV);
}

void TextureManager::useMultiTexture(float u, float v) {
    useMultiTexture(u, v, u, v);
}

void TextureManager::bindMultiTexture(int texture0, int texture1) {
    assert(mTextureIds != NULL);
    assert(texture0 >= 0);
    assert(texture1 >= 0);
    assert((unsigned int)texture0 <= mTextureCount);
    assert((unsigned int)texture1 <= mTextureCount);

    mFlags |= fUseMultiTexture;

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTextureIds[texture0]);

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTextureIds[texture1]);
}

void TextureManager::setMaxTextureCount(unsigned int n) {
    mTextureLimit = n;
    mTextureIds = new unsigned int[n];
    glGenTextures(n, mTextureIds);
}

int TextureManager::getTextureCount() {
    return mTextureCount - 1;
}

int TextureManager::loadBuffer(unsigned char *image,
        unsigned int width, unsigned int height,
        ColorMode mode, unsigned int bpp) {
    int id;

    assert(image != NULL);
    assert(width > 0);
    assert(height > 0);
    assert((bpp == 8) || (bpp == 24) || (bpp == 32));

    id = loadBufferSlot(image, width, height, mode, bpp, mTextureCount++);

    if (id < 0)
        return id;

    return id;
}

int TextureManager::loadBufferSlot(unsigned char *image,
        unsigned int width, unsigned int height,
        ColorMode mode, unsigned int bpp,
        unsigned int slot) {
    unsigned char bytes;
    unsigned int glcMode;

    assert(mTextureIds != NULL);
    assert(slot < mTextureLimit);
    assert(image != NULL);
    assert(width > 0);
    assert(height > 0);
    assert((bpp == 8) || (bpp == 24) || (bpp == 32));

    switch (mode) {
        case GREYSCALE:
            if (bpp != 8) {
                printf("TextureManager::Load ERROR Unsupported GREYSCALE, %i bpp\n", bpp);
                return -1;
            }
            bytes = 1;
            glcMode = GL_LUMINANCE;
            break;

        case RGB:
            if (bpp != 24) {
                printf("TextureManager::Load ERROR Unsupported RGB, %i bpp\n", bpp);
                return -1;
            }
            bytes = 3;
            glcMode = GL_RGB;
            break;

        case ARGB:
            if (bpp == 32) {
                argb2rgba32(image, width, height);
            } else {
                printf("TextureManager::Load ERROR Unsupported ARGB, %i bpp\n", bpp);
                return -1;
            }
            bytes = 4;
            glcMode = GL_RGBA;
            break;

        case RGBA:
            if (bpp != 32) {
                printf("TextureManager::Load ERROR Unsupported RGBA, %i bpp\n", bpp);
                return -1;
            }
            bytes = 4;
            glcMode = GL_RGBA;
            break;

        case BGR:
            if (bpp != 24) {
                printf("TextureManager::Load ERROR Unsupported BGR, %i bpp\n", bpp);
                return -1;
            }
            bytes = 3;
            glcMode = GL_BGR_EXT;
            break;

        case BGRA:
            if (bpp != 32) {
                printf("TextureManager::Load ERROR Unsupported BGRA, %i bpp\n", bpp);
                return -1;
            }
            bytes = 4;
            glcMode = GL_BGRA_EXT;
            break;
    }

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, mTextureIds[slot]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, bytes, width, height, 0, glcMode, GL_UNSIGNED_BYTE, image);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    return slot;
}

void TextureManager::bindTextureId(unsigned int n) {
    assert(mTextureIds != NULL);
    assert(n <= mTextureCount);

    glEnable(GL_TEXTURE_2D);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, mTextureIds[n]);
}

int TextureManager::loadPCX(const char *filename) {
    assert(filename != NULL);
    assert(filename[0] != '\0');

    unsigned char *image;
    unsigned int w, h, bpp;
    ColorMode c;
    int id = -1;
    int error = pcxLoad(filename, &image, &w, &h, &c, &bpp);

    if (error == 0) {
        unsigned char *image2 = scaleBuffer(image, &w, &h, bpp);
        if (image2) {
            delete [] image;
            image = image2;
        }
        id = loadBuffer(image, w, h, c, bpp);
        delete [] image;
    }

    return id;
}

int TextureManager::loadTGA(const char *filename) {
    assert(filename != NULL);
    assert(filename[0] != '\0');

    unsigned char *image = NULL;
    unsigned char *image2 = NULL;
    unsigned int w, h;
    char type;
    int id = -1;

    if (!tgaCheck(filename)) {
        tgaLoad(filename, &image, &w, &h, &type);

        image2 = scaleBuffer(image, &w, &h, (type == 2) ? 32 : 24);

        if (image2) {
            delete [] image;
            image = image2;
        }

        if (image) {
            id = loadBuffer(image, w, h,
                    (type == 2) ? RGBA : RGB,
                    (type == 2) ? 32 : 24);

            delete [] image;
        }
    }

    if (id == -1) {
        printf("TextureManager::loadTGA> ERROR: Failed to load '%s'\n", filename);
    }

    return id;
}
