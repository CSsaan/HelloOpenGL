// #version 330 core 
//precision lowp float;
varying vec3 textureOut; 

void main(void)
{
    gl_FragColor = vec4(textureOut, 1.0);
}
