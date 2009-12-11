#pragma once
#include "main.h"

class Texture
{
private:
    IDirect3DDevice9 *device;
    IDirect3DTexture9 *texture;
public:
    Texture::Texture(IDirect3DDevice9 *device, const char *filename = NULL);
    void set();
    ~Texture();
};