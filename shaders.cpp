#include "shaders.h"

VertexShader::VertexShader(IDirect3DDevice9 *device, const char *shader_filename)
: device(device), shader(NULL)
{
    ID3DXBuffer * shader_buffer = NULL;
    try
    {
        if( FAILED( D3DXAssembleShaderFromFileA( shader_filename, NULL, NULL, NULL, &shader_buffer, NULL ) ) )
            throw VertexShaderAssemblyError();
        if( FAILED( device->CreateVertexShader( (DWORD*) shader_buffer->GetBufferPointer(), &shader ) ) )
            throw VertexShaderInitError();
    }
    // using catch(...) because every caught exception is rethrown
    catch(...)
    {
        release_interface(shader_buffer);
        throw;
    }
    release_interface(shader_buffer);
}

void VertexShader::set()
{
    check_render( device->SetVertexShader(shader) );
}

VertexShader::~VertexShader()
{
    release_interface( shader );
}



PixelShader::PixelShader(IDirect3DDevice9 *device, const char *shader_filename)
: device(device), shader(NULL)
{
    if( shader_filename != NULL )
    {
        ID3DXBuffer * shader_buffer = NULL;
        try
        {
            if( FAILED( D3DXAssembleShaderFromFileA( shader_filename, NULL, NULL, NULL, &shader_buffer, NULL ) ) )
                throw PixelShaderAssemblyError();
            if( FAILED( device->CreatePixelShader( (DWORD*) shader_buffer->GetBufferPointer(), &shader ) ) )
                throw PixelShaderInitError();
        }
        // using catch(...) because every caught exception is rethrown
        catch(...)
        {
            release_interface(shader_buffer);
            throw;
        }
        release_interface(shader_buffer);
    }
}

void PixelShader::set()
{
    check_render( device->SetPixelShader(shader) );
}

PixelShader::~PixelShader()
{
    release_interface( shader );
}
