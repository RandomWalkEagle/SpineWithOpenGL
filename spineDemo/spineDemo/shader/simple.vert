attribute highp vec4 vertex;
attribute highp vec4 color;
attribute mediump vec4 texCoord;

varying mediump vec4 texc;
uniform mediump mat4 matrix;
varying vec4 DestinationColor;

void main(void)
{
    DestinationColor = color;
    gl_Position = matrix * vertex;
    texc = texCoord;
};