#version 330

in vec4 in_Position;
in vec4 inColor;
in vec2 inPsize;

out vec4 gColor;
out vec2 gPsize;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;

void main(void){
	gColor = inColor;
	gPsize = inPsize;
	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}
