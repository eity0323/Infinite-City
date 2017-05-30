# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;

uniform mat4 vTransf;
uniform mat4 vCam;
uniform mat4 vProj;
uniform vec4 vColor;

out vec4 color;
out vec4 localPos;
out vec4 globalPos;

void main ()
 {
	vec4 p = vTransf * vec4 ( vPos.x, vPos.y, vPos.z, 1.0 );
	globalPos = p;
	p = vCam * p;
	localPos = p;

	color = vColor;

	gl_Position = vProj * p;
 }
