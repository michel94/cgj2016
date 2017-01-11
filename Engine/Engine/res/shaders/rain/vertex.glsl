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
	float threshold = 0.9999;

	ex_Texcoord = inTexcoord;

	float posX = Matrix[0][3];
	float posY = Matrix[1][3];
	float posZ = Matrix[2][3];

	vec3 quadPosition = vec3(posX, posY, posZ);

	vec3 objToCamProj = normalize(vec3(CameraPosition.x - posX, 0, CameraPosition.z - posZ));

	vec3 lookAt = vec3(0,0,1);
	//objToCamProj = (objToCamProj);

	vec3 upAux = vec3(0, 1, 0);

	upAux = cross(lookAt, objToCamProj);

	float angleCosine = dot(lookAt, objToCamProj);
	
	if(angleCosine < threshold && angleCosine > -threshold){
			
	}

	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;
}

