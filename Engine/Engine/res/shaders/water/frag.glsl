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
uniform samplerCube cube_texture2;
uniform float rotation;
uniform float blendfactor;

out vec4 out_Color;

uniform float reflectionBlend;

void main(void){
	int nLights = fs_in.nLights;
	
	vec2 TexCoords = fs_in.TexCoords * 4;
	vec3 normal = CorrectNormal;
    // Get texture color
    //vec3 color = texture(tex, TexCoords).rgb;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 I = normalize(FragPos - ex_CameraPosition);
	vec3 R = reflect(I, normalize(CorrectNormal));
	//rotate R 
	vec2 t = vec2(R.x, R.z);
	float angle = atan(R.z, R.x);
	angle -= rotation;
	R.x = cos(angle)*length(t);
	R.z = sin(angle)*length(t);
	vec4 texture1 = texture(cube_texture, R);
	vec4 texture2 = texture(cube_texture2, R);
	vec4 skyColor =  mix(texture1, texture2, blendfactor);
	vec3 color = skyColor.rgb;
	
    float att;
    for(int i=0; i<nLights; i++){ 
    	vec3 lightDir;
		vec3 lightColor;

		lightColor = fs_in.LightColor[i];
		vec3 origColor = lightColor;
		//directional light
		if(LightPosition_out[i].w == 0.0f){ 
			lightDir  = normalize(-fs_in.TangentLightPos[i]);
			att = 1;
		}else { //point light
			lightDir  = normalize(fs_in.TangentLightPos[i] - fs_in.TangentFragPos);
			vec3 L = normalize(LightPosition_out[i].xyz - FragPos);
			float dist = length(L);
			att=0.1/(1.0+0.1*dist+0.01*dist*dist);
			lightColor *= att;
		}
		// Ambient
		ambient = vec3(0.1);
		
		// Diffuse
		float diff = max(dot(lightDir, normal), 0.0);
		diffuse += diff * lightColor * 1.8 * att;
		
		// Specular
	    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	    vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);
	    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);

	    //vec4 skyColor = texture(cube_texture, R);
	    //vec4 lightColor = vec4(fs_in.LightColor[i], 1)
	    vec3 specColor = origColor.rgb;
		specular += spec * specColor;
		
    }
	out_Color = vec4( (ambient + diffuse) * color + specular, 0.8);
    //out_Color = vec4(specular, 0.7);
    
}
