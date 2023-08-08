//#version 330 core

attribute vec2 vertexIn;
attribute vec2 textureIn;
uniform mat4 transform;
varying vec2 textureOut;

void main(void)
{
    gl_Position = transform * vec4(vertexIn, 0.0, 1.0);
    textureOut = textureIn;
}
