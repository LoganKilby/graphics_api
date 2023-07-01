uniform vec2 u_resoltuion;

float circle(float radius, vec2 position) {
	float value = distance(position, vec2(0.5));
	return step(radius, value);
}