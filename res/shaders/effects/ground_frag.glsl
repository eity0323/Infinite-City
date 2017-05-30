# version 400

uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform float fogStart;
uniform float fogDist;

uniform float t;

uniform int blockSize;
uniform int buildingSize;

in vec4 localPos;
in vec4 globalPos;
in vec2 uvs;
in vec3 norm;

layout(location = 0) out vec4 fColor;
layout(location = 1) out vec4 fBloom;
layout(location = 2) out vec4 fPos;
layout(location = 3) out vec4 fNorm;

void main() { 
	float depth = (length(localPos.xyz) - fogStart) / fogDist;

	depth = clamp(depth, 0, 1);

	vec4 color;

	vec2 bb = vec2(buildingSize, buildingSize);

	vec2 tpos = mod(globalPos.xy + buildingSize/2., blockSize);

	vec2 inside = step(bb, tpos);

	if(inside.x*inside.y > 0){
		color = texture2D(Texture0, globalPos.xy/2.);

		color.rgb *= 1.1 + sin(-t*2. + globalPos.z/3. + globalPos.y/3. - globalPos.x/3.)*.15;

		fColor = vec4(color.xyz, color.w)*(1-depth);

		vec3 bl = fColor.xyz;
		bl -= .05;
		bl *= 1.7;

		fBloom = vec4(bl.x, bl.y, bl.z, color.w*(1-depth));
	}else{
		color = texture2D(Texture1, globalPos.xy/16.);
		color.rgb *= 1.0 + sin(-t*2. + globalPos.z/3. + globalPos.y/3. - globalPos.x/3.)*.15;

		fColor = vec4(color.xyz, color.w)*(1-depth);

		fBloom = vec4(fColor.xyz/4., color.w*(1-depth));
	}
	
	

	fPos = globalPos;
	fNorm = vec4(norm.xyz, 1.0);
} 
