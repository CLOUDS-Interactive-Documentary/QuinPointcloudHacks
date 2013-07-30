#version 110
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable


uniform sampler2DRect texture;

varying float positionValid;
varying vec4 faceFeatureSample;
varying vec4 deltaChangeSample;

//QUINS ADDITIONS
uniform int enableFlags;
//FOR 3D PRINTER
uniform vec2 printhead;
uniform float printheadWidth;
uniform vec4 colorRect;
//FOR SCATTER SHOT
uniform float smoothAudioAmp;
uniform float audioAmp;
uniform float randSeed;
uniform float threshold;

//LIGHTING
varying vec3 eye;
varying vec3 normal;
varying float diffuseAttenuate;
varying vec3 diffuseLightDirection;

uniform float eyeMultiplier;
uniform float skinMultiplier;
uniform float baseMultiplier;

const float epsilon = 1e-6;

float rand(vec2 n)
{
    return 0.5 + 0.5 *
    fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

float calculateLight(){
	vec3 N = normal;
	vec3 L = diffuseLightDirection;
	
	float lambertTerm = dot(N,L) * diffuseAttenuate;
	return lambertTerm;
}

float isEye(){
	return min(min(faceFeatureSample.r, faceFeatureSample.g), faceFeatureSample.b);
}

float isFace(){
	return min(faceFeatureSample.r, faceFeatureSample.g);
}

float isSkin(){
	return min(faceFeatureSample.r + faceFeatureSample.g, 1.0);
}

void main(){
    if ((enableFlags & 1) > 0){//if Printing
        //throw away anything not covered by the printhead yet
        float texCoordT = colorRect.w+colorRect.y-(printhead.y*colorRect.w);
        float texCoordS = colorRect.z+colorRect.x-(printhead.x*colorRect.z);
        float texCoordW = colorRect.w*printheadWidth;
        if (gl_TexCoord[0].t < texCoordT){
            if (gl_TexCoord[0].t < texCoordT - texCoordW){
                discard;
                return;
            } else {
                if (gl_TexCoord[0].s < texCoordS){
                    discard;
                    return;
                }
            }
        }
    }
    
    if(positionValid < epsilon){
    	discard;
        return;
    }

	float attenuate = 1.0 + max(isEye() * eyeMultiplier, isSkin() * skinMultiplier);
	if(attenuate < epsilon){
		discard;
		return;
	}
	
    vec4 col = texture2DRect(texture, gl_TexCoord[0].st);
//    float p = 1 - audioAmplitude; //rand(gl_TextCoord) - audioAmplitude;
//    if (p > threshold){
//        col += rand(gl_TexCoord[0] + gl_TexCoord[1], p);
//    }

//    float p = rand(vec2(randSeed, gl_TexCoord[0].s));
//    p = p*p;
//    p = p*p;
//    p = p*p;
//    p = p*p;
//    p = p*p;//*p*p*p*p*p*p*p*p*p*p*p*p*p*p*p*p*p*p;
////    p = p*p*p;
//    if (p > smoothAudioAmp){
//        float v = rand(vec2(randSeed, gl_TexCoord[0].t));
//        if (v < .5){
//            v = 0.0;
//        } else {
//            v = 1.0;
//        }
//        gl_FragColor.x = v;//vec3(v,v,v)//rand(vec2(gl_TexCoord[0].s, audioAmp));
//        gl_FragColor.y = v;
//        gl_FragColor.z = v;
//    }
    
    if ((enableFlags & 1) > 0){//if Printing
        float v = col.r*.59+col.b*.11+col.g*.29;
        v = pow(v*1.5, 2.);
        col.r = v;
        col.g = v;
        col.b = v;
    }
    gl_FragColor = gl_Color * col * attenuate * max( calculateLight(), isSkin() );
	//gl_FragColor = vec4(normal,1.0);
}

