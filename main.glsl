#include <rayMarchUtils/Core.hglsl>

void main()
{

	vec2 fragCoords = v_position.xy;
	vec2 uv = (fragCoords-.5*u_Resolution.xy)/u_Resolution.y;
	vec2 mouse = vec2(u_Mouse.x-0.5*u_Resolution.x, u_Mouse.y)/u_Resolution.y;

	// uv = abs(uv);
	vec3 col = vec3(uv,0.);

	fragColor = vec4(col,1.0);

}
