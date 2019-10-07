#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;
out vec2 fragTexCoord;

void main()
{
	gl_Position = u_Projection * u_Model * vec4(position.x, position.y, 0.0f, 1.0f);
	fragTexCoord = texCoord;
};