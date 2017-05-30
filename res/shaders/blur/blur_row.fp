# version 400

uniform sampler2D Texture0;

uniform vec2 off;

in vec2 uvs;

out vec4 fColor;

void main() { 
	vec2 c = uvs.st;
	
	vec4 sum = 5. * texture2D(Texture0, c);
	sum += 6. * texture2D(Texture0, c - off);
	sum += 6. * texture2D(Texture0, c + off);
	
	fColor = sum / 16.;
} 