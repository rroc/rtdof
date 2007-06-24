uniform float range;
uniform float focus;
uniform sampler2D RT;
uniform sampler2D Blur;

varying vec2 vTexCoord;

void main(void)
{
   //vec2 vTexCoord = gl_TexCoord[0].st;
   vec4 sharp = texture2D(RT,   vTexCoord);
   vec4 blur  = texture2D(Blur, vTexCoord);

   //gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0); //mix(sharp, blur, clamp(range * abs(focus - sharp.a), 0.0, 1.0));
   gl_FragColor = mix(sharp, blur, clamp(range * abs(focus - sharp.a), 0.0, 1.0));
   //gl_FragColor = vec4( sharp.rgb , 1.0); //vec4( focus, focus, focus, 1.0); //mix(sharp, blur, clamp(range * abs(focus - sharp.a), 0.0, 1.0));
}