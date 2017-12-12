
uniform sampler2D weight;
uniform sampler2D fungus;
uniform sampler2D grass;
uniform sampler2D dirt;

void main(void)
{
	vec4 weightSample  = texture2D(weight, gl_TexCoord[0].xy);
	vec4 fungusSample  = texture2D(fungus, gl_TexCoord[0].xy * 25.0);
	vec4 grassSample   = texture2D(grass , gl_TexCoord[0].xy * 20.0);
	vec4 dirtSample    = texture2D(dirt  , gl_TexCoord[0].xy * 20.0);

	float weightScale1 = clamp(weightSample.y * 1.25, 0.0, 1.0);
	float weightScale2 = clamp(weightSample.x, 0.0, 1.0);

	vec3  normal       = normalize(gl_TexCoord[1].xyz);
	float dotProduct   = clamp(dot(vec3(0.73, 0.73, 0.0), normal), 0.0, 1.0);

	vec4  dirtAndGrass = dirtSample   * weightScale1 + (1.0 - weightScale1) * grassSample;
	vec4  color        = weightScale2 * dirtAndGrass + (1.0 - weightScale2) * fungusSample;
	gl_FragColor       = color * dotProduct;
}
