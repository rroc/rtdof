uniform sampler2D Base;
uniform vec4 lightDir;
uniform float Ks;
uniform float Kd;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vViewVec;

void main(void)
{
   vec3 base = texture2D(Base, vTexCoord).xyz;

   // Basic lighting
   float diffuse = dot(lightDir.xyz, vNormal);
   float specular = pow(clamp(dot(reflect(-normalize(vViewVec), vNormal), lightDir.xyz),0.0, 1.0), 16.0);
   vec3  light = Kd * diffuse * base + Ks * specular;

   // We'll use the distance to decide how much blur we want
   float dist = length(vViewVec);

   gl_FragColor = vec4( light, dist);
}