#version 400


#define RTRDirectionalLight    0
#define RTRPointLight 1 

struct RTRLight {
	int Type;               // Required for light type ...
	vec3 Ambient;           //   Directional, Point
	vec3 Diffuse;           //   Directional, Point
	vec3 Specular;          //   Directional, Point
	vec3 Direction;         //   Directional
	vec3 Position;          //   Point
	float Constant;         //   Point
	float Linear;           //   Point
	float Quadratic;		//   Point
};


struct RTRMaterial {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

#define RTR_MAX_LIGHTS 10
#define RTR_MAX_MATERIAL 3

uniform RTRLight u_lights[RTR_MAX_LIGHTS];
uniform RTRMaterial u_materials[RTR_MAX_MATERIAL];
uniform int u_numLights;
uniform bool u_lightsEnabled;


in GEOM_DATA
{
    vec3 FragNormal;
    flat int MaterialIndex;
    vec3 FragPos;
} data_in;

out vec4 fragment_color; 
out vec3 fragment_normal;

uniform vec3 u_camPosition;

vec3 CreateDirLight(RTRLight currLight, vec3 viewDir);
vec3 CreatePointLight(RTRLight currLight, vec3 viewDir);

void main() {

	fragment_normal = normalize(data_in.FragNormal);
	vec3 viewDir = normalize(u_camPosition - data_in.FragPos);

	vec3 lightingOutput = vec3(0,0,0);

	//create lights

	for(int i = 0; i < u_numLights; i++){
		if(u_lights[i].Type == RTRDirectionalLight){
			lightingOutput += CreateDirLight(u_lights[i], viewDir);
		}else{
			lightingOutput += CreatePointLight(u_lights[i], viewDir);
		}
	}

	fragment_color = vec4(lightingOutput, 1.0);

}

vec3 CreateDirLight(RTRLight currLight, vec3 viewDir){
	
	//ambient lighting
	vec3 ambient = u_materials[data_in.MaterialIndex].Ambient * currLight.Ambient;

	//diffuse lighting
	vec3 lightDir = normalize(-currLight.Direction);
	float diff = max(dot(data_in.FragNormal, lightDir), 0.0);
	vec3 diffuse = currLight.Diffuse * (diff * u_materials[data_in.MaterialIndex].Diffuse);

	if(u_lightsEnabled){
		//specular lighting
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(data_in.FragNormal, halfwayDir), 0.0), u_materials[data_in.MaterialIndex].Shininess);
		vec3 specular = currLight.Specular * (u_materials[data_in.MaterialIndex].Specular * spec);


		return (ambient + diffuse + specular);
	}else{
		return (ambient + diffuse);
	}
}

vec3 CreatePointLight(RTRLight currLight, vec3 viewDir){
	
	//ambient lighting
	vec3 ambient = u_materials[data_in.MaterialIndex].Ambient * currLight.Ambient;

	//diffuse lighting
	vec3 lightDir = normalize(currLight.Position - data_in.FragPos);
	float diff = max(dot(data_in.FragNormal, lightDir), 0.0);
	vec3 diffuse = currLight.Diffuse * (diff * u_materials[data_in.MaterialIndex].Diffuse);

	//calculate attenuation
	float distance = length(currLight.Position - data_in.FragPos);
	float attenuation = 1.0 / (currLight.Constant + currLight.Linear * distance + currLight.Quadratic * (distance * distance));

	//apply attenuation
	ambient *= attenuation;
	diffuse *= attenuation;

	if(u_lightsEnabled){
		//specular lighting
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(data_in.FragNormal, halfwayDir), 0.0), u_materials[data_in.MaterialIndex].Shininess);
		vec3 specular = currLight.Specular * (u_materials[data_in.MaterialIndex].Specular * spec);

		specular *= attenuation;

		return (ambient + diffuse + specular);
	}else{
		return (ambient + diffuse);
	}
}