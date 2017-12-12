
uniform sampler2D colorMap;
uniform sampler2D normalMap;

varying vec3 lightDir;
varying vec3 halfVector;

float ffog(in float ecDistance)
{
    return(abs(ecDistance));
}

void main()
{
    vec2 newTexCoord;
    vec3 h = normalize(halfVector);

    newTexCoord = gl_TexCoord[0].st;

    vec3 n = normalize(texture2D(normalMap, newTexCoord).rgb * 2.0 - 1.0);
    vec3 l = normalize(lightDir);

    float nDotL = max(0.0, dot(n, l));
    float nDotH = max(0.0, dot(n, h));
    float power = (nDotL == 0.0) ? 0.0 : pow(nDotH, gl_FrontMaterial.shininess);

    vec4 ambient = gl_FrontLightProduct[0].ambient;
    vec4 diffuse = gl_FrontLightProduct[0].diffuse * nDotL;
    vec4 specular = gl_FrontLightProduct[0].specular * power;
    vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse + specular;

//    gl_FragColor = color * texture2D(colorMap, newTexCoord);


	color = color * texture2D(colorMap, newTexCoord);

    float fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
    fog = clamp(fog, 0.0, 1.0);

    color = vec4(mix( vec3(gl_Fog.color), vec3(color), fog), color.a);

    gl_FragColor = color;
}
