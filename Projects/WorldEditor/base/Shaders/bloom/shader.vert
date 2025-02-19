
varying vec2 vTexCoord;

void main(void)
{
	vec2 Position;
	Position.xy = sign(gl_Vertex.xy);

	gl_Position = vec4(Position.xy, 0.0, 1.0);

	vTexCoord = Position.xy *.5 + .5;
}
