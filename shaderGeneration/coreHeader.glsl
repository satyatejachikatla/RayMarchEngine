#shader vertex
#version 410 core

layout(location = 0) in vec2 position;

out vec2 v_position;

uniform mat4 u_P;

void main()
{

	gl_Position = u_P*vec4(position,0.0,1.0);
	v_position = position.xy;

}

#shader fragment
#version 410 core

layout(location = 0) out vec4 fragColor;

in vec2 v_position;

uniform float u_Time;
uniform float u_Itteration;
uniform vec2 u_Resolution;
uniform vec2 u_Mouse;
uniform sampler2D u_TextureChannels[5];
