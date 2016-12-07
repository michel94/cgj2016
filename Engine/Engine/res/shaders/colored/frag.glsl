#version 450

in vec4 ex_Color;
out vec4 out_Color;

in vec3 ex_Normal;
in vec2 ex_Texcoord;

uniform vec4 Color;
uniform sampler2D tex;

void main(void){
	//out_Color = ex_Color;
	out_Color = vec4(0.2, 0.2, 0.8, 1);
	//out_Color = vec4(ex_Texcoord, 1, 1);
	//out_Color = vec4(0.5,0.5,0.5,1);
}
