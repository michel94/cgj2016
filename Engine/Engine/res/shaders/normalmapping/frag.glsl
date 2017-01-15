#version 420

const int MAX_LIGHTS=8;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos[MAX_LIGHTS];
    vec3 LightColor[MAX_LIGHTS];
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 worldNormal;
    flat int nLights;
} fs_in;

in vec3 FragPos;
in vec3 ex_CameraPosition;
in vec3 CorrectNormal;
in vec4 LightPosition_out[MAX_LIGHTS];

uniform vec4 Color;
uniform sampler2D tex;
uniform sampler2D normalTex;
uniform samplerCube cube_texture;

out vec4 out_Color;

void main(void){
	int nLights = fs_in.nLights;
	
	vec2 TexCoords = fs_in.TexCoords * 4;
	vec3 normal = texture(normalTex, TexCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    // Get diffuse color
    vec3 color = texture(tex, TexCoords).rgb;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	vec3 lightDir;
	vec3 lightColor;
    float att;
    for(int i=0; i<nLights; i++){ 
		//directional light
		if(LightPosition_out[i].w == 0.0f){ 
			lightDir  = normalize(fs_in.TangentLightPos[i]);
			att = 1;
		}

		//point light
		else {
			lightDir  = normalize(fs_in.TangentLightPos[i] - fs_in.TangentFragPos);
			lightColor = fs_in.LightColor[i];
			vec3 L = normalize(LightPosition_out[i].xyz - FragPos);
			float dist = length(L);
			att=1.0/(1.0+0.1*dist+0.01*dist*dist);
			lightColor *= att;
		}
		//vec3 lightDir = normalize(LightPosition_out[i].xyz - FragPos);
		// Ambient
		ambient = vec3(0.3);
		//difuse
		float diff = max(dot(lightDir, normal), 0.0) * 2;
		diffuse += diff * lightColor;
		
		// Specular
	    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	    vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);
	    float spec = pow(max(dot(normal, halfwayDir), 0.0), 4.0);
		specular += vec3(0.6) * spec * fs_in.LightColor[i];

		//out_Color = texture(cube_texture, reflectDir);
    }
    vec3 I = normalize(FragPos - ex_CameraPosition);
    vec3 R = reflect(I, normalize(CorrectNormal));
	//R.x*=-1;
	//R.y*=-1;
	//R.z*=-1;
    out_Color = texture(cube_texture, R);
    //out_Color = vec4( (ambient*0 + diffuse) * color + specular, 1.0f);
	
}

