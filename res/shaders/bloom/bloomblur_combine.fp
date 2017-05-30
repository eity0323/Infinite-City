uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
void main(){
	vec2 c = gl_TexCoord[0].st; 
	
	vec4 combine = (texture2D(tex0, c) + texture2D(tex1, c) + texture2D(tex2, c))/3.;
	
	//combine.xyz += .2;
	//combine.xyz *= .5;
	
    gl_FragColor = combine;
}