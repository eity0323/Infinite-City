# version 400

uniform sampler2D Texture0;
uniform float t;

in vec4 localPos;
in vec4 globalPos;
in vec2 uvs;

out vec4 fColor;

void main() { 
	vec4 color = texture2D(Texture0, uvs);
	
	float depth = length(localPos.xyz) / 100.;

	depth = clamp(depth - .5, 0, 1);

	color.rgb *= clamp((sin(-t + globalPos.z/20.) - .975)*28., 0., 1.) + 1.;

	fColor = vec4(color.xyz*(1-depth), color.w);
} 
