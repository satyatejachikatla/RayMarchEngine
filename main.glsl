#include <rayMarchUtils/core.hglsl>
#include <rayMarchUtils/utils.hglsl>
#include <rayMarchUtils/shapes/shapes3D.hglsl>

float GetDist(vec3 p){

	float sphereDist1 = SDSphere(p,vec3(0.,1.,6.),1.);
	float sphereDist2 = SDSphere(p,vec3(2.1,1.,6.),1.);
	float planeDist = SDXZPlaneAtYZero(p);
	float cd = SDCapsule(p,vec3(0.,1.,6.),vec3(1.,2.,2.),.2);
	float td = SDTorus(p-vec3(.0,.5,6.),vec2(1.5,.25));
	float bd = SDBox(p-vec3(-2.5,.5,6.),vec3(.75));
	float cyld = SDCylinder(p-vec3(.5,.5,.5),vec3(0.,.3,.0),vec3(1.,2.,2.),.1);

	float d = sphereDist1;
	d = min(d,sphereDist2);
	d = min(d,planeDist);
	d = min(d,cd);
	d = min(d,td);
	d = min(d,bd);
	d = min(d,cyld);
	
	return d;

}

void main()
{
	vec2 uv = getPixelNormalized();
	vec2 mouse = getMouseNormalized();
	float time = getTime();

	Camera3D cam = getDefaultCamera3D();
	RayMarchSettings rayMarchSettings = getDefaultRayMarchSettings();

	// Camera with mouse
	cam.lookAt = vec3(-mouse.x,mouse.y,0.0);
	cam.rayOrigin = vec3(-1.,1.,-1.);

	// Light Source Movement
	time = time * 2.;
	rayMarchSettings.lightSource = vec3(3.*sin(time),5.,2.*cos(time));

	vec3 rayDirection = getCamera3DRay(cam);

	float rayHitDistance = RayMarch(cam.rayOrigin,rayDirection,rayMarchSettings);
	vec3 rayHitPosition = cam.rayOrigin + rayDirection * rayHitDistance;
	float l = GetLight(rayHitPosition,rayMarchSettings);
	vec3 col = vec3(l);
	
	fragColor = vec4(col,1.0);
}
