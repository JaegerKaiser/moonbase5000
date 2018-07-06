#version 150

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;
in vec3 vertTangent;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragTangent;

void main() {
    // Pass some variables to the fragment shader
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    fragVert = vert;
	fragTangent = vertTangent;
	
    // Apply all matrix transformations to vert
    gl_Position = camera * model * vec4(vert, 1);
}