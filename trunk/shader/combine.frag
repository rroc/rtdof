uniform float range;
uniform sampler2D RT;
uniform sampler2D Blur1;
uniform float focus;

varying vec2 vTexCoord;


void main(void)
{
   vec4 sharp = texture2D(RT,    vTexCoord);
   vec4 blur  = texture2D(Blur1, vTexCoord);

   gl_FragColor = mix(sharp, blur, clamp(range * abs(focus - sharp.a), 0.0, 1.0));
}