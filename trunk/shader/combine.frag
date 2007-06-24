uniform float range;
uniform float focus;
uniform sampler2D RT;
uniform sampler2D Blur;

void main(void)
	{
	vec4 sharp = texture2D(RT,   gl_TexCoord[0].st);
	vec4 blur  = texture2D(Blur, gl_TexCoord[0].st);

	//ONLY ALPHA:
	//gl_FragColor = vec4( sharp.a, sharp.a, sharp.a, 1.0);

	//MIX the initial image and blurred one
	gl_FragColor = mix(sharp, blur, clamp(range * abs(focus - sharp.a), 0.0, 1.0));
	}