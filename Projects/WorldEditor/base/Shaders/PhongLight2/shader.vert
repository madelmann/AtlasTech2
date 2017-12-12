
uniform float lightRadius;

varying vec3 lightDir;
varying vec3 viewDir;
varying vec3 normal;

void main()
{
    vec3 vertexPos = vec3(gl_ModelViewMatrix * gl_Vertex);

    lightDir = (gl_LightSource[0].position.xyz - vertexPos) / lightRadius;
    viewDir = -vertexPos;
    normal = normalize(gl_NormalMatrix * gl_Normal);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;    
}
