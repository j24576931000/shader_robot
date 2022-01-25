// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
#ifdef GL_ES
precision mediump float;
#endif
uniform float u_time;
vec2 u_resolution=vec2(800,600);

struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
  sampler2D tex0;
};

uniform MaterialInfo Material;
out vec4 vFragColor;

//lighting color
vec4    ambientColor = vec4(0.1,0.1,0.1,1);
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1,1,1,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
//in vec3 Position;
in vec2 UV;
float Shininess = 30.0;//for material specular

float random2d(vec2 coord){
  return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
	

uniform int background_num;   
uniform bool light;  
uniform int control;
uniform vec3 cameraPos;
uniform samplerCube skybox;

//uniform  sampler2D tex0;
void main(void)
{ 
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(vVaryingNormal),
					normalize(vVaryingLightDir)));

    // Multiply intensity by diffuse color, force alpha to 1.0
    vFragColor = diff * diffuseColor*vec4(Material.Kd,1);

    // Add in ambient light
    vFragColor += ambientColor;


    // Specular Light
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
								normalize(vVaryingNormal)));//反射角
    float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));

  //雜訊
  vec2 coord = gl_FragCoord.xy/u_resolution ;
  vec3 color = vec3(0.0);
  float grain =0.0;
  grain = random2d(vec2 (sin(coord)/999999.9)*u_time);     
  color=vec3(grain );

  //
  float m_count=10;
  vec3 uv2 = vec3 (UV,1.0);

  vec3 uv3 = floor(uv2 * m_count) / m_count;
  //color = texture( myTextureSampler, uv2 )* particlecolor;
  
  vec4 sample0,sample1,sample2,sample3;
	float fstep=0.15;
	sample0=texture(Material.tex0,vec2(UV.x-fstep,UV.y-fstep));
	sample1=texture(Material.tex0,vec2(UV.x+fstep,UV.y-fstep));
	sample2=texture(Material.tex0,vec2(UV.x+fstep,UV.y+fstep));
	sample3=texture(Material.tex0,vec2(UV.x-fstep,UV.y+fstep));
	vec4 color2=(sample0+sample1+sample2+sample3) / 4.0;
	

    if(diff != 0) {
		spec = pow(spec, Shininess);
    //if(background_num==0)
		  //vFragColor.rgba+= specularColor*vec4(1.0-Material.Kd.x,1.0-Material.Kd.y,1.0-Material.Kd.z,1).rgba*spec;
    //else if(background_num==1 || background_num==2){
     // if(light==false)
        //vFragColor.rgba+= texture(Material.tex0,UV);
      //else
        //vFragColor.rgba+= texture(Material.tex0,UV)*specularColor*vec4(Material.Ks.x,Material.Ks.y,Material.Ks.z,1).rgba*spec;
    //}
    if(control==6)
        vFragColor.rgba+= vec4(color,1.0);
    else if(control==7){
      
      vec3 I = normalize(vVaryingLightDir-cameraPos);
      vec3 R = reflect(I, normalize(vVaryingNormal));
      vFragColor.rgba+=texture(skybox, R);
    }
    else if(control==8){     
      float ratio = 1.00 / 1.52;
      vec3 I = normalize(vVaryingLightDir - cameraPos);
      vec3 R = refract(I, normalize(vVaryingNormal), ratio);
      //vec3 I = normalize(vVaryingLightDir-cameraPos);
      //vec3 R = reflect(I, normalize(vVaryingNormal));
      vFragColor.rgba+=texture(skybox, R);
    }
    else
      //vFragColor.rgba+= texture(Material.tex0,UV);
      //vFragColor=color2*vec4(1.0-Material.Kd.x,1.0-Material.Kd.y,1.0-Material.Kd.z,1).rgba*spec;
      //vFragColor.rgba+= specularColor*vec4(1.0-Material.Kd.x,1.0-Material.Kd.y,1.0-Material.Kd.z,1).rgba*spec;
      vFragColor+= specularColor*vec4(Material.Ks.x,Material.Ks.y,Material.Ks.z,1)*spec;
    
    
		//vFragColor.rgba+= texture(Material.tex0,UV)*specularColor*vec4(Material.Ks.x,Material.Ks.y,Material.Ks.z,1).rgba*spec;
    
    
    }
}
	
    