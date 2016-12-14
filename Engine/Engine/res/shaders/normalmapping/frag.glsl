#version 330

in vec4 ex_Color;
out vec4 out_Color;

in vec2 ex_Texcoord;
in vec3 ex_Normal;
in vec3 eyeDir;
in vec3 lightDir;
in mat3 TBN;

uniform vec4 Color;
uniform sampler2D tex;
uniform sampler2D normalTex;

void main(void){
	vec3 n = TBN * normalize(texture(normalTex, ex_Texcoord).rgb * 2.0 - 1.0);
	out_Color = vec4(n, 1);

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 E = TBN * normalize(eyeDir);
	//vec3 N = TBN * normalize(ex_Normal);
	vec3 N = normalize(texture(normalTex, ex_Texcoord).rgb*2.0 - 1.0); // Normal tangentspace
	vec3 L = TBN * normalize(lightDir);
	
	vec3 H = normalize(E + L);

	//Ambient
	float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;

	//difuse
	float diff = max(dot(L, N), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.8f;
	float spec = pow(max(dot(H, N), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

	//out_Color = ex_Color;
	out_Color = (vec4(ambient, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f)) * texture(tex, ex_Texcoord);
	//out_Color = vec4(ex_Texcoord, 1, 1);
	//out_Color = vec4(0.5,0.5,0.5,1);
}
