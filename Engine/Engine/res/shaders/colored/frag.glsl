#version 420

in vec4 ex_Color;
out vec4 out_Color;

in vec2 ex_Texcoord;
in vec3 ex_Normal;
in vec3 eyeDir;
in vec3 lightDir;

uniform vec4 Color;
uniform sampler2D tex;

void main(void){
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 E = normalize(eyeDir);
	vec3 N = normalize(ex_Normal);
	vec3 L = normalize(lightDir);

	vec3 H = normalize(E + L);

	//Ambient
	float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;

	//diffuse
	float diff = max(dot(L, N), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.8f;
	float spec = pow(max(dot(H, N), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

	out_Color = (vec4(ambient, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f)) * texture(tex, ex_Texcoord);
}
