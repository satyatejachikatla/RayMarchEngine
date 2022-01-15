#include <rayMarchUtils/core.hglsl>
#include <rayMarchUtils/utils.hglsl>

#include <rayMarchUtils/math/math.hglsl>

#include <rayMarchUtils/lighting/pointLight.hglsl>
#include <rayMarchUtils/lighting/directionLight.hglsl>

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
	DirectionLight directionLight = getDefaultDirectionLight();

	// Control speed of time
	time = time * .5;

	// Camera with mouse
	cam.rayOrigin = vec3(-1.,1.,-1.);

	float pitch = mouse.y * 360.;
	pitch = clamp(pitch,-89,89);
	pitch = convertDegreeToRadian(pitch);

	float yaw = mouse.x * 360.;
	yaw = convertDegreeToRadian(yaw);

	vec3 direction;
	direction.x = cos(yaw) * cos(pitch);
	direction.y = sin(pitch);
	direction.z = sin(yaw) * cos(pitch);
	direction = normalize(direction);

	cam.lookAt = cam.rayOrigin  + direction;

	// Point Light Source Movement
	pointLight.lightSource = vec3(3.*sin(time),3.,2.*cos(time));

	// Direction Light Source Movement
	directionLight.lightDirection = normalize(vec3(3.*sin(time),3.,2.*cos(time)));

	vec3 rayDirection = getCamera3DRay(cam);

	// RayMarch Hitpoint
	RayMarchReturn rayMarchReturn = RayMarch(cam.rayOrigin,rayDirection,rayMarchSettings);

	//Lighting
	float l = 0;
	if (rayMarchReturn.isHit) {
		l = GetPointLight(rayMarchReturn.hitAt,rayMarchSettings,pointLight);
		// l = GetDirectionLight(rayMarchReturn.hitAt,rayMarchSettings,directionLight);
	}

	vec3 col = vec3(l);
	
	fragColor = vec4(col,1.0);
}
