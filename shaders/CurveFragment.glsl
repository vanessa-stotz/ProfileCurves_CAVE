// Modified from:-
// Bailey (2023) GLSL Geometry Shaders (p. 15)
// Accessed [22.01.2024]
// Available from https://web.engr.oregonstate.edu/~mjb/cs519/Handouts/geometry_shaders.1pp.pdf

#version 420 core

layout( location = 0 ) out vec4 fragColour;
in vec4 colour;

void main()
{
    fragColour = vec4(1,0,0,1);
}