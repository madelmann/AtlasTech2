
varying vec3 lightVec; 
varying vec3 eyeVec;
varying vec2 texCoord;
attribute vec3 vTangent;

void main(void)
{
	gl_Position = ftransform();
	texCoord = gl_MultiTexCoord0.xy;

	// if the engine does not provide the tangent vector you 
	// can compute it with the following piece of of code:
	{	// calculate tangent
		vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0)); 
		vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0)); 

		if ( length(c1) > length(c2) ) {
			vTangent = c1;	
		}
		else {
			vTangent = c2;	
		}

		vTangent = normalize(vTangent);
	}
	
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * vTangent);
	vec3 b = cross(n, t);

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 tmpVec = gl_LightSource[0].position.xyz - vVertex;

	lightVec.x = dot(tmpVec, t);
	lightVec.y = dot(tmpVec, b);
	lightVec.z = dot(tmpVec, n);

	tmpVec = -vVertex;
	eyeVec.x = dot(tmpVec, t);
	eyeVec.y = dot(tmpVec, b);
	eyeVec.z = dot(tmpVec, n);
}
