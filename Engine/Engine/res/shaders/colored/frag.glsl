#version 330

in vec4 ex_Color;
out vec4 out_Color;

uniform vec4 Color;

void main(void){
	out_Color = vec4(ex_Color.xyz, ex_Color.w);
}
