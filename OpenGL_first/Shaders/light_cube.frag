#version 460 core
out vec4 FragColor;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse;
	vec3 specular = light.specular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
//	FragColor = vec4(1.0);
}