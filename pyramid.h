#pragma once
#include "main.h"
#include "Vertex.h"
#include "tessellate.h"

inline Index pyramid_vertices_count(DWORD tessellate_degree)
{
    return PLANES_PER_PYRAMID*tesselated_vertices_count(tessellate_degree);
}
inline DWORD pyramid_indices_count(DWORD tessellate_degree)
{
    return PLANES_PER_PYRAMID*tesselated_indices_count(tessellate_degree);
}

void pyramid( float side, TexturedVertex *res_vertices, Index *res_indices,
              D3DCOLOR color, DWORD tesselate_degree,
              bool make_sphere = false, float final_radius = 1.0f);
