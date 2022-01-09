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

/* 3d Camera */

struct Camera3D{
    float zoom;
    vec3 lookAt;
    vec3 rayOrigin;
    vec3 upWorld;
};

/*
	Returns rayDirection wrt to the screen
	Consider the screen as the viewing window
*/
vec3 getCamera3DRay(Camera3D cam) {

	float zoom = cam.zoom;
	vec3 ro = cam.rayOrigin;
	vec3 lookat = cam.lookAt;
	vec3 u_world = cam.upWorld;
    vec2 screen = getPixelNormalized();

	vec3 f = normalize(lookat-ro);
	vec3 r = cross(u_world,f);
	vec3 u = cross(f,r);

	vec3 c = ro + f*zoom;
	vec3 i = c + screen.x*r + screen.y*u;

	vec3 rd = normalize(i-ro);

    return rd;
}

/* 3d Camera End */


/* Ray March Core */

#define MAX_DIST        100.
#define MAX_STEPS       100
#define SURF_DIST       .01


float GetDist(vec3 p){
	vec4 s = vec4(0.,1.,6.,1.);
	vec4 s2 = vec4(2.1,1.,6.,1.);

	float sphereDist = length(p-s.xyz) - s.w;
	float sphereDist2 = length(p-s2.xyz) - s2.w;
	float planeDist = p.y;

	float d = min(min(sphereDist,planeDist),sphereDist2);

	return d;

}

vec3 GetNormal(vec3 p){
	float d = GetDist(p);
	vec2 e = vec2(0.01,0.);

	vec3 n = d- vec3(
			GetDist(p-e.xyy),
			GetDist(p-e.yxy),
			GetDist(p-e.yyx));

	return normalize(n);
}


float RayMarch(vec3 ro,vec3 rd){
	float dO = 0;

	for(int i=0;i<MAX_STEPS;i++){
		vec3 p = ro+rd*dO;
		float dS = GetDist(p);
		dO += dS;
		if(dO > MAX_DIST || dS < SURF_DIST) break;
	}

	return dO;
} 

float GetLight(vec3 p){
	vec3 lightPosition = vec3(0.,5.,6.);
	float t = u_Time*2.;
	lightPosition.xz = vec2(3.*sin(t),2.*cos(t));
	vec3 l = normalize(lightPosition-p);
	vec3 n = GetNormal(p);

	float diff = clamp(dot(n,l),0.,1.);

	float d = RayMarch(p+n*SURF_DIST*1.2,l);
	if (d<length(lightPosition-p)) diff *= .1;

	return diff;
}



/* Ray March Core End */




#define PI 3.1415926535897932384626433832795

/*
    Returns Polar (theta,radius) of Cartesian point (uv.x,uv.y)
*/
vec2 convertPixelToPolar(vec2 uv) {
    uv = vec2(atan(uv.x,uv.y),length(uv));
    return uv;
}


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

	Camera3D cam;
   	cam.zoom = 1.; // No zoom
    cam.lookAt = vec3(-mouse,1.);
    cam.rayOrigin = vec3(-0.,1.,-0);
    cam.upWorld = vec3(0.,1.,0.); // Up of the worl will always be y

	vec3 rayDirection = getCamera3DRay(cam); // returns the ray direction for the current pixel


	float rayHitDistance = RayMarch(cam.rayOrigin,rayDirection);
	vec3 rayHitPosition = cam.rayOrigin + rayDirection * rayHitDistance;
	float l = GetLight(rayHitPosition);
	vec3 col = vec3(l);
	
	fragColor = vec4(col,1.0);
}
