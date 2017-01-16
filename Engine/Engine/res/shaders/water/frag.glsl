#version 420

float max3(float a, float b, float c){
	return max(a, max(b, c));
}

float maxv3(vec3 v){
	return max(v.x, max(v.y, v.z));
}

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

uniform float reflectionBlend;

void main(void){
	int nLights = fs_in.nLights;
	
	vec2 TexCoords = fs_in.TexCoords * 4;
	//vec3 normal = texture(normalTex, TexCoords).rgb;
	vec3 normal = CorrectNormal;
    // Transform normal vector to range [-1,1]
    //normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    // Get texture color
    vec3 color = texture(tex, TexCoords).rgb;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float att;
    for(int i=0; i<nLights; i++){ 
    	vec3 lightDir;
		vec3 lightColor;

		lightColor = fs_in.LightColor[i];
		//directional light
		if(LightPosition_out[i].w == 0.0f){ 
			lightDir  = normalize(-fs_in.TangentLightPos[i]);
			att = 1;
		}else { //point light
			lightDir  = normalize(fs_in.TangentLightPos[i] - fs_in.TangentFragPos);
			vec3 L = normalize(LightPosition_out[i].xyz - FragPos);
			float dist = length(L);
			att=1.0/(1.0+0.1*dist+0.01*dist*dist);
			lightColor *= att;
		}
		// Ambient
		ambient = vec3(0.1);
		
		// Diffuse
		float diff = max(dot(lightDir, normal), 0.0);
		diffuse += diff * lightColor * 0.8;
		
		// Specular
	    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	    vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);
	    float spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);

	    vec3 I = normalize(FragPos - ex_CameraPosition);
    	vec3 R = reflect(I, normalize(CorrectNormal));
    	vec4 skyColor = texture(cube_texture, R);
	    //vec4 skyColor = texture(cube_texture, R);
	    //vec4 lightColor = vec4(fs_in.LightColor[i], 1)
	    vec3 specColor = mix(skyColor.xyz, lightColor, reflectionBlend);
		specular += spec * specColor;
		//out_Color = texture(cube_texture, reflectDir);
    }
	vec3 I = normalize(FragPos - ex_CameraPosition);
    vec3 R = reflect(I, normalize(CorrectNormal));

    vec4 skyColor = texture(cube_texture, R);
    //out_Color = mix(skyColor, vec4(color, 1), reflectionBlend) + spec;
    out_Color = vec4(  + specular, 0.7);
    
}

