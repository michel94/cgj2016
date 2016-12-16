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

uniform Light{
	vec3 LightPosition[3];
	vec3 LightColor[3];
};

uniform mat4 Matrix;

void main(void){
	vec3 lightPos = vec3(0.5f, 0.0f, 2.0f);

	gl_Position = ProjMatrix * ViewMatrix * Matrix * inPosition;
	vs_out.FragPos = vec3(Matrix * inPosition);
    vs_out.TexCoords = inTexcoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(Matrix)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 B = normalize(normalMatrix * inBitangent);
    vec3 N = normalize(normalMatrix * inNormal);
    
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * CameraPosition;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

}
