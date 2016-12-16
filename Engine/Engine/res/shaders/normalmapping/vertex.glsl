#version 430

const int MAX_LIGHTS=8;

in vec4 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;
in vec3 inTangent;
in vec3 inBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos[MAX_LIGHTS];
    vec3 LightColor[MAX_LIGHTS];
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    flat int nLights;
} vs_out;

uniform CameraBlock{
	mat4 ProjMatrix;
	mat4 ViewMatrix;
	vec3 CameraPosition;
};

uniform LightBlock{
	vec3 LightPosition[MAX_LIGHTS];
	vec3 LightColor[MAX_LIGHTS];
	int nLights;
};

uniform mat4 Matrix;

void main(void){
	vs_out.nLights = nLights;
	gl_Position = ProjMatrix * ViewMatrix * Matrix * inPosition;
	vs_out.FragPos = vec3(Matrix * inPosition);
    vs_out.TexCoords = inTexcoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(Matrix)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 B = normalize(normalMatrix * inBitangent);
    vec3 N = normalize(normalMatrix * inNormal);
    
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentViewPos  = TBN * CameraPosition;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    for(int i=0; i<nLights; i++){
        vs_out.TangentLightPos[i] = TBN * LightPosition[i];
        vs_out.LightColor[i] = LightColor[i];
    }

}
