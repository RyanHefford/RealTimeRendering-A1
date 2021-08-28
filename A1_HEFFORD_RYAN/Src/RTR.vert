#version 400
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in mat4 model_Instance;

out VERT_DATA
{
    vec3 Position;
	vec3 FragPos;
	mat4 CurrModel;
} data_out;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;
uniform float u_learpPercentage;

void main(){
	vec3 currentPosition = vertex_position;

	//check if lerping is being used
	if(u_learpPercentage != 0){
		float radius = (abs(vertex_position.x) > abs(vertex_position.y) ? vertex_position.x : vertex_position.y);
        radius = abs(radius) > abs(vertex_position.z) ? radius : vertex_position.z;
        radius = abs(radius);

        vec3 endPoint = vec3(vertex_position.x, vertex_position.y, vertex_position.z);
        endPoint = normalize(endPoint);

        endPoint *= radius;

		currentPosition = (1.0f - u_learpPercentage) * currentPosition + u_learpPercentage * endPoint;
	}

	//check for instances
	if(u_model != 0){
		data_out.CurrModel = u_model;
		data_out.Position = currentPosition;
		data_out.FragPos = vec3(u_model * vec4(currentPosition, 1.0));
		gl_Position = u_proj * u_view * u_model * vec4(currentPosition, 1.0);
	}else{
		data_out.CurrModel = model_Instance;
		data_out.Position = currentPosition;
		data_out.FragPos = vec3(model_Instance * vec4(currentPosition, 1.0));
		gl_Position = u_proj * u_view * model_Instance * vec4(currentPosition, 1.0);
	}
}