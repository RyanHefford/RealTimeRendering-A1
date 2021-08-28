#version 400
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


out GEOM_DATA
{
    vec3 FragNormal;
    flat int MaterialIndex;
    vec3 FragPos;
} data_out;

in VERT_DATA
{
    vec3 Position;
	vec3 FragPos;
	mat4 CurrModel;
} data_in[];


void main() {

    vec3 calculatedNormal = normalize(cross((data_in[1].Position - data_in[0].Position), (data_in[2].Position - data_in[0].Position)));
    mat3 model = mat3(data_in[0].CurrModel);
    calculatedNormal = mat3(transpose(inverse(model))) * calculatedNormal;

    data_out.MaterialIndex = gl_PrimitiveIDIn / 4 % 3;

    data_out.FragNormal = calculatedNormal;
    data_out.FragPos = data_in[0].FragPos;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    data_out.FragNormal = calculatedNormal;
    data_out.FragPos = data_in[1].FragPos;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    data_out.FragNormal = calculatedNormal;
    data_out.FragPos = data_in[2].FragPos;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();


    EndPrimitive();
}
