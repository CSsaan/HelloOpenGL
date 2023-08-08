// #version 330 core 
// ffmpeg -i D:\Users\Desktop\in.mp4 -s 1920x1080 -pix_fmt yuyv422 D:\Users\Desktop\yuyv.yuv

//precision lowp float;
varying vec2 textureOut; // ori size
uniform sampler2D texs_map;
uniform sampler2D texs_fbo;

void main(void)
{   
    vec4 FboColor = texture2D(texs_fbo, textureOut);

    vec4 MapColor = texture2D(texs_map, textureOut);
    vec4 new_MapColor = MapColor;

    vec4 result = mix(new_MapColor, FboColor, FboColor);
    gl_FragColor = vec4(result.rgb, 1.0);

    // if(FboColor.r >0.5 || FboColor.g>0.5 || FboColor.b >0.5)
    // {
    //     MapColor.rgb = vec3(1.0);
    // }
    // gl_FragColor = vec4(MapColor.rgb, 1.0);
}
