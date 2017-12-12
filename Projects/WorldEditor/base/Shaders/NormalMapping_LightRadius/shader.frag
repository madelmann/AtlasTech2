
uniform sampler2D colorMap;
uniform sampler2D normalMap;

varying vec3 lightDir;
varying vec3 viewDir;

void main()
{
    vec3 l = lightDir;
    float atten = max(0.0, 1.0 - dot(l, l));

    l = normalize(l);

    vec3 n = normalize(texture2D(normalMap, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
    vec3 v = normalize(viewDir);
    vec3 h = normalize(l + v);

    float nDotL = max(0.0, dot(n, l));
    float nDotH = max(0.0, dot(n, h));
    float power = (nDotL == 0.0) ? 0.0 : pow(nDotH, gl_FrontMaterial.shininess);
    
    vec4 ambient = gl_FrontLightProduct[0].ambient * atten;
    vec4 diffuse = gl_FrontLightProduct[0].diffuse * nDotL * atten;
    vec4 specular = gl_FrontLightProduct[0].specular * power * atten;
    vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse + specular;
    
    gl_FragColor = color * texture2D(colorMap, gl_TexCoord[0].st);
}
