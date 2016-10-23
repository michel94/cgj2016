#version 450

in vec4 ex_Color;
out vec4 out_Color;
uniform vec4 Color;

void main(void){
	out_Color = Color;
}
