#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_normal;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec3 u_light_pos;
uniform mat4 u_normal;
uniform vec3 u_camera_pos;
uniform vec3 u_ambient_color;

out vec2 UV;
out vec3 o_normal;
out vec3 vertexColor;
void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
    UV = a_UV;
    o_normal = normalize(mat3(u_normal)*a_normal);

    vec3 vertexPos = vec3(u_model * vec4(a_pos, 1.0));
    vec3 lightVector = normalize(u_light_pos - vertexPos);

    vec3 lightColor = vec3(1.0, 1.0, 1.0);


    float specularStrength = 1.0;
    vec3 viewDir = normalize(u_camera_pos - vertexPos);
    vec3 reflectDir = reflect(-lightVector, o_normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 8);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 diffuse = vec3(max(dot(a_normal, lightVector), 0.0));// * diffuse_color;
    vec3 ambient = u_ambient_color * lightColor;
    vertexColor = u_ambient_color + diffuse + specular;
}