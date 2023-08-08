// #version 330 core

//precision lowp float;
attribute vec2 vertexIn;
attribute vec2 textureIn;
varying vec2 textureOut;

void main(void)
{
    gl_Position = vec4(vertexIn.x, -vertexIn.y, 0.0, 1.0);
    textureOut = textureIn;
}
