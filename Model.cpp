#include "Model.h"
#include "matrices.h"


const unsigned DIFF_TEXTURE_SAMPLER = 0;
const unsigned NORMALS_TEXTURE_SAMPLER = 1;

namespace
{
    const unsigned LIGHT_SOURCE_CONSTANTS_USED = 1; // radius
}

extern const unsigned VECTORS_IN_MATRIX;

Model::Model(   IDirect3DDevice9 *device, D3DPRIMITIVETYPE primitive_type,
                VertexShader &vertex_shader, VertexShader &shadow_vertex_shader, PixelShader &pixel_shader, PixelShader &shadow_pixel_shader,
                VertexDeclaration &vertex_declaration, unsigned vertex_size,
                const Vertex *vertices, unsigned vertices_count, const Index *indices, unsigned indices_count,
                unsigned primitives_count, D3DXVECTOR3 position, D3DXVECTOR3 rotation )
 
: device(device), vertices_count(vertices_count), primitives_count(primitives_count),
  primitive_type(primitive_type), vertex_buffer(NULL), index_buffer(NULL),
  position(position), rotation(rotation), 
  vertex_shader(vertex_shader), shadow_vertex_shader(shadow_vertex_shader), pixel_shader(pixel_shader), shadow_pixel_shader(shadow_pixel_shader),
  vertex_declaration(vertex_declaration), vertex_size(vertex_size)
{
    _ASSERT(vertices != NULL);
    _ASSERT(indices != NULL);
    try
    {
        const unsigned vertices_size = vertices_count*vertex_size;
        const unsigned indices_size = indices_count*sizeof(indices[0]);

        if(FAILED( device->CreateVertexBuffer( vertices_size, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertex_buffer, NULL ) ))
            throw VertexBufferInitError();
        

        if(FAILED( device->CreateIndexBuffer( indices_size, D3DUSAGE_WRITEONLY, INDEX_FORMAT, D3DPOOL_DEFAULT, &index_buffer, NULL ) ))
            throw IndexBufferInitError();

        // fill the vertex buffer.
        VOID* vertices_to_fill;
        if(FAILED( vertex_buffer->Lock( 0, vertices_size, &vertices_to_fill, 0 ) ))
            throw VertexBufferFillError();
        memcpy( vertices_to_fill, vertices, vertices_size );
        vertex_buffer->Unlock();

        // fill the index buffer.
        VOID* indices_to_fill;
        if(FAILED( index_buffer->Lock( 0, indices_size, &indices_to_fill, 0 ) ))
            throw IndexBufferFillError();
        memcpy( indices_to_fill, indices, indices_size );
        index_buffer->Unlock();
    
        update_matrix();
    }
    // using catch(...) because every caught exception is rethrown
    catch(...)
    {
        release_interfaces();
        throw;
    }
}

void Model::draw() const
{
    check_render( device->SetStreamSource( 0, vertex_buffer, 0, vertex_size ) );
    check_render( device->SetIndices( index_buffer ) );
    check_render( device->DrawIndexedPrimitive( primitive_type, 0, 0, vertices_count, 0, primitives_count ) );
}

void Model::update_matrix()
{
    rotation_and_position = rotate_and_shift_matrix(rotation, position);
}

void Model::rotate(float phi)
{
    rotation.z += phi;
    update_matrix();
}

const D3DXMATRIX &Model::get_rotation_and_position() const
{
    return rotation_and_position;
}

void Model::release_interfaces()
{
    release_interface(vertex_buffer);
    release_interface(index_buffer);
}

Model::~Model()
{
    release_interfaces();
}

// -------------------------------------- TexturedModel -------------------------------------------------------------

TexturedModel::TexturedModel
    (IDirect3DDevice9 *device, D3DPRIMITIVETYPE primitive_type,
     VertexShader &vertex_shader, VertexShader &shadow_vertex_shader, PixelShader &pixel_shader, PixelShader &shadow_pixel_shader,
     const TexturedVertex *vertices, unsigned int vertices_count, const Index *indices, unsigned int indices_count,
     unsigned int primitives_count, D3DXVECTOR3 position, D3DXVECTOR3 rotation, Texture &texture, Texture &normals_texture)
