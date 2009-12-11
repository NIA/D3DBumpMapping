#pragma once

#include "main.h"

class VertexShader
{
private:
    IDirect3DDevice9            *device;
    IDirect3DVertexShader9      *shader;        // vertex shader
public:
    VertexShader(IDirect3DDevice9 *device, const char *shader_filename);
    void set();
    ~VertexShader();
};

class PixelShader
{
private:
    IDirect3DDevice9            *device;
    IDirect3DPixelShader9       *shader;        // pixel shader
public:
    PixelShader(IDirect3DDevice9 *device, const char *shader_filename = NULL); // NULL means use no shader
    void set();
    ~PixelShader();
};
