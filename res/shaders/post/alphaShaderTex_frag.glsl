# version 400

uniform sampler2D Texture0;
uniform vec4 vColor;

in vec2 uvs;

out vec4 fColor;

void main() { 
	vec4 color = texture2D(Texture0, uvs);

	fColor = color*vColor;
} 
