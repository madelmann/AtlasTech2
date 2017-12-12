
void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = vec4(gl_Normal, 0.0);
	gl_Position    = ftransform();
}
