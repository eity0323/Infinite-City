# version 400

uniform sampler2D Texture0;

uniform float fogStart;
uniform float fogDist;

in vec4 localPos;
in vec4 globalPos;
in vec2 uvs;
in vec3 norm;
in float _t;

layout(location = 0) out vec4 fColor;
layout(location = 1) out vec4 fBloom;
layout(location = 2) out vec4 fPos;
layout(location = 3) out vec4 fNorm;

void main() { 
	float t = _t;

	vec4 color = texture2D(Texture0, uvs);
	
	float depth = (length(localPos.xyz) - fogStart) / fogDist;

	depth = clamp(depth, 0, 1);
	
	color.rgb *= clamp((sin(-t + globalPos.z/20.) - .975)*28., 0., 1.) + 1.;

	color.rgb *= 1.1 + sin(-t*2. + globalPos.z/3. + globalPos.y/3. - globalPos.x/3.)*.15;

	color.b += (sin(t*6. + globalPos.y/3. - globalPos.x/3.) + .5)*mod(globalPos.z, 1.)*0.02;
	color.g += (sin(t*4. + 1.4 + globalPos.y/3. - globalPos.x/3.) + .5)*mod(globalPos.x, 1.)*0.02;
	color.r += (cos(t*5. + globalPos.y/3. - globalPos.x/3.) + .5)*mod(globalPos.y, 1.)*0.02;

	fColor = vec4(color.xyz, color.w)*(1-depth);
	
	vec3 bl = vec3(fColor.x, fColor.y, fColor.z);
	bl -= .05;
	bl *= (.5 + sin(t*1.5)*sin(t*1.5)*2.5);

	fBloom = vec4(bl.x, bl.y, bl.z, color.w*(1-depth));

	fPos = globalPos;
	fNorm = vec4(norm.xyz, 1.0);
} 
