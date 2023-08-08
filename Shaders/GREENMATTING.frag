// #version 330 core 
//precision lowp float;
varying vec2 textureOut; 
uniform sampler2D tex_yuyv;
uniform sampler2D tex_bg;
uniform vec2 frameWH;

int greenMethod = 1;

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

vec3 greenMat1(vec2 tex)
{
    vec3 bg = texture2D(tex_bg, textureOut).rgb;
	vec3 fg = YUV2RGB4(textureOut);
    float maxrb = max( fg.r, fg.b );
    float k = clamp( (fg.g-maxrb)*5.0, 0.0, 1.0 );
    float ll = length( fg );
    fg.g = min(fg.g, maxrb*0.8);
    fg = ll*normalize(fg);
    return mix(fg, bg, k);
}

vec3 greenMat2(vec2 tex)
{
    vec3 bg = texture2D(tex_bg, textureOut).rgb;
	vec3 fg = YUV2RGB4(textureOut);
    float maxrb = max( fg.r, fg.b );
    float k = clamp( (fg.g-maxrb)*5.0, 0.0, 1.0 );
    float dg = fg.g; 
    fg.g = min(fg.g, maxrb*0.8); 
    fg.g += dg - fg.g;
    return mix(fg, bg, k);
}

vec3 greenMat3(vec2 tex)
{
    vec3 bg = texture2D(tex_bg, textureOut).rgb;
	vec3 fg = YUV2RGB4(textureOut);
    float AVGrb = (fg.r + fg.b) * 0.5;
    float alpha = (fg.g - AVGrb); // [-1,1]
    alpha = (alpha + 1.0) * 0.5; // [0,1]
    alpha = pow(alpha, 2.0);
    alpha = clamp(alpha, 0.0, 1.0);
    return mix(fg, bg, alpha);
}

void main(void)
{
    vec3 result;
    if(greenMethod == 0)
    {
        result = greenMat1(textureOut);
    }
    else if(greenMethod == 1)
    {
        result = greenMat2(textureOut);
    }
    else if(greenMethod == 2)
    {
        result = greenMat3(textureOut);
    }
    gl_FragColor = vec4(result, 1.0);
}
