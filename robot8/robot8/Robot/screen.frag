#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform int picturetype;
const float offset = 1.0 / 300;  
//銳利
float kernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);


//模糊
float kernel2[9] = float[](
1.0 / 16, 2.0 / 16, 1.0 / 16,
2.0 / 16, 4.0 / 16, 2.0 / 16,
1.0 / 16, 2.0 / 16, 1.0 / 16  
);


//描邊
float kernel3[9] = float[](
    1, 1, 1,
    1,-8, 1,
    1, 1, 1
);

void main()
{
    if(picturetype==0)
        color = texture(screenTexture, TexCoords);//一般
    else if(picturetype==1)
        color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);//黑白 這個比較像負片
    else if(picturetype==2){
    //也是黑白
    color = texture(screenTexture, TexCoords);
    float average = (color.r + color.g + color.b) / 3.0;
    color = vec4(average, average, average, 1.0);
    }
    else if(picturetype==3||picturetype==4||picturetype==5){

    //也是黑白 這個更好
    /*color = texture(screenTexture, TexCoords);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average, average, average, 1.0);*/
   
    //銳利 or 模糊 or 描邊
    vec2 offsets[9] = vec2[](
    vec2(-offset, offset),  // top-left
    vec2(0.0f,    offset),  // top-center
    vec2(offset,  offset),  // top-right
    vec2(-offset, 0.0f),    // center-left
    vec2(0.0f,    0.0f),    // center-center
    vec2(offset,  0.0f),    // center-right
    vec2(-offset, -offset), // bottom-left
    vec2(0.0f,    -offset), // bottom-center
    vec2(offset,  -offset)  // bottom-right
    );
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col=vec3(0.f);
    if(picturetype==3){
    //銳利
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    color = vec4(col, 1.0);
    }
    else if(picturetype==4){
    //模糊
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel2[i];
    color = vec4(col, 1.0);
    }
    else if(picturetype==5){
    //描邊
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel3[i];

    color = vec4(col, 1.0);
    }
   
    
    }
    else if(picturetype==6)
    {
        float m_count=100;
        vec2 uv = TexCoords;
        uv = floor(uv * m_count) / m_count;
        color = texture(screenTexture, uv);
    }

}