
varying vec3 Normal;
varying vec3 Position;
varying vec3 Vertex;

void main(void)
{
    vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
    Vertex = vec3(gl_Vertex);
    Position = (vec3 (ecPosition)) / ecPosition.w;
    Normal = gl_NormalMatrix * gl_Normal;;
    gl_Position    = ftransform();
}
