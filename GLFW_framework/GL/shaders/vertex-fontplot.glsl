#version 150

uniform float xPos;
uniform float yPos;
uniform float xSize;
uniform float ySize;

uniform float uPos;
uniform float vPos;
uniform float uSize;
uniform float vSize;

in vec2 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main() {
	// apply transform to vertex and uv
	fragTexCoord = vec2( vertTexCoord.x * uSize + uPos, vertTexCoord.y * vSize + vPos);
	gl_Position = vec4( vert.x * xSize + xPos, vert.y * ySize + yPos, 0.0f, 1.0f);
}