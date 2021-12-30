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

#define PI 3.1415926535897932384626433832795

layout(location = 0) out vec4 fragColor;

in vec2 v_position;

uniform float u_Time;
uniform float u_Itteration;
uniform vec2 u_Resolution;
uniform vec2 u_Mouse;
uniform sampler2D u_TextureChannels[5];



void main()
{
	vec2 u_Resolution = vec2(100,100);

	vec2 fragCoords = v_position.xy;
	vec2 uv = (fragCoords-.5*u_Resolution.xy)/u_Resolution.y;
	// vec2 mouse = vec2(u_Mouse.x-0.5*u_Resolution.x, u_Mouse.y)/u_Resolution.y;

	uv = abs(uv);
	vec3 col = vec3(uv,0.);

	fragColor = vec4(col,1.0);

}
