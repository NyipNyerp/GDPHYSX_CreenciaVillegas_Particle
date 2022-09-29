#version 330 core
layout(location = 0) in vec3 a_vertex;

out vec3 TexCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
	TexCoords = a_vertex;
	gl_Position = u_projection * u_view * vec4(a_vertex, 1.0);
}