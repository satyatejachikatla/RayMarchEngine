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





/*
    Returns mouse co-ords in range [-0.5,0.5] in y and appropriate linear interpolated range in x
*/
vec2 getMouseNormalized() {
    vec2 mouse = vec2(u_Mouse.x-0.5*u_Resolution.x, u_Mouse.y)/u_Resolution.y;
    return mouse;
}

/*
    Returns mouse co-ords in range [0,maxScreenDimension] in both x and y
*/
vec2 getMouse() {
    return u_Mouse;
}

/*
    Returns pixel co-ords in range [-0.5,0.5] in y and appropriate linear interpolated range in x
*/
vec2 getPixelNormalized() {
    vec2 uv = (v_position-.5*u_Resolution.xy)/u_Resolution.y;
    return uv;
}

/*
    Returns pixel co-ords in range [0,maxScreenDimension] in both x and y
*/
vec2 getPixel() {
    return v_position;
}


/*
    Returns time in seconds
*/
float getTime() {
    return u_Time;
}



#define PI 3.1415926535897932384626433832795


/*
    2D Transforms
*/

vec2 translate(vec2 uv ,vec2 move) {
	return uv - move;
}

vec2 scale(vec2 uv ,vec2 scaleFactor) {
	return uv*scaleFactor;
}

vec2 rotate(vec2 uv ,float angle) {

    float s = sin(angle);
    float c = cos(angle);

	float xnew = uv.x * c - uv.y * s;
    float ynew = uv.x * s + uv.y * c;

    return vec2(xnew,ynew);
}
void main()
{
	vec2 uv = getPixelNormalized();
	vec2 mouse = getMouseNormalized();
	float sec = getTime();

	uv = translate(uv,vec2(-.5,-.5));

	uv = rotate(uv,sec);
	// uv = translate(uv,-mouse);

	vec3 col = vec3(uv,0.);
	
	fragColor = vec4(col,1.0);
}
