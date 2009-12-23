#include "Vertex.h"

///////////////////////// C O N S T A N T S /////////////////////////////////////////////
const D3DFORMAT INDEX_FORMAT = D3DFMT_INDEX32;

//////////////////////////// D E C L A R A T I O N ///////////////////////////////////////////////
VertexDeclaration::VertexDeclaration(IDirect3DDevice9 *device, const D3DVERTEXELEMENT9* vertex_declaration)
: device(device), vertex_decl(NULL)
{
    _ASSERT(device != NULL);
    if( FAILED( device->CreateVertexDeclaration(vertex_declaration, &vertex_decl) ) )
        throw VertexDeclarationInitError();
}

void VertexDeclaration::set()
{
    check_render( device->SetVertexDeclaration(vertex_decl) );
}

VertexDeclaration::~VertexDeclaration()
{
    release_interface( vertex_decl );
}

const D3DVERTEXELEMENT9 VERTEX_DECL_ARRAY[] =
{
    {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
    {0, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    D3DDECL_END()
};
extern const D3DVERTEXELEMENT9 TEXTURED_VERTEX_DECL_ARRAY[] = 
{
    {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
    {0, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    {0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    {0, 40, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
    {0, 56, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
    D3DDECL_END()
};
