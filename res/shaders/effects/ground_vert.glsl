# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vUVs;

uniform mat4 vTransf;
uniform mat4 vCam;
uniform mat4 vProj;

out vec2 uvs;
out vec4 localPos;
out vec4 globalPos;
out vec3 norm;

void main (){
	vec4 p = vTransf * vec4 ( vPos.xyz, 1.0 );

	globalPos = p;
	p = vCam * p;
	localPos = p;

	uvs = vUVs;
	norm = vNorm;

	gl_Position = vProj * p;
}
