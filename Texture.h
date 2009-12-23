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