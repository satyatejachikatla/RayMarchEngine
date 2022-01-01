#include <rayMarchUtils/core.hglsl>
#include <rayMarchUtils/utils.hglsl>
#include <rayMarchUtils/math/math.hglsl>
#include <rayMarchUtils/math/spaceTransforms.hglsl>

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
