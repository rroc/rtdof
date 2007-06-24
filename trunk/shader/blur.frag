uniform float sampleDist;
uniform sampler2D RT;

varying vec2 vTexCoord;

void main(void)
{
   vec2 samples00 = vec2(-0.326212, -0.405805);
   vec2 samples01 = vec2(-0.840144, -0.073580);
   vec2 samples02 = vec2(-0.695914,  0.457137);
   vec2 samples03 = vec2(-0.203345,  0.620716);
   vec2 samples04 = vec2( 0.962340, -0.194983);
   vec2 samples05 = vec2( 0.473434, -0.480026);
   vec2 samples06 = vec2( 0.519456,  0.767022);
   vec2 samples07 = vec2( 0.185461, -0.893124);
   vec2 samples08 = vec2( 0.507431,  0.064425);
   vec2 samples09 = vec2( 0.896420,  0.412458);
   vec2 samples10 = vec2(-0.321940, -0.932615);
   vec2 samples11 = vec2(-0.791559, -0.597705);
   
   // vTexCoord = gl_TexCoord[0].st;

   vec2 newCoord;
   vec4 sum = texture2D(RT, vTexCoord);
   
   /*
   for (int i = 0; i < 12; i++)
   {
      sum += tex2D(RT, texCoord + sampleDist * samples[i]);
   }
   */
   
   newCoord = vTexCoord + sampleDist * samples00;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples01;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples02;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples03;
   sum += texture2D(RT, newCoord);
   
   newCoord = vTexCoord + sampleDist * samples04;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples05;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples06;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples07;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples08;
   sum += texture2D(RT, newCoord);
   
   newCoord = vTexCoord + sampleDist * samples09;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples10;
   sum += texture2D(RT, newCoord);

   newCoord = vTexCoord + sampleDist * samples11;
   sum += texture2D(RT, newCoord);

   sum /= 13.0;

   gl_FragColor = sum; //vec4( 1.0, 1.0, 1.0, 1.0); // sum );
}
