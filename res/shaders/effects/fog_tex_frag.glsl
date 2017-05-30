# version 400

uniform sampler2D Texture0;

uniform float fogStart;
uniform float fogDist;

in vec4 localPos;
in vec4 globalPos;
in vec2 uvs;
in vec3 norm;

layout(location = 0) out vec4 fColor;
layout(location = 1) out vec4 fBloom;
layout(location = 2) out vec4 fPos;
layout(location = 3) out vec4 fNorm;

void main() { 
	vec4 color = texture2D(Texture0, uvs);
	
	float depth = (length(localPos.xyz) - fogStart) / fogDist;

	depth = clamp(depth, 0, 1);

	fColor = vec4(color.xyz, color.w)*(1-depth);
	fBloom = vec4((fColor.rgb - .8)/4., (1-depth));

	fPos = globalPos;
	fNorm = vec4(norm.xyz, 1.0);
} 
