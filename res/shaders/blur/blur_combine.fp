# version 400

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

uniform float sh_add;
uniform float sh_mult;

in vec2 uvs;

out vec4 fColor;

void main() { 
	vec4 color = (texture2D(Texture0, uvs) + texture2D(Texture1, uvs) + texture2D(Texture2, uvs))/3.;

	color.xyz += sh_add;
	color.xyz *= sh_mult;

	fColor = color;
} 
