// Modified from:-
// Bailey (2023) GLSL Geometry Shaders (p. 16)
// Accessed [22.01.2024]
// Available from https://web.engr.oregonstate.edu/~mjb/cs519/Handouts/geometry_shaders.1pp.pdf

#version 420 core

#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable
layout(lines_adjacency) in;
layout(line_strip, max_vertices = 128) out;
uniform float steps = 0.1f;
uniform int uNum = 20;
vec4 newPos;

vec4 bezier(in float t, in vec4 p0, in vec4 p1, in vec4 p2, in vec4 p3)
{
        float B0 = (1.0 - t) * (1.0 -t) * (1.0 - t);
        float B1 = 3.0 * t * (1.0 - t) * (1.0 - t);
        float B2 = 3.0 * t * t * (1-t);
        float B3 = t * t * t;

        vec4 p = B0 * p0 + B1 * p1 + B2 * p2 + B3 * p3;
        return p;
}

void main()
{
        float dt = 1.0f / float(uNum);
        float t = 0;
        for(int i = 0; i <= uNum; ++i)
        {
                newPos = bezier(t,gl_PositionIn[0].xyzw, gl_PositionIn[1].xyzw, gl_PositionIn[2].xyzw, gl_PositionIn[3].xyzw);
                gl_Position = newPos;
                EmitVertex();
                t += dt;

        }

}

//endOfCitation