#version 330 core
layout (location = 0) in vec3 vertex_position;

out DATA
{
    vec3 Position;
} data_out;

uniform mat4 u_proj;
uniform mat4 u_view;

void main(){
	data_out.Position = vertex_position;
	gl_Position = u_proj * u_view * vec4(vertex_position, 1.0);
}