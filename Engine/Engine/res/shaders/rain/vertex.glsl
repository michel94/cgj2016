#version 420

in vec4 in_Position;
in vec2 inTexcoord;
in vec3 inNormal;

out vec2 ex_Texcoord;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
};

uniform mat4 Matrix;
uniform vec4 Color;

void main(void){
	ex_Texcoord = inTexcoord;
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}
