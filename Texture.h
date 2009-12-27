#pragma once
#include "main.h"

class Texture
{
private:
    IDirect3DDevice9 *device;
    IDirect3DTexture9 *texture;
public:
    Texture(IDirect3DDevice9 *device, const char *filename = NULL);
    void set(unsigned samler_index = 0);
    void unset(unsigned samler_index = 0);
    ~Texture();
};

class CubeTexture
{
private:
    IDirect3DDevice9 *device;
    IDirect3DCubeTexture9 *texture;
    unsigned size;

    void fill();
public:
    CubeTexture(IDirect3DDevice9 *device, unsigned size);
    void set(unsigned samler_index = 0);
    void unset(unsigned samler_index = 0);
    ~CubeTexture();
};