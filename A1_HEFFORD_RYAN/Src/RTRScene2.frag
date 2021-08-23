#version 330 core
in vec3 Color, Normal;
in vec3 Position;
out vec4 fragment_color; 
out vec3 fragment_normal;

void main() {
	//fragment_color = vec4(Color, 1.0f);
	fragment_normal = Normal;

	//create lights
	float ambientStrength = 0.1;
	float diffuseStrength = 0.9;
	vec3 lightPos = vec3(0,0,0);

	vec3 lightColor = vec3(1,1,1);
	vec3 lightDir = normalize(lightPos - Position);
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 ambient = ambientStrength * lightColor;
	vec3 result = (ambient + diffuse) * Color;
	fragment_color = vec4(result, 1.0);

}