// #version 330 core 
//precision lowp float;
varying vec2 textureOut; 
uniform sampler2D tex_yuyv;
uniform sampler2D _lutTexture;

uniform vec2 frameWH;
uniform float size_lut;
uniform float qsz_lut;

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

vec3 lut3dMapping(vec3 rgb, float _qsz_lut, float _size_lut)
{
    vec3 new_rgb = clamp(rgb, 0.0, 1.0);
    mediump float _sizeBackDiv = 1.0 / _qsz_lut;
    mediump float _allSizeDiv = 1.0 / (_qsz_lut * _size_lut);
    mediump float blueColor = new_rgb.b * float(_size_lut - 1.0);
    mediump vec2 quad1;
    quad1.y = floor(floor(blueColor) * _sizeBackDiv);
    quad1.x = floor(blueColor) - (quad1.y * _qsz_lut);
    mediump vec2 quad2;
    quad2.y = floor(ceil(blueColor) * _sizeBackDiv);
    quad2.x = ceil(blueColor) - (quad2.y * _qsz_lut);
    mediump vec2 texPos1;
    texPos1.x = (quad1.x * _sizeBackDiv) + 0.5 * _allSizeDiv + ((_sizeBackDiv - _allSizeDiv) * new_rgb.r);
    texPos1.y = (quad1.y * _sizeBackDiv) + 0.5 * _allSizeDiv + ((_sizeBackDiv - _allSizeDiv) * new_rgb.g);
    mediump vec2 texPos2;
    texPos2.x = (quad2.x * _sizeBackDiv) + 0.5 * _allSizeDiv + ((_sizeBackDiv - _allSizeDiv) * new_rgb.r);
    texPos2.y = (quad2.y * _sizeBackDiv) + 0.5 * _allSizeDiv + ((_sizeBackDiv - _allSizeDiv) * new_rgb.g);
    vec4 newColor1 = texture2D(_lutTexture, texPos1);
    vec4 newColor2 = texture2D(_lutTexture, texPos2);
    vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
    return newColor.rgb;
}

void main(void)
{
    vec3 LutColor = lut3dMapping(YUV2RGB4(textureOut), qsz_lut, size_lut);
    gl_FragColor = vec4(LutColor, 1.0);
}
