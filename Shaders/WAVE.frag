// #version 330 core
//precision lowp float;
varying vec2 textureOut;
uniform sampler2D tex_yuyv;
uniform sampler2D Tick_sampler;
uniform int useWaveform; // 1:Luma,2:RGB
uniform float alphabscope;
uniform vec2 frameWH;

// ##################################################
// 新方法：无条格
float getY(vec2 tex)
{
	highp vec4 yuyv = texture2D(tex_yuyv, tex);
    highp float tx = tex.x * frameWH.x;
    float odd = floor(mod(tx, 2.0));
    float y = odd * yuyv.z + (1.0 - odd) * yuyv.x;
	return y;
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

vec4 waveform_graph(float L, float R, float T, float B)
{
    highp vec2 vv2_Texcoord = textureOut;
    int hres = 110;
    float intensity = 0.04 + (0.1 * alphabscope);
    const float thres = 0.004;
    float factor = 1.0;
    float Ax = (vv2_Texcoord.x - L - 0.004) / (R - L);
    highp float Ay = 1.0 - (vv2_Texcoord.y - T) / (B - T);
    float AyTick = (vv2_Texcoord.y - T) / (B - T);
    float col = 0.0;
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
    highp float s = Ay * 0.890 + 0.045; // 第一个：越大越缩小↓， 第二个：越大越向↓移动
    float maxb = s + thres;
    float minb = s - thres;
    vec4 result;
    if (useWaveform == 1)
    {
        for (int i = 0; i <= hres; i++)
        {
            float dy = float(i) / float(hres);
             float xc = getY(vec2(Ax, 1.0 - dy));
            col += intensity * step(xc, maxb) * step(minb, xc);
            float l = xc * xc;
            col += intensity * step(l, maxb * maxb) * step(minb * minb, l);
        }
        col = col * factor;
        result = vec4(col, col, col, 1.0);
    }
    else
    {
        s =Ay * 1.050 - 0.025;
        maxb = s + thres;
        minb = s - thres;
        for (int i = 0; i <= hres; i++)
        {
            float dy = float(i) / float(hres);
            if (vv2_Texcoord.x >= L && vv2_Texcoord.x <= L + 0.333 * (R - L))
            {
                float xc = YUV2RGB4(vec2((vv2_Texcoord.x - L - 0.004) / (L + 0.333 * (R - L) - L), 1.0 - dy)).r;
                r += intensity * step(xc, maxb) * step(minb, xc);
                float l = xc * xc;
                r += intensity * step(l, maxb * maxb) * step(minb * minb, l);
            }
            else if (vv2_Texcoord.x > L + 0.333 * (R - L) && vv2_Texcoord.x <= L + 0.666 * (R - L))
            {
                float xc = YUV2RGB4(vec2((vv2_Texcoord.x - (L + 0.333 * (R - L)) - 0.004) / (L + 0.666 * (R - L) - (L + 0.333 * (R - L))), 1.0 - dy)).g;
                g += intensity * step(xc, maxb) * step(minb, xc);
                float l = xc * xc;
                g += intensity * step(l, maxb * maxb) * step(minb * minb, l);
            }
            else if (vv2_Texcoord.x > L + 0.666 * (R - L) && vv2_Texcoord.x <= R)
            {
                float xc = YUV2RGB4(vec2((vv2_Texcoord.x - (L + 0.666 * (R - L)) - 0.004) / (R - (L + 0.666 * (R - L))), 1.0 - dy)).b;
                b += intensity * step(xc, maxb) * step(minb, xc);
                float l = xc * xc;
                b += intensity * step(l, maxb * maxb) * step(minb * minb, l);
            }
        }
        result = vec4(r, g, b, 1.0);
    }
    if (mod(Ay, 0.2) <= 0.006)
        result = vec4(0.512, 0.384, 0.304, 1.0);
    vec4 Tick = vec4(0.0);
    Tick = texture2D(Tick_sampler, vec2(Ax, AyTick));
    Tick.r = smoothstep(0.0, 1.0, Tick.r);
    if (Tick.r > 0.02)
    {
        result.rgb = vec3(0.512, 0.384, 0.304) * 1.3;
    }
    return result;
}

vec3 waveform()
{
vec4 resultcolor;
resultcolor = waveform_graph(0.0, 1.0, 0.0, 1.0);
return resultcolor.rgb;
}
// ##################################################
void main(void)
{
    vec3 result = waveform();
    gl_FragColor = vec4(result, alphabscope);
    // gl_FragColor = vec4(YUV2RGB(textureOut), 1.0);
}
