#include "Texture.h"

// -------------------------------- Texture --------------------------------------------

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

// ------------------------------ Cube Texture ------------------------------------------

CubeTexture::CubeTexture(IDirect3DDevice9 *device, unsigned size)
: device(device), texture(NULL), size(size)
{
    _ASSERT( 0 == size % 2 ); // size is even integer
    check_texture( device->CreateCubeTexture(size, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, 0 ) );
    fill();
}

unsigned float_to_color(float f)
{
    return static_cast<unsigned>( f*128 ) + 128 ;
}

D3DCOLOR vector_to_color(const D3DXVECTOR3 &vector)
{
    return D3DCOLOR_XRGB( float_to_color(vector.x), float_to_color(vector.y), float_to_color(vector.z) );
}

void CubeTexture::fill()
{
	int half_size = size/2;
	D3DLOCKED_RECT rect;
    D3DCOLOR *data;
    int x, y, z;
    
    check_texture( texture->LockRect( D3DCUBEMAP_FACE_POSITIVE_X, 0, &rect, NULL, 0 ) );
    data = reinterpret_cast<D3DCOLOR*>(rect.pBits);
    x = half_size;
    for( int y = -half_size + 1; y <= half_size; ++y )
    {
        for( int z = -half_size + 1; z <= half_size; ++z )
        {
            D3DXVECTOR3 vector( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
            D3DXVec3Normalize(&vector, &vector);
            data[(half_size - y)*size + (half_size - z)] = vector_to_color(vector);
        }
    }
    check_texture( texture->UnlockRect( D3DCUBEMAP_FACE_POSITIVE_X, 0 ) );
    
    check_texture( texture->LockRect( D3DCUBEMAP_FACE_NEGATIVE_X, 0, &rect, NULL, 0 ) );
    data = reinterpret_cast<D3DCOLOR*>(rect.pBits);
    x = -half_size;
    for( int y = -half_size + 1; y <= half_size; ++y )
    {
        for( int z = -half_size; z < half_size; ++z )
        {
            D3DXVECTOR3 vector( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
            D3DXVec3Normalize(&vector, &vector);
            data[(half_size - y)*size + (half_size + z)] = vector_to_color(vector);
        }
    }
    check_texture( texture->UnlockRect( D3DCUBEMAP_FACE_NEGATIVE_X, 0 ) );
    
    check_texture( texture->LockRect( D3DCUBEMAP_FACE_POSITIVE_Y, 0, &rect, NULL, 0 ) );
    data = reinterpret_cast<D3DCOLOR*>(rect.pBits);
    y = half_size;
    for( int x = -half_size; x < half_size; ++x )
    {
        for( int z = -half_size; z < half_size; ++z )
        {
            D3DXVECTOR3 vector( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
            D3DXVec3Normalize(&vector, &vector);
            data[(half_size + x) + (half_size + z)*size] = vector_to_color(vector);
        }
    }
    check_texture( texture->UnlockRect( D3DCUBEMAP_FACE_POSITIVE_Y, 0 ) );
    
    check_texture( texture->LockRect( D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &rect, NULL, 0 ) );
    data = reinterpret_cast<D3DCOLOR*>(rect.pBits);
    y = -half_size;
    for( int x = -half_size; x < half_size; ++x )
    {
        for( int z = -half_size + 1; z <= half_size; ++z )
        {
            D3DXVECTOR3 vector( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
            D3DXVec3Normalize(&vector, &vector);
            data[(half_size + x) + (half_size - z)*size] = vector_to_color(vector);
        }
    }
    check_texture( texture->UnlockRect( D3DCUBEMAP_FACE_NEGATIVE_Y, 0 ) );
    
    check_texture( texture->LockRect( D3DCUBEMAP_FACE_POSITIVE_Z, 0, &rect, NULL, 0 ) );
    data = reinterpret_cast<D3DCOLOR*>(rect.pBits);
    z = half_size;
    for( int x = -half_size; x < half_size; ++x )
    {
        for( int y = -half_size + 1; y <= half_size; ++y )
        {
            D3DXVECTOR3 vector( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
            D3DXVec3Normalize(&vector, &vector);
            data[(half_size + x) + (half_size - y)*size] = vector_to_color(vector);
        }
    }
    check_texture( texture->UnlockRect( D3DCUBEMAP_FACE_POSITIVE_Z, 0 ) );
    
    check_texture( texture->LockRect( D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &rect, NULL, 0 ) );
    data = reinterpret_cast<D3DCOLOR*>(rect.pBits);
    z = -half_size;
    for( int x = -half_size + 1; x <= half_size; ++x )
    {
        for( int y = -half_size + 1; y <= half_size; ++y )
        {
            D3DXVECTOR3 vector( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
            D3DXVec3Normalize(&vector, &vector);
            data[(half_size - x) + (half_size - y)*size] = vector_to_color(vector);
        }
    }
    check_texture( texture->UnlockRect( D3DCUBEMAP_FACE_NEGATIVE_Z, 0 ) );
    
}

void CubeTexture::set(unsigned samler_index /*= 0*/)
{
    check_texture( device->SetTexture(samler_index, texture) );
}

void CubeTexture::unset(unsigned samler_index /*= 0*/)
{
    check_texture( device->SetTexture(samler_index, NULL) );
}

CubeTexture::~CubeTexture()
{
    release_interface(texture);
}
