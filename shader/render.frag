/*
uniform float Ks;
uniform float Kd;

varying vec3 vNormal;
varying vec3 vViewVec;
//varying vec4 vVertexColor;

void main(void)
{
   vec3 base = vec3 ( 1.0, 1.0, 1.0 );
   
   vec4 lightDir    = gl_LightSource[0].position;
	//vec4 material = gl_FrontMaterial.diffuse.rgba;

   //vec4 vVertexColor = gl_FrontMaterial.diffuse.rgba;
   vec4 vVertexColor = gl_FrontLightModelProduct.sceneColor 
						+ 0.0 * gl_FrontMaterial.ambient 
						+ 0.9 * gl_FrontMaterial.diffuse;
   vVertexColor += 0.9 * gl_FrontMaterial.specular;

   // Basic lighting with phong
   float diffuse = dot(lightDir.xyz, vNormal);
   float specular = pow(clamp(dot(reflect(normalize(vViewVec), vNormal), lightDir.xyz),0.0, 1.0), 16.0);
   vec3  light = Kd * diffuse* vVertexColor.rgb + Ks * specular;

   // We'll use the distance to decide how much blur we want
   float dist = length(vViewVec);

   //gl_FragColor = vec4( dist,dist,dist, 1.0 );//light.rgb, dist );
   //gl_FragColor = vec4( gl_FrontMaterial.diffuse.rgba ); //vVertexColor.rgb, 1.0 );
   gl_FragColor = vec4( light.rgb, 1.0 );
   }
   */

uniform float Ks;
uniform float Kd;

varying vec3 vNormal;
varying vec3 vViewVec;
varying vec3 vVertexColor;

void main(void)
	{
	vec4 lightDir = gl_LightSource[0].position;

	// Basic lighting
	float diffuse = dot(lightDir.xyz, vNormal);
	float specular = pow(clamp(dot(reflect(-normalize(vViewVec), vNormal), lightDir.xyz),0.0, 1.0), 16.0);
	vec3  light = Kd * diffuse * vVertexColor + Ks * specular;

	// We'll use the distance to decide how much blur we want
	float dist = length( vViewVec );

	//Fragment color with alpha value stored in.	
	gl_FragColor = vec4( light.rgb, dist );
	}

   
   