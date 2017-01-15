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


mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}


uniform mat4 Matrix;
uniform vec4 Color;

void main(void){
	float threshold = 0.9999;

	ex_Texcoord = inTexcoord;
	mat4 matCopy = Matrix;

	vec4 worldSpace = Matrix * in_Position;

	float posX = worldSpace[0];
	float posY = worldSpace[1];
	float posZ = worldSpace[2];

	vec3 quadPosition = vec3(posX, posY, posZ);

	vec3 objToCamProj = normalize(vec3(CameraPosition.x - posX, 0, CameraPosition.z - posZ));

	vec3 lookAt = vec3(0,0,1);

	vec3 upAux = vec3(0, 1, 0);

	upAux = cross(lookAt, objToCamProj);

	float angleCosine = dot(lookAt, objToCamProj);
	
	if(angleCosine < threshold && angleCosine > -threshold){
		matCopy *= rotationMatrix(upAux, acos(angleCosine) * 180 / 3.14);
	}

	gl_Position = ProjMatrix * ViewMatrix * matCopy * in_Position;
}
