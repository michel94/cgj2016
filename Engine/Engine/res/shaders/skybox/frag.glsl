#version 330

in vec3 ex_Texcoord;

uniform samplerCube cube_texture;
uniform samplerCube cube_texture2;
uniform float blendfactor;
out vec4 out_Color;

void main() {
	vec4 texture1 = texture(cube_texture, ex_Texcoord);
	vec4 texture2 = texture(cube_texture2, ex_Texcoord);
	out_Color = mix(texture1, texture2, blendfactor);
}