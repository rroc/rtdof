uniform float distanceScale;

varying vec3 vNormal;
varying vec3 vViewVec;
varying vec4 vVertexColor;

void main(void)
	{
	gl_Position = ftransform();
//	vTexCoord = vec2(gl_MultiTexCoord0);

	// Eye-space lighting
	vNormal = gl_NormalMatrix * gl_Normal;

	// We multiply with distance scale in the vertex shader
	// instead of the fragment shader to improve performance.
	vViewVec = vec3(distanceScale * gl_ModelViewMatrix * gl_Vertex);
	
	vVertexColor = gl_FrontMaterial.diffuse.rgba;
	}
