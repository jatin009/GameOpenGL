#version 330 core

in vec2 textureCoords;
out vec4 color;

uniform vec4 u_Color;
uniform sampler2D ourTexture;

void main()
{
	color = u_Color * texture(ourTexture, textureCoords);
}