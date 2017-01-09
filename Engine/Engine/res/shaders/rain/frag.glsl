#version 420

out vec4 out_Color;

in vec3 ex_Normal;
in vec2 ex_Texcoord;

uniform vec4 Color;
uniform sampler2D tex;
uniform sampler2D normalTex;

void main(void){
		//out_Color = ex_Color;
	out_Color = vec4(0.5, 0.60, 0.90, 1);
}
