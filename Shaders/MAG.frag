// #version 330 core 
//precision lowp float;
varying vec2 textureOut; 
uniform sampler2D tex_yuyv;
uniform vec2 frameWH;
uniform float sharpenStr;
int video_flip_type = 0;
uniform vec4 cunstom_Params;// = vec4(0.250000, 0.750000, 0.277778, 0.722222);
uniform vec3 cunstom_Params_plus; // = vec3(2.000000, 0.250000, 0.250000);
uniform float MagLineWifth; // = 2.0;
vec4 iColor = vec4(1.0, 0.0, 0.0, 1.0);

// 新方法：效果看不出条纹
vec3 YUV2RGB4(vec2 texCoord)
{
    const lowp vec3 offset = vec3(-0.0625, -0.5, -0.5);
    const lowp mat3 converter = mat3(1.164, 1.164, 1.164,   0.000,-0.391, 2.018,   1.596,-0.813, 0.000);
    lowp vec4 yuyv = texture2D(tex_yuyv, texCoord);
//    highp vec4 yuyv = texelFetch(tex_yuyv, ivec2(int(texCoord.x * frameWH.x/2.0), int(texCoord.y * frameWH.y)), 0);
    lowp float tx = texCoord.x * frameWH.x;
    lowp float odd = floor(mod(tx, 2.0));
    lowp float y = odd * yuyv.z + (1.0 - odd) * yuyv.x;

    lowp vec3 yuv = vec3(y, yuyv.y, yuyv.w);
    lowp vec3 rgb = converter * (yuv + offset);
    return rgb;
}
// 新方法：无条格
float getY(vec2 tex)
{
    highp vec4 yuyv = texture2D(tex_yuyv, tex);
    highp float tx = tex.x * frameWH.x;
    float odd = floor(mod(tx, 2.0));
    float y = odd * yuyv.z + (1.0 - odd) * yuyv.x;
        return y;
}

vec2 getMagCoord(vec2 oriCoord)
{
    vec2 new_vv2_Texcoord;
    new_vv2_Texcoord.x = clamp(oriCoord.x, 0.0, 1.0);
    new_vv2_Texcoord.y = clamp(oriCoord.y, 0.0, 1.0);

    float x = video_flip_type == 0x1 ? (1.0 - oriCoord.x) : oriCoord.x;
    float y = video_flip_type == 0x2 ? (1.0 - oriCoord.y) : oriCoord.y;
    float l = cunstom_Params.x;
    float r = cunstom_Params.y;
    float t = cunstom_Params.z;
    float b = cunstom_Params.w;
    float ratio = cunstom_Params_plus.x;
    float offsetX = cunstom_Params_plus.y;
    float offsetY = cunstom_Params_plus.z;
    if (x > l && x < r && y > t && y < b)
    {
        float xp = x / ratio + offsetX;
        float yp = y / ratio + offsetY;
        if (video_flip_type == 0x1)
        {
            xp = 1.0 - xp;
        }
        else if (video_flip_type == 0x2)
        {
            yp = 1.0 - yp;
        }
        new_vv2_Texcoord = vec2(xp, yp);
    }

    return new_vv2_Texcoord;
}


vec3 magnifier(vec3 color)
{
    float x = video_flip_type == 1 ? (1.0 - textureOut.x) : textureOut.x;
    float y = video_flip_type == 2 ? (1.0 - textureOut.y) : textureOut.y;
    float l = cunstom_Params.x;
    float r = cunstom_Params.y;
    float t = cunstom_Params.z;
    float b = cunstom_Params.w;
    float ratio = cunstom_Params_plus.x;
    float offsetX = cunstom_Params_plus.y;
    float offsetY = cunstom_Params_plus.z;
    vec3 rgb = vec3(0.0);

    float xp1 = x / ratio + offsetX;
    float yp1 = y / ratio + offsetY;
    if (xp1 > l && xp1 < r && yp1 > t && yp1 < b)
    {
        float xp = xp1 / ratio + offsetX;
        float yp = yp1 / ratio + offsetY;
        if (video_flip_type == 1)
        {
            xp = 1.0 - xp;
        }
        else if (video_flip_type == 2)
        {
            yp = 1.0 - yp;
        }
        rgb = YUV2RGB4(vec2(xp, yp));

        if (abs(xp1 - l) < 0.001 * MagLineWifth || abs(xp1 - r) < 0.001 * MagLineWifth || abs(yp1 - t) < 0.001 * MagLineWifth || abs(yp1 - b) < 0.001 * MagLineWifth)
        {
            rgb = iColor.rgb;
        }
    }
    return rgb;
}

void main(void)
{
    vec4 rgba = vec4(YUV2RGB4(textureOut), 1.0);
    vec3 color = rgba.rgb;
    color = magnifier(color);

    gl_FragColor = vec4(color, color.r>0.0?1.0:0.0);
}
