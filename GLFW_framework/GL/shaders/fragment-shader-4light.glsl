#version 150

uniform mat4 model;
uniform vec3 cameraPosition;

// material settings
uniform sampler2D materialTex;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform struct Light {
   vec3 position[4];
   vec3 intensities[4]; //a.k.a the color of the light
   float attenuation[4];
   float ambientCoefficient[4];
} light;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

void main() {
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = texture(materialTex, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

	vec3 linearColor = vec3(0,0,0);
	for ( int i = 0; i < 4; i++ )
	{
	    vec3 surfaceToLight = normalize(light.position[i] - surfacePos);
    
		//ambient
		vec3 ambient = light.ambientCoefficient[i] * surfaceColor.rgb * light.intensities[i];

		//diffuse
		float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
		vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities[i];
    
		//specular
		float specularCoefficient = 0.0;
		if(diffuseCoefficient > 0.0)
			specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
		vec3 specular = specularCoefficient * materialSpecularColor * light.intensities[i];
    
		//attenuation
		float distanceToLight = length(light.position[i] - surfacePos);
		float attenuation = 1.0 / (1.0 + light.attenuation[i] * pow(distanceToLight, 2));

		//linear color (color before gamma correction)
		linearColor += ambient + attenuation*(diffuse + specular);
    }
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}