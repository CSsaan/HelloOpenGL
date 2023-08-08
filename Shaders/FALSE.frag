#version 330 core
//precision lowp float;
varying vec2 textureOut; 
uniform sampler2D tex_yuyv;
uniform vec2 frameWH;

// float getY(vec2 tex)
// {
// 	vec4 yuyv = texture2D(tex_yuyv, tex);
// 	vec2 yy = yuyv.xz;
// 	float cur_y = (yy.x + yy.y) * 0.5;
// 	return cur_y;
// }

// 新方法：无条格
float getY(vec2 tex)
{
    highp vec4 yuyv = texture2D(tex_yuyv, tex);
    highp float tx = tex.x * frameWH.x;
    float odd = floor(mod(tx, 2.0));
    float y = odd * yuyv.z + (1.0 - odd) * yuyv.x;
	return y;
} 

vec3 getFalseColor(float y) 
{
    vec3 outcolor;
    float num = 1.0 / 255.0 * (219.0 / 255.0);
    if (y < 8.0 * num)
    {
        outcolor = vec3(123.0, 106.0, 176.0) / 255.0;
    }
    else if ((y >= 8.0 * num) && (y <= 10.0 * num))
    {
        outcolor = vec3(2.0, 117.0, 190.0) / 255.0;
    }
    else if ((y >= 97.0 * num) && (y <= 108.0 * num))
    {
        outcolor = vec3(129.0, 194.0, 64.0) / 255.0;
    }
    else if ((y >= 133.0 * num) && (y <= 143.0 * num))
    {
        outcolor = vec3(244.0, 137.0, 183.0) / 255.0;
    }
    else if ((y >= 248.0 * num) && (y < 253.0 * num))
    {
        outcolor = vec3(254.0, 242.0, 60.0) / 255.0;
    }
    else if ((y >= 253.0 * num) && (y <= 255.0 * num))
    {
        outcolor = vec3(238.0, 47.0, 45.0) / 255.0;
    }
    else
    {
        outcolor = vec3(y);
    }
    // float l = cunstom_Params.x;
    // float r = cunstom_Params.y;
    // float t = cunstom_Params.z;
    // float b = cunstom_Params.w;
    // float factor = iframeWH.x / iframeWH.y;
    // float x = video_flip_type == 0x1 ? (1.0 - vv2_Texcoord.x) : vv2_Texcoord.x;
    // float y = video_flip_type == 0x2 ? (1.0 - vv2_Texcoord.y) : vv2_Texcoord.y;
    // vec2 new_vv2_Texcoord = vec2(x, y);
    // if (new_vv2_Texcoord.x > l && new_vv2_Texcoord.x < r && new_vv2_Texcoord.y > t && new_vv2_Texcoord.y < b)
    // {
    //     if (abs(new_vv2_Texcoord.x - l) < 0.0007 * MagLineWifth || abs(new_vv2_Texcoord.x - r) < 0.0007 * MagLineWifth || abs(new_vv2_Texcoord.y - t) < 0.0007 * MagLineWifth * factor || abs(new_vv2_Texcoord.y - b) < 0.0007 * MagLineWifth * factor)
    //     {
    //         outcolor = iColor.rgb;
    //     }
    // }
    return outcolor;
}

void main(void)
{
    // float gray = dot(YUV2RGB(textureOut),vec3(0.3,0.59,0.11));
    vec3 FalseColor = getFalseColor(getY(textureOut));
    gl_FragColor = vec4(FalseColor, 1.0);
}
