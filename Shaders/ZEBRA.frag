// #version 330 core 
//precision lowp float;
varying vec2 textureOut; 
uniform sampler2D tex_yuyv;
uniform sampler2D texs_zebra;
uniform float zebraLow;
uniform float zebraHig;
uniform float iGlobalTime;
uniform vec2 frameWH;

// 新方法：无条格
float getY(vec2 tex)
{
    highp vec4 yuyv = texture2D(tex_yuyv, tex);
    highp float tx = tex.x * frameWH.x;
    float odd = floor(mod(tx, 2.0));
    float y = odd * yuyv.z + (1.0 - odd) * yuyv.x;
	return y;
} 

vec3 zebra(vec2 texcoord, vec3 inColor, float y, float zebraLow, float zebraHig) 
{
    // highp float num = 219.0 / 255.0;
    highp float num = 255.0 / 219.0;

    float newX = (((texcoord.x - 0.5) * 0.9 * 2.0) + 1.0) / 2.0; // 通过0.9来控制斑马纹移动时图片不会超过边缘位置
    float width = 0.007;
    float shift = mod(iGlobalTime * 6.0, 10.0) * 0.2 * width;
    lowp vec2 new_coord = vec2(newX + shift, texcoord.y);
    lowp vec4 tex = vec4(inColor, 1.0);
    highp float luminance = y * num - 16.0/219.0;
    lowp vec4 zebra_tex = texture2D(texs_zebra, new_coord);
    zebra_tex = vec4(zebra_tex.rgb, 1.0 - zebra_tex.r <= 0.5 ? 0.55 : 0.75);
    if (luminance < zebraLow || luminance > zebraHig)
    {
        zebra_tex.a = 0.0;
    }
    lowp vec3 out_color = mix(tex, zebra_tex, zebra_tex.a).rgb;
    return out_color;
}

// vec3 YUV2RGB(vec2 tex)
// {
//    vec4 yuyv = texture2D(tex_yuyv, tex);
//    vec2 yy = yuyv.xz;
//    vec2 uv = yuyv.yw;
//    float cur_y = (yy.x + yy.y) * 0.5;
//    vec3 yuv = vec3(cur_y-0.062745, uv-vec2(0.501961,0.501961));
//    vec3 rgb = mat3( 1.164144,   1.164144,   1.164144,
//                     0,         -0.213289,   2.11236,
//                     1.791733,  -0.533916,   0) * yuv;
//    return rgb;
// }

// 新方法：效果看不出条纹
vec3 YUV2RGB4(vec2 texCoord)
{
    const vec3 offset = vec3(-0.0625, -0.5, -0.5);
    const mat3 converter = mat3(1.164, 1.164, 1.164,   0.000,-0.391, 2.018,   1.596,-0.813, 0.000);
    highp vec4 yuyv = texture2D(tex_yuyv, texCoord);
    highp float tx = texCoord.x * frameWH.x;
    float odd = floor(mod(tx, 2.0));
    float y = odd * yuyv.z + (1.0 - odd) * yuyv.x;
    
    vec3 yuv = vec3(y, yuyv.y, yuyv.w);
    vec3 rgb = converter * (yuv + offset);
    return rgb;
}

void main(void)
{
    // float gray = dot(YUV2RGB4(textureOut),vec3(0.3,0.59,0.11));
    float lzebraLow = zebraLow;
    float lzebraHig = zebraHig;
    vec3 ZebraColor = zebra(textureOut, YUV2RGB4(textureOut), getY(textureOut), lzebraLow, lzebraHig);
    gl_FragColor = vec4(ZebraColor, 1.0);
}
