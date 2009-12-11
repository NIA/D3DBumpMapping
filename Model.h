#pragma once
#include "main.h"
#include "Vertex.h"
#include "shaders.h"

class Model
{
private:
    IDirect3DDevice9    *device;

    VertexDeclaration   &vertex_declaration;
    VertexShader        &vertex_shader;
    VertexShader        &shadow_vertex_shader;
    PixelShader         &pixel_shader;

    unsigned    vertices_count;
    unsigned    primitives_count;

    D3DPRIMITIVETYPE        primitive_type;
    IDirect3DVertexBuffer9  *vertex_buffer;
    IDirect3DIndexBuffer9   *index_buffer;
    unsigned vertex_size;

    D3DXVECTOR3 position;
    D3DXVECTOR3 rotation;
    D3DXMATRIX rotation_and_position;

    void update_matrix();

    void release_interfaces();

public:
    Model(  IDirect3DDevice9 *device,
            D3DPRIMITIVETYPE primitive_type,
            VertexShader &vertex_shader,
            VertexShader &shadow_vertex_shader,
            PixelShader &pixel_shader,
            VertexDeclaration &vertex_declaration,
            unsigned vertex_size,
            const Vertex *vertices,
            unsigned vertices_count,
            const Index *indices,
            unsigned indices_count,
            unsigned primitives_count,
            D3DXVECTOR3 position,
            D3DXVECTOR3 rotation);
    
    void set_shaders_and_decl(bool shadow)
    {
        vertex_declaration.set();
        shadow ? shadow_vertex_shader.set() : vertex_shader.set();
        pixel_shader.set();
    }
    virtual void set_time(float time) { UNREFERENCED_PARAMETER(time); }
    
    // set_constants() returns number of constant registers used
    virtual unsigned set_constants(D3DXVECTOR4 *out_data, unsigned buffer_size) const { UNREFERENCED_PARAMETER(out_data); UNREFERENCED_PARAMETER(buffer_size); return 0; }
    
    const D3DXMATRIX &get_rotation_and_position() const;
    void rotate(float phi);
    
    virtual void draw() const;

    virtual ~Model();
private:
    // No copying!
    Model(const Model&);
    Model &operator=(const Model&);
};

class TexturedMorphingModel : public Model
{
private:
    float morphing_param;
    float final_radius;
public:
    TexturedMorphingModel(  IDirect3DDevice9 *device,
                            D3DPRIMITIVETYPE primitive_type,
                            VertexShader &vertex_shader,
                            VertexShader &shadow_vertex_shader,
                            PixelShader &pixel_shader,
                            const TexturedVertex *vertices,
                            unsigned vertices_count,
                            const Index *indices,
                            unsigned indices_count,
                            unsigned primitives_count,
                            D3DXVECTOR3 position,
                            D3DXVECTOR3 rotation,
                            float final_radius);

    // Overrides:
    virtual void set_time(float time);
    virtual unsigned set_constants(D3DXVECTOR4 *out_data, unsigned buffer_size) const; // returns number of constant registers used
};

class Plane : public Model
{
private:
    D3DXVECTOR3 normal;
    float d; // coeff. in plane equation (x,n)=d
public:
    Plane(  IDirect3DDevice9 *device,
            D3DPRIMITIVETYPE primitive_type,
            VertexShader &vertex_shader,
            PixelShader &pixel_shader,
            const Vertex *vertices,
            unsigned vertices_count,
            const Index *indices,
            unsigned indices_count,
            unsigned primitives_count,
            D3DXVECTOR3 position,
            D3DXVECTOR3 rotation);

    D3DXMATRIX get_projection_matrix(const D3DXVECTOR3 light_position) const;
};

class LightSource : public Model
{
private:
    float radius;
public:
    LightSource( IDirect3DDevice9 *device,
                 D3DPRIMITIVETYPE primitive_type,
                 VertexShader &vertex_shader,
                 PixelShader &pixel_shader,
                 const TexturedVertex *vertices,
                 unsigned vertices_count,
                 const Index *indices,
                 unsigned indices_count,
                 unsigned primitives_count,
                 D3DXVECTOR3 position,
                 D3DXVECTOR3 rotation,
                 float radius);
    // Overrides:
    virtual unsigned set_constants(D3DXVECTOR4 *out_data, unsigned buffer_size) const; // returns number of constant registers used
};
