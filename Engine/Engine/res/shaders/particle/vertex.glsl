#version 330

in vec4 in_Position;
in vec4 inColor;

out vec2 ex_Texcoord;

out vec4 ex_Color;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;

void main(void){
	ex_Color = inColor;
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}
