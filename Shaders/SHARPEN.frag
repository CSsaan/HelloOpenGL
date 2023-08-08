// #version 330 core 
//precision lowp float;
varying vec2 textureOut; 
uniform sampler2D tex_yuyv;
uniform vec2 frameWH;
uniform float sharpenStr;

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

// vec3 YUV2RGB2(vec2 tex)
// {
//     vec4 yuyv = texture2D(tex_yuyv, tex);
//     float Y = yuyv.x;
//     float U = yuyv.y - 0.5;
//     float V = yuyv.w - 0.5;
//     vec2 texOffset = 1.0 / textureSize(tex_yuyv, 0);
//     vec3 yuv00 = vec3(texture2D(tex_yuyv, tex - texOffset).x, U, V);
//     vec3 yuv10 = vec3(texture2D(tex_yuyv, tex + vec2(texOffset.x, -texOffset.y)).x, U, V);
//     vec3 yuv01 = vec3(texture2D(tex_yuyv, tex + vec2(-texOffset.x, texOffset.y)).x, U, V);
//     vec3 yuv11 = vec3(texture2D(tex_yuyv, tex + texOffset).x, U, V);
//     vec3 yuv = mix(mix(yuv00, yuv10, fract(tex.x)), mix(yuv01, yuv11, fract(tex.x)), fract(tex.y));
//     vec3 rgb = mat3( 1.164144,   1.164144,   1.164144,
//                     0,         -0.213289,   2.11236,
//                     1.791733,  -0.533916,   0) * yuv;
//     return rgb;
// }

// vec3 YUV2RGB3(vec2 texCoord)
// {
//     vec4 yuyv = texture2D(tex_yuyv, texCoord);
//     vec3 yuv0 = vec3(yuyv.x-0.062745, yuyv.yw-vec2(0.501961,0.501961));
//     vec3 yuv1 = vec3(yuyv.z-0.062745, yuyv.yw-vec2(0.501961,0.501961));
//     vec3 rgb0 = mat3( 1.164144,   1.164144,   1.164144,
//                     0,         -0.213289,   2.11236,
//                     1.791733,  -0.533916,   0) * yuv0;
//     vec3 rgb1 = mat3( 1.164144,   1.164144,   1.164144,
//                     0,         -0.213289,   2.11236,
//                     1.791733,  -0.533916,   0) * yuv1;
//     return (rgb0 + rgb1) * 0.5;
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

vec3 mtexSample(vec2 fragCoord, const float x, const float y)
{
    vec2 uv = fragCoord + vec2(x / frameWH.x, y / frameWH.y);
    lowp vec3 textureColor = YUV2RGB4(uv);
    return textureColor;
}

vec3 sharpen(vec2 fragCoord, float strength)
{
    vec3 f =
        mtexSample(fragCoord, -1.0, -1.0) * -1.0 +
        mtexSample(fragCoord, 0.0, -1.0) * -1.0 +
        mtexSample(fragCoord, 1.0, -1.0) * -1.0 +
        mtexSample(fragCoord, -1.0, 0.0) * -1.0 +
        mtexSample(fragCoord, 0.0, 0.0) * 9.0 +
        mtexSample(fragCoord, 1.0, 0.0) * -1.0 +
        mtexSample(fragCoord, -1.0, 1.0) * -1.0 +
        mtexSample(fragCoord, 0.0, 1.0) * -1.0 +
        mtexSample(fragCoord, 1.0, 1.0) * -1.0;
    return mix(mtexSample(fragCoord, 0.0, 0.0), f, strength);
}

void main(void)
{
    vec3 SharpenColor = sharpen(textureOut, sharpenStr);
    gl_FragColor = vec4(SharpenColor, 1.0);
}
