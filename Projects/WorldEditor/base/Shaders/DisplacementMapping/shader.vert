
uniform sampler2D heightMap;

varying vec3 lightDir;
varying vec3 halfVector;

float ffog(in float ecDistance)
{
    return(abs(ecDistance));
}

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;

    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
    vec3 b = cross(n, t) * gl_MultiTexCoord1.w;
    
    mat3 tbnMatrix = mat3(t.x, b.x, n.x,
                          t.y, b.y, n.y,
                          t.z, b.z, n.z);
    
    lightDir = gl_LightSource[0].position.xyz;
    lightDir = tbnMatrix * lightDir;
    
    halfVector = gl_LightSource[0].halfVector.xyz;
    halfVector = tbnMatrix * halfVector;



/*
    // Eye-coordinate position of vertex, needed in various calculations
    vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;

    // Do fixed functionality vertex transform
    gl_FogFragCoord = ffog(ecPosition.z);
*/
}
