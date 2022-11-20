#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal;
out vec4 vertexColor;
out vec3 Normal;
out vec3 FragPos;

out vec2 UV;
void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
	UV = a_UV;
	Normal = normalize(mat3(u_normal) * a_normal);

}