#version 330

/*in vec2 ex_Texcoord;
in vec3 ex_Normal;
in vec3 eyeDir;
in vec3 lightDir;*/

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform vec4 Color;
uniform sampler2D tex;
uniform sampler2D normalTex;

out vec4 out_Color;

void main(void){
	/*vec3 n = normalize(texture(normalTex, ex_Texcoord).rgb * 2.0 - 1.0);
	out_Color = vec4(n, 1);

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 E = normalize(eyeDir);
	//vec3 N = normalize(ex_Normal);
	vec3 N = normalize(texture(normalTex, ex_Texcoord).rgb*2.0 - 1.0); // Normal tangentspace
	vec3 L = normalize(lightDir);
	//L = normalize(L);
	
	vec3 H = normalize(E + L);

	//Ambient
	float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;

	//diffuse
	float diff = max(dot(L, N), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.8f;
	float spec = pow(max(dot(H, N), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor; 

	//out_Color = ex_Color;
	out_Color = (vec4(ambient, 1.0f) + vec4(diffuse, 1.0f) + vec4(specular, 1.0f)) * texture(tex, ex_Texcoord);*/
	//out_Color = (vec4(diffuse, 1.0f)) * texture(tex, ex_Texcoord);
	//float s = max(dot(L, N), 0.0f);
	//out_Color = vec4(s, s, s, 1);
	//out_Color = vec4(N, 1);

	// Obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalTex, fs_in.TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // Get diffuse color
    vec3 color = texture(tex, fs_in.TexCoords).rgb;
    // Ambient
    vec3 ambient = 0.3 * color;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0) * 2;
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;
    
    out_Color = vec4(ambient + diffuse + specular, 1.0f);
	
}

