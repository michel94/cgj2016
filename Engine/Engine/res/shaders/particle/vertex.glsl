#version 330

in vec4 in_Position;
in vec4 inColor;

out vec4 gColor;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;

void main(void){
	gColor = inColor;
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}
