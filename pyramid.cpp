#include "pyramid.h"

void pyramid( float side, TexturedVertex *res_vertices, Index *res_indices,
              D3DCOLOR color, DWORD tesselate_degree,
              bool make_sphere /*= false*/, float final_radius /*= 1.0f*/)
{
    const D3DXVECTOR3 normal_up(0,0,1);
    const float half_side = side/2.0f;
    const TexturedVertex pyramid_vertices[]=
    {
        TexturedVertex( D3DXVECTOR3(  half_side,          0,  0.00f ), normal_up, 0, 0 ),
        TexturedVertex( D3DXVECTOR3(          0,  half_side,  0.00f ), normal_up, 0, 0 ),
        TexturedVertex( D3DXVECTOR3( -half_side,          0,  0.00f ), normal_up, 0, 0 ),
        TexturedVertex( D3DXVECTOR3(          0, -half_side,  0.00f ), normal_up, 0, 0 ),

        TexturedVertex( D3DXVECTOR3(  0.0f,  0.0f,  side/sqrt(2.0f) ), normal_up, 0, 0 ),
        TexturedVertex( D3DXVECTOR3(  0.0f,  0.0f, -side/sqrt(2.0f) ), normal_up, 0, 0 ),
    };
    const Index pyramid_indices[PLANES_PER_PYRAMID*VERTICES_PER_TRIANGLE] =
    {
        0, 4, 1,
        1, 4, 2,
        2, 4, 3,
        3, 4, 0,

        1, 5, 0,
        2, 5, 1,
        3, 5, 2,
        0, 5, 3,
    };

    for( DWORD i = 0; i < PLANES_PER_PYRAMID; ++i )
    {
        //DWORD i = 0;
        tessellate( pyramid_vertices, pyramid_indices, i*VERTICES_PER_TRIANGLE,
                    &res_vertices[i*tesselated_vertices_count(tesselate_degree)], i*tesselated_vertices_count(tesselate_degree),
                    &res_indices[i*tesselated_indices_count(tesselate_degree)], color, tesselate_degree, i%4, i/4, make_sphere, final_radius );
    }
}