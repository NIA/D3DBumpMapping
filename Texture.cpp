#include "Texture.h"

Texture::Texture(IDirect3DDevice9 *device, const char *filename)
: device(device), texture(NULL)
{
    D3DXCreateTextureFromFileA(device, filename, &texture);
    device->SetTextureStageState(0, D3DTSS_COLOROP,  D3DTOP_MODULATE);
    device->SetTextureStageState(0, D3DTSS_COLORARG1,D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2,D3DTA_DIFFUSE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
    //device->SetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value );
}

void Texture::set()
{
    device->SetTexture(0, texture);
}

Texture::~Texture()
{
    release_interface(texture);
}