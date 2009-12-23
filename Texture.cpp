#include "Texture.h"

Texture::Texture(IDirect3DDevice9 *device, const char *filename)
: device(device), texture(NULL)
{
    check_texture( D3DXCreateTextureFromFileA(device, filename, &texture) );
}


void Texture::set(unsigned samler_index /*= 0*/)
{
    check_texture( device->SetTexture(samler_index, texture) );
}

void Texture::unset(unsigned samler_index /*= 0*/)
{
    check_texture( device->SetTexture(samler_index, NULL) );
}

Texture::~Texture()
{
    release_interface(texture);
}