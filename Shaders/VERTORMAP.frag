// #version 330 core 
// ffmpeg -i D:\Users\Desktop\out.mp4 -s 1920x1080 -pix_fmt yuyv422 D:\Users\Desktop\yuyv.yuv

//precision lowp float;
varying vec2 textureOut; // ori size
uniform sampler2D texs_map;

void main(void)
{   
    vec4 MapColor = texture2D(texs_map, textureOut);

    gl_FragColor = vec4(MapColor.rgb, 1.0);
}
