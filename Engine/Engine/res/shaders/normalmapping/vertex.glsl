#version 330

in vec4 in_Position;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;

out vec2 ex_Texcoord;

in vec4 in_Color;
out vec3 ex_Normal;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
};

uniform mat4 Matrix;
uniform vec4 Color;

void main(void){
	mat4 MV = ViewMatrix * Matrix;
	vec3 normalViewSpace = inNormal;
	vec3 tangentViewSpace = inTangent;
	vec3 bitangentViewSpace = cross(inNormal, inTangent);
	mat3 TBN = transpose(mat3(
		tangentViewSpace,
		bitangentViewSpace,
		normalViewSpace
	));
	
	ex_Normal = inNormal;
	ex_Texcoord = inTexcoord;
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}
