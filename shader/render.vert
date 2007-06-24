uniform float distanceScale;

varying vec3 vNormal;
varying vec3 vViewVec;
varying vec3 vVertexColor;

void main(void)
{
	//3d Screen space:
	// (These two are the same)
	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
	
	
	// For eye-space lighting:
	vNormal = gl_NormalMatrix * gl_Normal;

	// We multiply with distance scale in the vertex shader
	// instead of the fragment shader to improve performance.
	vViewVec = -vec3(distanceScale * gl_ModelViewMatrix * gl_Vertex);

	//Have our colors interpolated as well
	vVertexColor = gl_Color.rgb; //gl_FrontMaterial.diffuse.rgb;
}
