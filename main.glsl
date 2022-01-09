#include <rayMarchUtils/core.hglsl>
#include <rayMarchUtils/utils.hglsl>
#include <rayMarchUtils/math/math.hglsl>
#include <rayMarchUtils/math/spaceTransforms.hglsl>

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
