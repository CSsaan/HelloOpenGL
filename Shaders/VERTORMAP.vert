// #version 330 core

//precision lowp float;
attribute vec2 vertexIn_map;
attribute vec2 textureIn_map;
varying vec2 textureOut;

void main(void)
{
    gl_Position = vec4(vertexIn_map, 0.0, 1.0);
    textureOut = textureIn_map;
}
