#version 430 core

out vec4 FragColor;

struct Material
{
	sampler2D albedo;
	sampler2D normal;
	sampler2D metallic;
	sampler2D roughness;
	sampler2D ao;
};

struct TexelValue
{
	vec3 albedo;
    vec3 normal;
    float metallic;
    float roughness;
    float ao;
};

struct DirLight
{
	vec3 direction;
	vec3 color;
};

struct PointLight
{
	vec3 position;
	vec3 color;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

uniform vec3 in_ViewPos;
uniform DirLight in_DirLight;

uniform int in_NoPointLights;
uniform int in_NoSpotLights;

uniform PointLight in_PointLights[50];
uniform SpotLight in_SpotLights[50];

uniform Material in_Material;
const float PI = 3.14159265359;

vec3 CalcNormal();
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, TexelValue tex);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, TexelValue tex, vec3 F0);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, TexelValue tex);

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

void main()
{
	TexelValue tex;
	tex.albedo    = pow(texture(in_Material.albedo, fs_in.TexCoord).rgb, vec3(2.2));
    tex.normal    = CalcNormal();
    tex.metallic  = texture(in_Material.metallic, fs_in.TexCoord).r;
    tex.roughness = texture(in_Material.roughness, fs_in.TexCoord).r;
    tex.ao        = texture(in_Material.ao, fs_in.TexCoord).r;

	vec3 viewDir = normalize(in_ViewPos - fs_in.FragPos); // V
	vec3 F0 = vec3(0.04); // Surface reflection at zero incidence
	F0 = mix(F0, tex.albedo, tex.metallic);

	vec3 Lo = CalcDirLight(in_DirLight, tex.normal, viewDir, tex);
	for(int i = 0; i < in_NoPointLights; i++)
	{
		Lo += CalcPointLight(in_PointLights[i], tex.normal, fs_in.FragPos, viewDir, tex, F0); 
	}
	for (int i = 0; i < in_NoSpotLights; i++)
	{
		Lo += CalcSpotLight(in_SpotLights[i], tex.normal, fs_in.FragPos, viewDir, tex);
	}

	vec3 ambient = vec3(0.03) * tex.albedo * tex.ao;
	vec3 color = ambient + Lo;
	
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}

vec3 CalcNormal()
{
	return normalize(fs_in.Normal);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, TexelValue tex)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 rtn = vec3(0.0, 0.0, 0.0);
	return rtn; 
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, TexelValue tex, vec3 F0)
{
	// calculate radiance 
	vec3 L = normalize(light.position - fragPos); 		// Light direction
	vec3 H = normalize(viewDir + L);              		// Inbetween vector
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = light.color * attenuation;
	
	// cook-torrance BRDF
	float NDF = DistributionGGX(normal, H, tex.roughness);
	float G = GeometrySmith(normal, viewDir, L, tex.roughness);
	vec3 F = FresnelSchlick(max(dot(H, viewDir), 0.0), F0);
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - tex.metallic;
	
	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.0);
	
	//add outgoing radiance Lo
	float NdotL = max(dot(normal, L), 0.0);
	vec3 rtn = (kD * tex.albedo / PI + specular) * radiance * NdotL;
	return rtn;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, TexelValue tex)
{
	vec3 lightDir = normalize(light.position - fragPos);
	return vec3(0.0, 0.0, 0.0);
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}