// #version 330 core

//precision lowp float;
attribute vec3 vertexIn;
varying vec3 textureOut;

void main(void)
{
    float Cb = 0.5 + dot(vertexIn, vec3(-0.168736, -0.331264, 0.5));
	float Cr = 0.5 + dot(vertexIn, vec3(0.5, -0.418688, -0.081312));
	vec2 verpos = (vec2(Cb, Cr) - 0.5) * 2.0;
	gl_Position = vec4(verpos, 0.0, 1.0);
    gl_PointSize = 400.0;
	textureOut = vertexIn;
}
