#version 330 core
layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 texCoord;

out vec2 textureCoords;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection *u_Model*pos;
	textureCoords = texCoord;
}