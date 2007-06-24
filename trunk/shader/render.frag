uniform float Ks;
uniform float Kd;

varying vec3 vNormal;
varying vec3 vViewVec;
varying vec3 vVertexColor;

void main(void)
	{
	vec4 lightDir = gl_LightSource[0].position;

	//vVertexColor = gl_Color.rgb;

	//Phong lighting:
	float diffuse = dot(lightDir.xyz, vNormal);
	float specular = pow(clamp(dot(reflect(-(lightDir.xyz), vNormal), normalize(vViewVec.xyz)),0.0, 1.0), 16.0);
	vec3  light = Kd * diffuse * vVertexColor + Ks * specular;

	//We'll use the distance to decide how much blur we want
	float dist = length( vViewVec );

	//Fragment color with alpha value stored in.	
	gl_FragColor = vec4( light.rgb, dist );
	}
