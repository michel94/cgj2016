#version 430

const int MAX_LIGHTS=8;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos[MAX_LIGHTS];
    vec3 LightColor[MAX_LIGHTS];
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    flat int nLights;
} fs_in;

uniform vec4 Color;
uniform sampler2D tex;
uniform sampler2D normalTex;

out vec4 out_Color;

void main(void){
	int nLights = fs_in.nLights;
	
	vec2 TexCoords = fs_in.TexCoords * 4;
	vec3 normal = texture(normalTex, TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    // Get diffuse color
    vec3 color = texture(tex, TexCoords).rgb;
    // Ambient
    vec3 ambient = vec3(0.3);
    vec3 diffuse;
    vec3 specular;
    // Diffuse
    for(int i=0; i<nLights; i++){
	    vec3 lightDir = normalize(fs_in.TangentLightPos[i] - fs_in.TangentFragPos);
	    float diff = max(dot(lightDir, normal), 0.0);
	    diffuse += diff * fs_in.LightColor[i];
	    // Specular
	    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	    vec3 reflectDir = reflect(-lightDir, normal);
	    vec3 halfwayDir = normalize(lightDir + viewDir);
	    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
		specular += vec3(0.6) * spec; // multiply by * fs_in.LightColor[i]
    }
    
    out_Color = vec4( (ambient + diffuse) * color + specular, 1.0f);
	
}

