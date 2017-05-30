uniform sampler2D tex;

uniform float offsetx;
uniform float offsety;

void main(){
	vec4 sum;
	vec2 off = vec2(offsetx, offsety);
	vec2 c = gl_TexCoord[0].st;
	
	sum = 5. * texture2D(tex, c);
	sum += 6. * texture2D(tex, c - off);
	sum += 6. * texture2D(tex, c + off);
	
    gl_FragColor = sum / 16.;
}