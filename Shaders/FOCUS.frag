// #version 330 core
//precision lowp float;
varying vec2 textureOut;
uniform sampler2D tex_yuyv;

uniform vec3 crgb; // 线条颜色
uniform float sensitivity; // 灵敏度
uniform float highThreshold; // 高阈值
uniform vec2 frameWH; // 帧宽高

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

vec2 getGradientMagnitudeDirection(vec2 texCoord) 
{
    lowp float center = getY(texCoord);
    lowp float right = getY(texCoord + vec2(1.0 / frameWH.x, 0));
    lowp float bottom = getY(texCoord + vec2(0, 1.0 / frameWH.y));
    float dx = right - center;
    float dy = bottom - center;
    return vec2(sqrt(dx * dx + dy * dy), atan(dy, dx));
}

float nonMaximumSuppression(vec2 texCoord, float magnitude, float direction) 
{
    lowp vec2 offset = vec2(sin(direction), cos(direction));
    float neighbor1 = getGradientMagnitudeDirection(texCoord + offset).x;
    float neighbor2 = getGradientMagnitudeDirection(texCoord - offset).x;
    if (magnitude >= neighbor1 && magnitude >= neighbor2)
    {
        return magnitude;
    }
    return 0.0;
}


float applyDoubleThreshold(float magnitude, float lowTh, float highTh) 
{
    return (magnitude >= highTh) ? 1.0 : (magnitude >= lowTh) ? 0.5: 0.0;
}

vec2 hysteresis(vec2 texCoord, float lowTh, float highTh) 
{
    vec2 magnitude_direction = getGradientMagnitudeDirection(texCoord);
    float suppressed = nonMaximumSuppression(texCoord, magnitude_direction.x, magnitude_direction.y);
    float thresholded = applyDoubleThreshold(suppressed, lowTh, highTh);
    if (thresholded == 1.0)
    {
        return vec2(1.0, magnitude_direction.x);
    }
    else if (thresholded == 0.5)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                lowp vec2 offset = vec2(i, j) / frameWH;
                float neighbor = getGradientMagnitudeDirection(texCoord + offset).x;
                if (applyDoubleThreshold(neighbor, lowTh, highTh) == 1.0)
                {
                    return vec2(1.0, magnitude_direction.x);
                }
            }
        }
    }
    return vec2(0.0, magnitude_direction.x);
}

vec3 newauxfocus(vec2 texCoord, float sensitivity, float highThreshold, vec3 inColor, vec3 linecolor) 
{
    lowp vec4 origin = vec4(inColor, 1.0);
    lowp vec2 gradient_direction = getGradientMagnitudeDirection(texCoord);
    lowp vec2 edge_magnitude = hysteresis(texCoord, 0.02, highThreshold * 0.3);
    float canny_sobel = (edge_magnitude.x * 0.8 + gradient_direction.x * 4.0 * 0.2);
    lowp vec4 result = mix(origin, vec4(linecolor, 1.0), canny_sobel > (sensitivity * 0.2 + 0.8) ? 1.0 : 0.0); // edge_magnitude.y>0.01?1.0:0.0
    return result.rgb;
}

void main(void)
{
    vec3 incolor = YUV2RGB4(textureOut);
    vec3 lcrgb = crgb.rgb;
    float ST = sensitivity;
    float HT = highThreshold;
    vec3 Color = newauxfocus(textureOut, ST, HT, incolor, lcrgb);
    gl_FragColor = vec4(Color, 1.0);
}
