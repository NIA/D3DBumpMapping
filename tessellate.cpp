#include "tessellate.h"

namespace
{
    const float TEXTURE_REPEATS = 1;

    const float U_OFFSETS[PLANES_PER_PYRAMID/2] = {0, 0.5f, 0.5f, 1};
    const float U_SHIFTS_WHEN_X_IS_0[PLANES_PER_PYRAMID/2] = {1.0f/4, -1.0f/4, 1.0f/4, -1.0f/4};
    const float V_OFFSETS[2] = {0, 1.0f};

    const float EPSILON = 1e-4f;
    bool is_zero(float a)
    {
        return abs(a) < EPSILON;
    }
}

void tessellate(const TexturedVertex *src_vertices, const Index *src_indices, DWORD src_index_offset,
                TexturedVertex *res_vertices, Index res_vertices_offset, Index *res_indices,
                D3DCOLOR color, DWORD tesselate_degree, unsigned quater, unsigned half,
                bool make_sphere /*= false*/, float final_radius /*= 1.0f*/)
// Divides each side of triangle into given number of parts
// Writes data into arrays given as `res_vertices' and `res_indices',
//   assuming that there are already `res_vertices_offset' vertices before `res_vertices' pointer.
{
    _ASSERT(src_vertices != NULL);
    _ASSERT(src_indices != NULL);
    _ASSERT(res_vertices != NULL);
    _ASSERT(res_indices != NULL);
    _ASSERT(tesselate_degree != 0);
    // i1, i2 i3 are indices of source triangle vertices
    const Index i1 = src_indices[src_index_offset];
    const Index i2 = src_indices[src_index_offset + 1];
    const Index i3 = src_indices[src_index_offset + 2];
    const D3DXVECTOR3 step_down = (src_vertices[i1].pos - src_vertices[i2].pos)/static_cast<float>(tesselate_degree);
    const D3DXVECTOR3 step_right = (src_vertices[i3].pos - src_vertices[i1].pos)/static_cast<float>(tesselate_degree);

    D3DXVECTOR3 start_pos = src_vertices[i2].pos;
    Index vertex = 0; // current vertex
    DWORD index = 0; // current index
    
    for( Index line = 0; line <= tesselate_degree; ++line )
    {
        for( Index column = 0; column < line + 1; ++column ) // line #1 contains 2 vertices
        {
            D3DXVECTOR3 position = start_pos
                                 + static_cast<FLOAT>(line)*step_down
                                 + static_cast<FLOAT>(column)*step_right;
            if( make_sphere )
            {
                D3DXVec3Normalize(&position, &position);
                position = position * final_radius;
            }
            
            D3DXVECTOR3 normalized_position;
            D3DXVec3Normalize(&normalized_position, &position);
            float x = normalized_position.x;
            float y = normalized_position.y;
            float z = normalized_position.z;
            float u = ( is_zero(x) ? U_SHIFTS_WHEN_X_IS_0[quater] : atan( y/x )/D3DX_PI/2 ) + U_OFFSETS[quater];
            float v = is_zero(z) ? 0.5f : (atan( sqrt(x*x + y*y)/z )/D3DX_PI  + V_OFFSETS[half]);

            D3DXVECTOR3 binormal( cos(D3DX_PI*v)*cos(2*D3DX_PI*u), cos(D3DX_PI*v)*sin(2*D3DX_PI*u), -sin(D3DX_PI*v) );
            D3DXVECTOR3 tangent( -sin(2*D3DX_PI*u), cos(2*D3DX_PI*u),  0 );
            D3DXVECTOR3 normal;
            D3DXVec3Cross(&normal, &binormal, &tangent);
            
            res_vertices[vertex] = TexturedVertex( position, color, normal, TEXTURE_REPEATS*u, TEXTURE_REPEATS*v, binormal, tangent );
            if( column != 0 ) // not first coumn
            {
                // add outer triangle
                add_triangle( vertex, vertex - 1, vertex - line - 1, res_indices, index, res_vertices_offset );
            }
            if( ( column != 0 ) && ( column != line ) ) // not first and not last column
            {
                // add inner triangle
                add_triangle(  vertex, vertex - line - 1, vertex - line, res_indices, index, res_vertices_offset );
            }
            ++vertex;
        }
    }
}