: Model(device, primitive_type, vertex_shader, shadow_vertex_shader, pixel_shader, shadow_pixel_shader, TexturedVertex::get_declaration(device), sizeof(TexturedVertex), vertices, vertices_count, indices, indices_count, primitives_count, position, rotation),
  texture(texture), normals_texture(normals_texture)
{
}

// ------------------------------------------ Plane --------------------------------------------------------

Plane::Plane( IDirect3DDevice9 *device, D3DPRIMITIVETYPE primitive_type, VertexShader &vertex_shader, PixelShader &pixel_shader,
              const TexturedVertex *vertices, unsigned vertices_count, const Index *indices, unsigned indices_count,
              unsigned primitives_count, D3DXVECTOR3 position, D3DXVECTOR3 rotation, Texture &texture, Texture &normals_texture )
: TexturedModel(device, primitive_type, vertex_shader, vertex_shader, pixel_shader, pixel_shader, vertices, vertices_count,
                indices, indices_count, primitives_count, position, rotation, texture, normals_texture)
{
    _ASSERT( vertices_count > 0 );
    D3DXVECTOR4 normal_4d = vertices[0].normal;
    D3DXMATRIX rotation_mx = rotate_matrix(rotation);
    D3DXVec4Transform( &normal_4d, &normal_4d, &rotation_mx );

    normal = D3DXVECTOR3( normal_4d );

    d = D3DXVec3Dot( &position, &normal );
}

D3DXMATRIX Plane::get_projection_matrix(const D3DXVECTOR3 light_position) const
{
    // some aliases
    D3DXVECTOR3 const &n = normal;
    D3DXVECTOR3 const &L = light_position;

    float L_dot_n = D3DXVec3Dot(&L, &n);

    D3DXMATRIX M1( d, 0, 0, -d*L.x,
                   0, d, 0, -d*L.y,
                   0, 0, d, -d*L.z,
                   0, 0, 0,  0 );

    D3DXMATRIX M2( L_dot_n,       0,       0, 0,
                         0, L_dot_n,       0, 0,
                         0,       0, L_dot_n, 0,
                         0,       0,       0, 0 );

    D3DXMATRIX M3( L.x*n.x, L.x*n.y, L.x*n.z, 0,
                   L.y*n.x, L.y*n.y, L.y*n.z, 0,
                   L.z*n.x, L.z*n.y, L.z*n.z, 0,
                   0, 0, 0, 0 );

    D3DXMATRIX Mz( 0,   0,   0,  0,
                   0,   0,   0,  0,
                   0,   0,   0,  0,
                   n.x, n.y, n.z, -L_dot_n );

    return -( M1 - M2 + M3 + Mz );
}

LightSource::LightSource( IDirect3DDevice9 *device, D3DPRIMITIVETYPE primitive_type, VertexShader &vertex_shader, PixelShader &pixel_shader, const TexturedVertex *vertices,
                          unsigned vertices_count, const Index *indices, unsigned indices_count, unsigned primitives_count,
                          D3DXVECTOR3 position, D3DXVECTOR3 rotation, float radius )
: Model(device, primitive_type, vertex_shader, vertex_shader, pixel_shader, pixel_shader, TexturedVertex::get_declaration(device), sizeof(TexturedVertex), vertices, vertices_count, indices, indices_count,
        primitives_count, position, rotation), radius(radius)
{}

unsigned LightSource::set_constants(D3DXVECTOR4 *out_data, unsigned buffer_size) const
// returns number of constant registers used
{
    _ASSERT( buffer_size >= LIGHT_SOURCE_CONSTANTS_USED); // enough space?
    out_data[0] = D3DXVECTOR4(radius, radius, radius, radius);
    return LIGHT_SOURCE_CONSTANTS_USED;
}
