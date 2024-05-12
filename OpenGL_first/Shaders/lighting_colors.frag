#version 460 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
	
};

struct Light {
	vec3 position; // no longer necessary when using directional lights.
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

vec3 calculate_emission()
{
	vec3 show = step(vec3(1.0), vec3(1.0) - texture(material.specular, TexCoords).rgb);
	return texture(material.emission, TexCoords).rgb * show;
}

void main()
{
	


	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	//	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TexCoords).rgb;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos); // for the viewspace = -FragPos
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) ;
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	// spotlight
//	float theta = dot(lightDir, normalize(-light.direction));
//	float epsilon = light.cutOff - light.outerCutOff;
//	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//	diffuse *= intensity;
//	specular *= intensity;

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;


	// emission
	vec3 emission;// = calculate_emission();

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);


}

