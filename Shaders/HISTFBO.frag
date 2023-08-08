// #version 330 core 
//precision lowp float;
uniform sampler2D tex_yuyv;
varying vec2 textureOut;

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
    // float gray = dot(YUV2RGB(textureOut),vec3(0.3,0.59,0.11));
    gl_FragColor = vec4(YUV2RGB4(textureOut), 1.0);
}
