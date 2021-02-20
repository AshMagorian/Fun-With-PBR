#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	//vec3 tex = TexCoords;
	//
	//if (tex.x > 0.99999) {tex.z *= -1.0;}
	//if (tex.y > 0.99999) {tex.z *= -1.0;}
	//if (tex.z > 0.99999) {tex.x *= -1.0;}
	//if (tex.x < -0.99999) {tex.z *= -1.0;}
	//if (tex.y < -0.99999) {tex.z *= -1.0;}
	//if (tex.z < -0.99999) {tex.x *= -1.0;}
	//
	//FragColor = texture(skybox, tex);
	
	
	
	vec3 envColor = texture(skybox, TexCoords).rgb;
	envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
	FragColor = vec4(envColor, 1.0);
}