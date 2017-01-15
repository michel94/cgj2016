#version 420

in vec4 in_Position;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 ex_Texcoord;
out vec3 ex_CameraPosition;
out vec3 Normal;
out vec3 ex_inPosition;
out vec3 pos_eye;
out vec3 n_eye;
out mat4 ex_ViewMatrix;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;
uniform vec4 Color;


void main() {
	ex_Texcoord = vec3(in_Position);
	ex_ViewMatrix = ViewMatrix;
	pos_eye = vec3(ViewMatrix * Matrix * in_Position);
	n_eye = vec3(ViewMatrix * Matrix * vec4(inNormal, 0.0));
	Normal = mat3(transpose(inverse(Matrix))) * inNormal;//inNormal;
    ex_inPosition = vec3(in_Position);//vec3(Matrix * in_Position);//
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}