
varying vec2 vTexCoord;

uniform float strength;
uniform sampler2D colorMap;
uniform sampler2D normalMap; 

void main(void)
{
	vec2 offset = texture2D( Texture2, vTexCoord).xy;

	float d = length(offset);
	if ( d != 0.0) {
		offset -= 0.5 * d;
	}

	offset = offset * strength + vTexCoord;

	gl_FragColor = texture2D( colorMap, offset );
	//gl_FragColor = texture2D( colorMap, vTexCoord ) + texture2D( normalMap, vTexCoord );
};
