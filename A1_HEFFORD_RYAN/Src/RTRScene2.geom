#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Normal;
out vec3 Color;
out vec3 Position;

in DATA
{
    vec3 Position;
} data_in[];

void main() {

    vec3 calculatedNormal = normalize(cross((data_in[1].Position - data_in[0].Position), (data_in[2].Position - data_in[0].Position)));
    vec3 calculatedColor = abs(calculatedNormal);

    Color = calculatedColor;
    Normal = calculatedNormal;
    Position = vec3(data_in[0].Position);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    Color = calculatedColor;
    Normal = calculatedNormal;
    Position = vec3(data_in[1].Position);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    Color = calculatedColor;
    Normal = calculatedNormal;
    Position = vec3(data_in[2].Position);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();


    EndPrimitive();
}