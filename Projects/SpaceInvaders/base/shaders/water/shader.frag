varying vec4 refrCoords; 
varying vec4 normCoords; 
varying vec4 viewCoords;
varying vec4 viewTangetSpace;
varying vec4 lightTangetSpace;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D normalMap;
uniform sampler2D dudvMap;
uniform sampler2D depthMap;
uniform vec4 waterColor;
uniform int fresnelMode;


void main()
{
	const float kShine = 128.0;
	const float kDistortion = 0.015;
	const float kRefraction = 0.009;

	vec4 distOffset = texture2D(dudvMap, normCoords.xy) * kDistortion;
	vec4 dudvColor = texture2D(dudvMap, vec2(refrCoords + distOffset));
	dudvColor = normalize(dudvColor * 2.0 - 1.0) * kRefraction;

	vec4 normalVector = texture2D(normalMap, vec2(refrCoords + distOffset));
	normalVector = normalVector * 2.0 - 1.0;
	normalVector.a = 0.0;

	vec4 lightReflection = normalize( reflect(-lightTangetSpace, normalVector) );

	float invertedFresnel = dot(normalVector, lightReflection);
	float fresnelTerm = 1.0 - invertedFresnel;
	
	if ( fresnelMode == 0 )
	{
		invertedFresnel = dot(normalVector, lightReflection);
		//correction
		invertedFresnel = 1.0 - invertedFresnel * 1.3;
	}
	if ( fresnelMode == 1 )
	{
		fresnelTerm = 0.02 + 0.97 * pow( (1.0 - dot(normalVector, lightReflection)), 5.0);
	}
	if ( fresnelMode == 2 )
	{
		float fangle = 1.0 + dot(normalVector, lightReflection);
		fangle = pow(fangle, 5.0);
		fresnelTerm = 1.0 / fangle;
	}

	vec4 projCoord = viewCoords / viewCoords.q;
	projCoord = (projCoord + 1.0) * 0.5;
	projCoord += dudvColor;
	projCoord = clamp(projCoord, 0.001, 0.999);
	
	vec4 reflectionColor  = texture2D(reflection, projCoord.xy);
	vec4 refractionColor  = texture2D(refraction, projCoord.xy);
	vec4 depthValue = texture2D(depthMap, projCoord.xy);
		
	vec4 invDepth = 1.0 - depthValue;
	refractionColor *= invertedFresnel * invDepth;
	refractionColor +=  waterColor * depthValue * invertedFresnel;

	reflectionColor *= fresnelTerm;

	vec4 localView = normalize(viewTangetSpace);		
	float intensity = max(0.0, dot(lightReflection, localView) );
	vec4 specular = vec4(pow(intensity, kShine));

	gl_FragColor = reflectionColor + specular;
}