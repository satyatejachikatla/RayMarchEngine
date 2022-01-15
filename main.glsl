#include <rayMarchUtils/core.hglsl>
#include <rayMarchUtils/utils.hglsl>
#include <rayMarchUtils/lighting/pointLight.hglsl>

#include <tests/rayMarchTests.hglsl>

float GetDist(vec3 p){
	return Test_2(p);
}

void main()
{
	vec2 uv = getPixelNormalized();
	vec2 mouse = getMouseNormalized();
	float time = getTime();

	Camera3D cam = getDefaultCamera3D();
	RayMarchSettings rayMarchSettings = getDefaultRayMarchSettings();
	PointLight pointLight = getDefaultPointLight(rayMarchSettings);

	// Camera with mouse
	cam.lookAt = vec3(-mouse.x,mouse.y,0.0);
	cam.rayOrigin = vec3(-1.,1.,-1.);

	// Light Source Movement
	time = time * 2.;
	pointLight.lightSource = vec3(3.*sin(time),3.,2.*cos(time));

	vec3 rayDirection = getCamera3DRay(cam);

	// RayMarch Hitpoint
	float rayHitDistance = RayMarch(cam.rayOrigin,rayDirection,rayMarchSettings);
	vec3 rayHitPosition = cam.rayOrigin + rayDirection * rayHitDistance;

	//Lighting
	float l = GetPointLight(rayHitPosition,rayMarchSettings,pointLight);

	vec3 col = vec3(l);
	
	fragColor = vec4(col,1.0);
}
