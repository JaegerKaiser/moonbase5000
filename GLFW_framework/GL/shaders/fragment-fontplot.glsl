#version 150

uniform sampler2D fontTex;
uniform vec4 colour;

in vec2 fragTexCoord;

out vec4 finalColor;

void main() {
    finalColor = colour * texture(fontTex, fragTexCoord);
}