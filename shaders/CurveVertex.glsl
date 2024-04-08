// Modified from:-
// Bailey (2023) GLSL Geometry Shaders (p. 15)
// Accessed [22.01.2024]
// Available from https://web.engr.oregonstate.edu/~mjb/cs519/Handouts/geometry_shaders.1pp.pdf


#version 420 core
layout (location = 0) in vec3 inPos;

uniform mat4 MVP;

void main()
{
        gl_Position = MVP * vec4(inPos, 1);
}