#version 330

out vec4 out_Color;

in vec3 ex_Normal;
in vec2 ex_Texcoord;

uniform vec4 Color;
uniform sampler2D tex;
uniform sampler2D normalTex;

void main(void){
	out_Color = mix(texture(tex, ex_Texcoord), texture(normalTex, ex_Texcoord), 0.5);
	//vec3 TextureNormal_tangentspace = normalize(texture(normalTex, ex_Texcoord).rgb*2.0 - 1.0);
	//out_Color = texture(normalTex, ex_Texcoord);
	//out_Color = vec4(ex_Normal/2 + vec3(0.5, 0.5, 0.5), 1);
}
