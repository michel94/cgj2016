#version 420

in vec4 in_Position;
in vec2 inTexcoord;
in vec3 inNormal;

out vec2 ex_Texcoord;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;
uniform vec4 Color;

void main(void){
	ex_Texcoord = inTexcoord;

	mat4 MatrixCopy = Matrix;

	float x = MatrixCopy.x;
	float y = MatrixCopy.y;
	float z = MatrixCopy.z;

//	float d = sqrt( x * x * x + y * x * x + z * x * x);

//	MatrixCopy[0][0] = d;
	//MatrixCopy[0][1] = 0;
//	MatrixCopy[0][2] = 0;
	
	//MatrixCopy[1][0] = 0;
	//MatrixCopy[1][1] = d;
	//MatrixCopy[1][2] = 0;

	//MatrixCopy[2][0] = 0;
	//MatrixCopy[2][1] = 0;
	//MatrixCopy[2][2] = d;

	gl_Position = ProjMatrix * ViewMatrix * MatrixCopy * in_Position;
}
