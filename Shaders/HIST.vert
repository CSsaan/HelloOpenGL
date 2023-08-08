// #version 330 core

//precision lowp float;
attribute vec2 vertexIn;

void main(void)
{
    gl_Position = vec4(vertexIn, 0.0, 1.0);
}
