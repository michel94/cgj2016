
#version 330

in vec4 in_Position;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;
in vec4 in_Color;

out vec2 ex_Texcoord;
out vec4 ex_Color;
out mat3 TBN;

out vec3 ex_Normal;
out vec3 eyeDir;
out vec3 lightDir;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
};

uniform mat4 Matrix;
uniform vec4 Color;

void main(void){
	mat4 MV = ViewMatrix * Matrix;
	vec3 normalViewSpace = inNormal;
	vec3 tangentViewSpace = inTangent;
	vec3 bitangentViewSpace = cross(inNormal, inTangent);
	TBN = transpose(mat3(
		tangentViewSpace,
		bitangentViewSpace,
		normalViewSpace
	));
	TBN = inverse(TBN);

	ex_Normal = inNormal;
	ex_Texcoord = inTexcoord;

	vec3 lightPos = vec3(0.0f, 1.0f, 5.0f);

	ex_Color = in_Color;
	ex_Texcoord = inTexcoord;
	mat4 ModelView = Matrix*ViewMatrix;

	mat4 normalMatrix = transpose(inverse(ModelView));
	ex_Normal = (normalMatrix * vec4(inNormal, 0.0f)).xyz;

	vec3 vertexInCamSpace = (ModelView * in_Position).xyz;
	eyeDir = -vertexInCamSpace.xyz;

	vec3 lightInCamSpace = (ModelView * vec4(lightPos, 1.0f)).xyz;
	lightDir = vec3(lightInCamSpace - vertexInCamSpace).xyz;

	gl_Position = ProjMatrix * ViewMatrix * Matrix * in_Position;

}
