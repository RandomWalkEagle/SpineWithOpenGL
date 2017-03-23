uniform sampler2D texture;
varying mediump vec4 texc;
varying lowp vec4 DestinationColor;
void main(void)
{
    //lowp vec4 tc = texture2D(texture, texc.st);
   // gl_FragColor = DestinationColor * tc;

    gl_FragColor = texture2D(texture, texc.st);
};