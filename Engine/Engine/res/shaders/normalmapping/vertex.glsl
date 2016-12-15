
#version 330

in vec4 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;
in vec3 inBitangent;

/*out vec2 ex_Texcoord;
out vec3 eyeDir;
out vec3 lightDir;
out vec3 position;*/

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform mat4 Matrix;

void main(void){
	/*vec3 normalViewSpace = inNormal;
	vec3 tangentViewSpace = inTangent;
	vec3 bitangentViewSpace = inBitangent;
	mat4 TBN = mat4(transpose(mat3(
		tangentViewSpace,
		bitangentViewSpace,
		normalViewSpace
	)));

	ex_Normal = inNormal;
	

	ex_Texcoord = inTexcoord;
	mat4 ModelView = Matrix * ViewMatrix;

	mat4 normalMatrix = transpose(inverse(ModelView));
	ex_Normal = (normalMatrix * vec4(inNormal, 0.0f)).xyz;

	vec3 vertexInCamSpace = (TBN * ModelView * in_Position).xyz;
	eyeDir = -vertexInCamSpace.xyz;
	eyeDir = normalize(eyeDir);

	vec3 lightInCamSpace = (TBN * ModelView * vec4(lightPos, 1.0f)).xyz;
	lightDir = vec3(lightInCamSpace - vertexInCamSpace).xyz;
	lightDir = normalize(lightDir);
	
	gl_Position = position = ProjMatrix * ViewMatrix * Matrix * in_Position;*/
	vec3 lightPos = vec3(0.0f, -2.0f, 2.0f);

	gl_Position = ProjMatrix * ViewMatrix * Matrix * inPosition;
	vs_out.FragPos = vec3(ViewMatrix * Matrix * inPosition);
    vs_out.TexCoords = inTexcoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(Matrix)));
    vec3 T = normalize(mat3(Matrix) * inTangent);
    vec3 B = normalize(mat3(Matrix) * inBitangent);
    vec3 N = normalize(mat3(Matrix) * inNormal);
    
    //vec3 viewPos = vec3(0, 0, 0);
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos; // multiply by TBN
    vs_out.TangentViewPos  = TBN * CameraPosition; // multiply by TBN
    vs_out.TangentFragPos  = TBN * vs_out.FragPos; // multiply by TBN

}

