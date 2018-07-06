#version 150

uniform mat4 camera;
uniform vec3 cameraPosition;

in vec3 vert;
out vec3 texCoord;

void main() {
	// texture coordinate is just the vertex position
	texCoord = normalize(vert.xyz);
	// add the camera position to the vertex position, so the box is centred on the camera
	vec3 shifted = vert + cameraPosition;
	// transform by the view/projections matrix
	gl_Position = camera * vec4(shifted,1);
}