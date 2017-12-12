
uniform sampler2D heightMap;


#define textureSize 256.0
#define texelSize 1.0 / 256.0

vec4 tex2D_bilinear( sampler2D tex, vec2 t )
{
	vec2 f = fract( t.xy * textureSize );
	vec4 t00 = texture2D( tex, t );
	vec4 t10 = texture2D( tex, t + vec2( texelSize, 0.0 ));
	vec4 tA = mix( t00, t10, f.x );
	vec4 t01 = texture2D( tex, t + vec2( 0.0, texelSize ) );
	vec4 t11 = texture2D( tex, t + vec2( texelSize, texelSize ) );
	vec4 tB = mix( t01, t11, f.x );
	return mix( tA, tB, f.y );
}	

void main(void)
{
	vec4 displacedVertexPos;
	vec4 displacementVec;
	float f;
	
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
	
	//displacementVec = texture2D(heightMap, gl_MultiTexCoord1.xy);
	displacementVec = tex2D_bilinear(heightMap, gl_MultiTexCoord1.xy);
	f = (0.30*displacementVec.x + 0.59*displacementVec.y + 0.11*displacementVec.z);

	displacedVertexPos = vec4(gl_Normal * f * 100.0, 0.0) + gl_Vertex;
	
	gl_Position = gl_ModelViewProjectionMatrix * displacedVertexPos;
}
