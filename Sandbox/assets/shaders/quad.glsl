// Simple Textured Quad Shader

#type vertex
#version 430

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_InverseVP;

out vec3 v_Position;

void main()
{
	vec4 position = vec4(a_Position.xy, 1, 1.0);//z轴不能为0或者1，要不然深度测试过不去
	gl_Position =position;
	
	v_Position = (inverse(u_InverseVP)* position).xyz;
}

#type fragment
#version 430

layout(location = 0) out vec4 finalColor;

uniform samplerCube u_Texture;

in vec3 v_Position;

void main()
{
	finalColor = textureLod(u_Texture, v_Position, 0.0);
	//finalColor = vec4(v_Position,0,1);
}