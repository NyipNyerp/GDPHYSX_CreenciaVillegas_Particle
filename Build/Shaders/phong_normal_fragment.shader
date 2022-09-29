#version 330 core
uniform vec3 u_color;
out vec4 FragColor;
in vec3 FragPos;
in vec2 UV;
in vec3 Normal;
in mat3 TBN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D night_diffuse;

uniform vec3 u_light_pos;
uniform vec3 u_camera_pos;
uniform vec3 u_ambient_color;
uniform float u_shader_mode;

float attenuate(float value, float maximum)
{
	float clampedValue = min(value, maximum);
	return 1.0 / (pow(5 * clampedValue / maximum, 2) + 1);
}

void main()
{
	vec3 lightVector = normalize(u_light_pos - FragPos);

	float distance = length(u_light_pos - FragPos);
	float gradient = attenuate(distance, 35.0);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec3 tbnNormal = texture(texture_normal, UV).rgb;
	tbnNormal = tbnNormal * 2.0 - 1.0; // convert from pixel to vector
	tbnNormal = normalize(TBN * tbnNormal);
	if (u_shader_mode != 3.0f)
	{
		tbnNormal = Normal;
	}

	float specularStrength = 1.0;

	vec3 viewDir = normalize(u_camera_pos - FragPos);
	vec3 reflectDir = reflect(-lightVector, tbnNormal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 4);

	vec3 specular = specularStrength * spec * lightColor;
	float NdotL = max(dot(tbnNormal, lightVector), 0.0);
	vec3 diffuse = vec3(NdotL) * lightColor;
	vec3 ambient = u_ambient_color * lightColor;

	if (u_shader_mode == 0.0f) // basic
	{
		FragColor = texture(texture_diffuse, UV);
	}
	else if (u_shader_mode == 1.0f) // phong
	{
		FragColor = vec4(ambient + (diffuse + specular) * gradient, 1.0) * texture(texture_diffuse, UV);
	}
	else if (u_shader_mode == 2.0f) // phong multitexturing
	{
		FragColor = vec4(ambient + (diffuse + specular), 1.0) * texture(texture_diffuse, UV) + texture(night_diffuse, UV) * (1.0 - NdotL);
	}
	else if (u_shader_mode == 3.0f) // phong normal
	{
		//FragColor = texture(texture_normal, UV);
		FragColor = vec4(ambient + (diffuse + specular), 1.0) * texture(texture_diffuse, UV);
	}
}
