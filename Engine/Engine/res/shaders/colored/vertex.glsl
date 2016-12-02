#version 450

in vec4 in_Position;
in vec2 inTexcoord;
in vec3 inNormal;

out vec2 ex_Texcoord;

in vec4 in_Color;
out vec4 ex_Color;


	
uniform mat4 Matrix;
uniform vec4 Color;

void main(void){
	ex_Color = in_Color;
	ex_Texcoord = inTexcoord;
	gl_Position = Matrix * in_Position;
}
