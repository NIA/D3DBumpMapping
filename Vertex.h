#pragma once
#include "main.h"

typedef DWORD Index;
extern const D3DFORMAT INDEX_FORMAT;

// They must be macros, not constants, because they must be known at compile-time (they are used for array initialization in another module)
#define VERTICES_PER_TRIANGLE 3
#define PLANES_PER_PYRAMID 8

inline int rand_col_comp()
// Returns random color component: an integer between 0 and 255
{
    return rand()*255/RAND_MAX;
}

inline D3DCOLOR random_color()
{
    return D3DCOLOR_XRGB( rand_col_comp(), rand_col_comp(), rand_col_comp() );
}

class Vertex
{
public:
    D3DXVECTOR3 pos;            // The position for the vertex
    D3DXVECTOR4 normal;         // The outer normal of model
    D3DCOLOR color;             // The vertex color
    void set_normal(D3DXVECTOR3 tri_normal)
    {
        normal = D3DXVECTOR4(tri_normal, 0); // normal is a vector, not a point!
    }
    Vertex() : color(0)
    {
        set_normal(D3DXVECTOR3(1.0f, 0, 0));
    }
    Vertex(D3DXVECTOR3 pos, D3DCOLOR color, D3DXVECTOR3 normal) : pos(pos), color(color)
    {
        set_normal(normal);
    }
    Vertex(D3DXVECTOR3 pos, D3DXVECTOR3 normal) : pos(pos)
    {
        color = random_color();
        set_normal(normal);
    }
};

class TexturedVertex : public Vertex
{
public:
    float u, v;                 // The texture coordinates
    TexturedVertex() : u(0), v(0) {}
    TexturedVertex(D3DXVECTOR3 pos, D3DCOLOR color, D3DXVECTOR3 normal, float u, float v)
        : Vertex(pos, color, normal), u(u), v(v) {}
    TexturedVertex(D3DXVECTOR3 pos, D3DXVECTOR3 normal, float u, float v)
        : Vertex(pos, normal), u(u), v(v) {}
};

// a helper for generation functions (tesselate() and plane())
inline void add_triangle( Index i1, Index i2, Index i3, Index *indices, DWORD &current_index, Index offset = 0 )
{
    indices[current_index++] = i1 + offset;
    indices[current_index++] = i2 + offset;
    indices[current_index++] = i3 + offset;
}

extern const D3DVERTEXELEMENT9 VERTEX_DECL_ARRAY[];
extern const D3DVERTEXELEMENT9 TEXTURED_VERTEX_DECL_ARRAY[];
