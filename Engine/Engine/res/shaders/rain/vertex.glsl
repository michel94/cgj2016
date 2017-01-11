#version 330

in vec4 in_Position;
in vec2 inTexcoord;
in vec3 inNormal;

out vec2 ex_Texcoord;

in vec4 in_Color;
out vec4 ex_Color;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;
uniform vec4 Color;

void main(void){
	ex_Color = in_Color;
	ex_Texcoord = inTexcoord;
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}
