#version 330

in vec4 in_Position;

out vec3 ex_Texcoord;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;

void main() {
	ex_Texcoord = vec3(in_Position);
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}