#include "main.h"
#include "Application.h"
#include "Model.h"
#include "plane.h"
#include "pyramid.h"

namespace
{
    const char *SPHERE_SHADER_FILENAME = "sphere.vsh";
    const char *SPHERE_SHADOW_SHADER_FILENAME = "sphere_shadow.vsh";
    const char *PLANE_SHADER_FILENAME = "plane.vsh";
    const char *LIGHT_SOURCE_SHADER_FILENAME = "light_source.vsh";
    const char *SPHERE_PIXEL_SHADER_FILENAME = "sphere.psh";

    const char *SPHERE_TEXTURE_FILENAME = "brickdiff.tga";//"nia.jpg";//
    const D3DCOLOR SPHERE_COLOR = D3DCOLOR_XRGB(255, 190, 0);
    const D3DCOLOR PLANE_COLOR = D3DCOLOR_XRGB(50,150,80);

    const float SPHERE_RADIUS = sqrt(2.0f);
    const float LIGHT_SOURCE_RADIUS = 0.04f;

    const DWORD SPHERE_TESSELATE_DEGREE = 30;
    const Index SPHERE_ALL_TESSELATED_VERTICES_COUNT = pyramid_vertices_count(SPHERE_TESSELATE_DEGREE); // per 8 tessellated triangles
    const DWORD SPHERE_ALL_TESSELATED_INDICES_COUNT = pyramid_indices_count(SPHERE_TESSELATE_DEGREE); // per 8 tessellated triangles

    const DWORD LIGHT_SOURCE_TESSELATE_DEGREE = 10;
    const Index LIGHT_SOURCE_ALL_TESSELATED_VERTICES_COUNT = PLANES_PER_PYRAMID*tesselated_vertices_count(LIGHT_SOURCE_TESSELATE_DEGREE); // per 8 tessellated triangles
    const DWORD LIGHT_SOURCE_ALL_TESSELATED_INDICES_COUNT = PLANES_PER_PYRAMID*tesselated_indices_count(LIGHT_SOURCE_TESSELATE_DEGREE); // per 8 tessellated triangles

}

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, INT )
{
    srand( static_cast<unsigned>( time(NULL) ) );
    
    TexturedVertex * sphere_vertices = NULL;
    Index * sphere_indices = NULL;
    Vertex * plane_vertices = NULL;
    Index * plane_indices = NULL;
    TexturedVertex * light_source_vertices = NULL;
    Index * light_source_indices = NULL;
    try
    {
        try
        {
            Application app;

            VertexShader sphere_vertex_shader(app.get_device(), SPHERE_SHADER_FILENAME);
            VertexShader sphere_shadow_vertex_shader(app.get_device(), SPHERE_SHADOW_SHADER_FILENAME);
            VertexShader plane_vertex_shader(app.get_device(), PLANE_SHADER_FILENAME);
            VertexShader light_source_vertex_shader(app.get_device(), LIGHT_SOURCE_SHADER_FILENAME);
            PixelShader no_pixel_shader(app.get_device());
            PixelShader sphere_pixel_shader(app.get_device(), SPHERE_PIXEL_SHADER_FILENAME);

            Texture sphere_texture(app.get_device(), SPHERE_TEXTURE_FILENAME);
            
            // -------------------------- P y r a m i d -----------------------
            sphere_vertices = new TexturedVertex[SPHERE_ALL_TESSELATED_VERTICES_COUNT];
            sphere_indices = new Index[SPHERE_ALL_TESSELATED_INDICES_COUNT];

            pyramid(SPHERE_RADIUS*sqrt(2.0f), sphere_vertices, sphere_indices, SPHERE_COLOR, SPHERE_TESSELATE_DEGREE, true, SPHERE_RADIUS);
            
            TexturedModel sphere( app.get_device(),
                                  D3DPT_TRIANGLELIST,
                                  sphere_vertex_shader,
                                  sphere_shadow_vertex_shader,
                                  sphere_pixel_shader,
                                  no_pixel_shader,
                                  sphere_vertices,
                                  SPHERE_ALL_TESSELATED_VERTICES_COUNT,
                                  sphere_indices,
                                  SPHERE_ALL_TESSELATED_INDICES_COUNT,
                                  SPHERE_ALL_TESSELATED_INDICES_COUNT/VERTICES_PER_TRIANGLE,
                                  D3DXVECTOR3(0, 0, 0),
                                  D3DXVECTOR3(0,0,0),
                                  sphere_texture);

            // ----------------------------- P l a n e --------------------------
            plane_vertices = new Vertex[PLANE_VERTICES_COUNT];
            plane_indices = new Index[PLANE_INDICES_COUNT];
            plane(40, 40, plane_vertices, plane_indices, PLANE_COLOR);

            Plane plane( app.get_device(),
                         D3DPT_TRIANGLELIST,
                         plane_vertex_shader,
                         no_pixel_shader,
                         plane_vertices,
                         PLANE_VERTICES_COUNT,
                         plane_indices,
                         PLANE_INDICES_COUNT,
                         PLANE_INDICES_COUNT/VERTICES_PER_TRIANGLE,
                         D3DXVECTOR3(0, 0, -2.0f),
                         D3DXVECTOR3(0,0,0) );

            // -------------------------- Light source --------------------------
            light_source_vertices = new TexturedVertex[LIGHT_SOURCE_ALL_TESSELATED_VERTICES_COUNT];
            light_source_indices = new Index[LIGHT_SOURCE_ALL_TESSELATED_INDICES_COUNT];

            pyramid(LIGHT_SOURCE_RADIUS*LIGHT_SOURCE_RADIUS, light_source_vertices, light_source_indices, D3DCOLOR_XRGB(0,0,0) /* ignored */, LIGHT_SOURCE_TESSELATE_DEGREE);

            LightSource light_source( app.get_device(),
                                      D3DPT_TRIANGLELIST,
                                      light_source_vertex_shader,
                                      no_pixel_shader,
                                      light_source_vertices,
                                      LIGHT_SOURCE_ALL_TESSELATED_VERTICES_COUNT,
                                      light_source_indices,
                                      LIGHT_SOURCE_ALL_TESSELATED_INDICES_COUNT,
                                      LIGHT_SOURCE_ALL_TESSELATED_INDICES_COUNT/VERTICES_PER_TRIANGLE,
                                      D3DXVECTOR3(0,0,0),
                                      D3DXVECTOR3(0,0,0),
                                      LIGHT_SOURCE_RADIUS);

            // ---------------------------- a d d i n g -------------------------
            app.add_model(sphere);
            app.set_plane(plane);
            app.set_light_source_model(light_source);

            app.run();

            // -------------------------- c l e a n   u p -----------------------
            delete_array(&sphere_indices);
            delete_array(&sphere_vertices);
            delete_array(&plane_indices);
            delete_array(&plane_vertices);
            delete_array(&light_source_indices);
            delete_array(&light_source_vertices);
        }
        catch(std::bad_alloc)
        {
            throw NoMemoryError();
        }
    }
    catch(RuntimeError &e)
    {
        delete_array(&sphere_indices);
        delete_array(&sphere_vertices);
        delete_array(&plane_indices);
        delete_array(&plane_vertices);
        delete_array(&light_source_indices);
        delete_array(&light_source_vertices);
        const TCHAR *MESSAGE_BOX_TITLE = _T("BumpMapping error!");
        MessageBox(NULL, e.message(), MESSAGE_BOX_TITLE, MB_OK | MB_ICONERROR);
        return -1;
    }
    return 0;
}
