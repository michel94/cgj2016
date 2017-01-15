#version 420

in vec3 ex_Texcoord;
in vec3 ex_CameraPosition;
in vec3 Normal;
in vec3 ex_inPosition;
in vec3 pos_eye;
in vec3 n_eye;
in mat4 ex_ViewMatrix;

uniform samplerCube cube_texture;
uniform samplerCube cube_texture2;
uniform float blendfactor;
out vec4 out_Color;

void main() {
	vec4 texture1 = texture(cube_texture, ex_Texcoord);
	vec4 texture2 = texture(cube_texture2, ex_Texcoord);
	out_Color = mix(texture1, texture2, blendfactor);
}