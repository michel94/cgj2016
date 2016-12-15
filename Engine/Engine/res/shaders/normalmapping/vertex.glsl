#version 330

in vec4 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;
in vec3 inBitangent;

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
	vec3 lightPos = vec3(0.0f, -2.0f, 2.0f);

	gl_Position = ProjMatrix * ViewMatrix * Matrix * inPosition;
	vs_out.FragPos = vec3(ViewMatrix * Matrix * inPosition);
    vs_out.TexCoords = inTexcoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(Matrix)));
    vec3 T = normalize(mat3(Matrix) * inTangent);
    vec3 B = normalize(mat3(Matrix) * inBitangent);
    vec3 N = normalize(mat3(Matrix) * inNormal);
    
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * CameraPosition;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

}
