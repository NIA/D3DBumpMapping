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