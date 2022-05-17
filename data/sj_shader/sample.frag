/************************************************************
************************************************************/
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;

uniform float texture_w = 1280;
uniform float texture_h = 720;
uniform float xX = 1.0;

/************************************************************
************************************************************/

/******************************
******************************/
void main(){
	/********************
	********************/
    vec2 pos_0 = gl_TexCoord[0].st;     
	
	vec2 center = vec2(texture_w, texture_h) / 2.0;
	vec2 delta = (pos_0 - center) * xX;
	
	vec2 pos_1 = center + delta;
	
	vec4 color = texture2DRect(texture0, pos_1);
	gl_FragColor = color * gl_Color;
}
