#version 330

in vec4 ex_Color;
out vec4 out_Color;

in vec3 ex_Normal;
in vec2 ex_Texcoord;

uniform vec4 Color;
uniform sampler2D tex;
uniform sampler2D normalTex;

void main(void){
	out_Color = mix(texture(tex, ex_Texcoord), texture(normalTex, ex_Texcoord), 0.5);
}
