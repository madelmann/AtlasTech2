
#define MAX_LIGHTS 8
#define NUM_LIGHTS 1

varying vec3 lightVec[MAX_LIGHTS]; 
varying vec3 viewVec;

uniform sampler2D colorMap;
uniform sampler2D normalMap;

void main (void)
{
	vec2 uv = gl_TexCoord[0].st;// * 4.0;
	vec4 base = texture2D(colorMap, uv);

	vec4 final_color = vec4(0.2, 0.2, 0.2, 1.0) * base;

	vec3 vVec = normalize(viewVec);
	vec3 bump = normalize(texture2D(normalMap, uv).xyz * 2.0 - 1.0);

	bump.x *= -1.0;
	vec3 R = reflect(-vVec, bump);

	for ( int i = 0; i < NUM_LIGHTS; ++i ) {	
		vec3 lVec = normalize(lightVec[i]);
		float diffuse = max(dot(lVec, bump), 0.0);

		vec4 vDiffuse = gl_FrontLightProduct[i].diffuse * diffuse * base;	
		final_color += vDiffuse;

		float specular = pow(clamp(dot(R, lVec), 0.0, 1.0), gl_FrontMaterial.shininess);
		vec4 vSpecular = gl_FrontLightProduct[i].specular * specular * diffuse;	
		final_color += vSpecular;
	}



    float fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
    fog = clamp(fog, 0.0, 1.0);

    final_color = vec4(mix( vec3(gl_Fog.color), vec3(final_color), fog), final_color.a);

    gl_FragColor = final_color;


/*
	gl_FragColor = final_color;	
	gl_FragColor.a = 1.0;
*/
}