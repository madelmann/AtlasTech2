
uniform sampler2D colorMap;

void main(void)
{
    vec4 color;
	color = texture2D(colorMap, gl_TexCoord[0].xy);
    color += gl_SecondaryColor;
    color = clamp(color, 0.0, 1.0);

    float fog;
    fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
    fog = clamp(fog, 0.0, 1.0);

    color = vec4(mix( vec3(gl_Fog.color), vec3(color), fog), color.a);
    gl_FragColor = color;
}
