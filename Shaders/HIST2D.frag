// // #version 330 core 
// ffmpeg -i D:\Users\Desktop\out.mp4 -s 1920x1080 -pix_fmt yuyv422 D:\Users\Desktop\yuyv.yuv
// ffmpeg -i D:\Users\Desktop\out.mp4 -s 1920x1080 -pix_fmt nv12 D:\Users\Desktop\nv12.yuv
// precision lowp float;
// varying vec2 textureOut; // ori size

// void main(void)
// {   
//     gl_FragColor = vec4(1.0,1.0,1.0, 1.0);
// }

// #version 330 core 
//precision lowp float;

void main(void)
{
    // float gray = dot(YUV2RGB(textureOut),vec3(0.3,0.59,0.11));
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
