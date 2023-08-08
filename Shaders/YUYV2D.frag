//#version 330 core

varying vec2 textureOut;
uniform sampler2D m_Fbo2DTextureId;

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
//     float y0 = yuyv.x - 0.0625;
//     float u = yuyv.y - 0.5;
//     float y1 = yuyv.z - 0.0625;
//     float v = yuyv.w - 0.5;
//     float r = y0 + 1.13983 * v;
//     float g = y0 - 0.39465 * u - 0.58060 * v;
//     float b = y0 + 2.03211 * u;
//     float r1 = y1 + 1.13983 * v;
//     float g1 = y1 - 0.39465 * u - 0.58060 * v;
//     float b1 = y1 + 2.03211 * u;
//     vec3 rgb = vec3(r, g, b);
//     vec3 rgb1 = vec3(r1, g1, b1);
//     return (rgb + rgb1) * 0.5;
// }

//// 新方法：效果看不出条纹
//vec3 YUV2RGB4(vec2 texCoord)
//{
//    const vec3 offset = vec3(-0.0625, -0.5, -0.5);
//    const mat3 converter = mat3(1.164, 1.164, 1.164,   0.000,-0.391, 2.018,   1.596,-0.813, 0.000);
//    highp vec4 yuyv = texture2D(tex_yuyv, texCoord);
//    highp float tx = texCoord.x * frameWH.x;
//    float odd = floor(mod(tx, 2.0));
//    float y = odd * yuyv.z + (1.0 - odd) * yuyv.x;

//    vec3 yuv = vec3(y, yuyv.y, yuyv.w);
//    vec3 rgb = converter * (yuv + offset);
//    return rgb;
//}

void main(void)
{
    vec4 color = texture2D(m_Fbo2DTextureId, textureOut);
    gl_FragColor = vec4(color.rgb, 1.0);
}
