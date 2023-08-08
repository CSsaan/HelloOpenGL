// #version 330 core 
// ffmpeg -i D:\Users\Desktop\out.mp4 -s 1920x1080 -pix_fmt yuyv422 D:\Users\Desktop\yuyv.yuv
// ffmpeg -i D:\Users\Desktop\out.mp4 -s 1920x1080 -pix_fmt nv12 D:\Users\Desktop\nv12.yuv

//precision lowp float;
varying vec2 textureOut; 
uniform float alphabscope;
uniform int pointsR[256];
uniform int pointsG[256];
uniform int pointsB[256];
uniform int pointsY[256];

int getOneMax(int color)
{
    int Max = 0;
    if(color == 1)
    {
        for(int i = 0; i < 256; i++)
        {
            if(Max < pointsR[i])
                Max = pointsR[i];
        }
    }else if(color == 2)
    {
        for(int i = 0; i < 256; i++)
        {
            if(Max < pointsG[i])
                Max = pointsG[i];
        }
    }else if(color == 3)
    {
        for(int i = 0; i < 256; i++)
        {
            if(Max < pointsB[i])
                Max = pointsB[i];
        }
    }else
    {
        for(int i = 0; i < 256; i++)
        {
            if(Max < pointsY[i])
                Max = pointsY[i];
        }
    }
    return Max;
}

int getTotalMax()
{
    int Max = 0;
    for(int i = 0; i < 256; i++)
    {
        if(Max < pointsR[i])
            Max = pointsR[i];
        if(Max < pointsG[i])
            Max = pointsG[i];
        if(Max < pointsB[i])
            Max = pointsB[i];
        if(Max < pointsY[i])
            Max = pointsY[i];
    }
    return Max;
}

float getOneColorResult(int color, ivec2 uv)
{   
    float resultColor;
    if(color == 1)
    {
        resultColor = float(uv.y < pointsR[uv.x]);
    }
    else if(color == 2)
    {
        resultColor = float(uv.y < pointsG[uv.x]);
    }
    else if(color == 3)
    {
        resultColor = float(uv.y < pointsB[uv.x]);
    }
    else
    {
        resultColor = float(uv.y < pointsY[uv.x]);
    }
    return resultColor;
}

void main(void)
{
    int show_one_colorBar = 1; // One_color:[1];   RGB_colors:[other]
    int choseColor = 4; // [One_color] r:1 g:2 b:3 y:4

    if(show_one_colorBar == 1)
    {
        int Max = getOneMax(choseColor); // r:1 g:2 b:3 y:4
        vec2 new_textureOut = vec2((textureOut.x*1.04)+0.02, textureOut.y);
        ivec2 UV = ivec2(new_textureOut * vec2(255, Max));
        float resultColor = getOneColorResult(choseColor, UV);
        
        gl_FragColor = vec4(vec3(resultColor), alphabscope);
    }
    else
    {
        int MaxRGBY = getTotalMax();
        ivec2 UV = ivec2(textureOut * vec2(255, MaxRGBY));
        float resultR = float(UV.y < pointsR[UV.x]);
        float resultG = float(UV.y < pointsG[UV.x]);
        float resultB = float(UV.y < pointsB[UV.x]);
        float resultY = float(UV.y < pointsY[UV.x]);

        vec3 mix_color = mix(mix(mix(vec3(0.0), vec3(0.0, 0.0, resultB), resultB*1.0),vec3(0.0, resultG, 0.0), resultG*0.5),vec3(resultR, 0.0, 0.0), resultR*0.6);

        gl_FragColor = vec4(vec3(mix_color), alphabscope);
    }
}

