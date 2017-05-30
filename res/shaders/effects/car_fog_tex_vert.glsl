# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vUVs;
layout (location = 3) in vec4 vals; // xyzt
layout (location = 4) in vec4 rotvals; // rx, ry, tilt, ???

uniform mat4 vTransf;
uniform mat4 vCam;
uniform mat4 vProj;
uniform float t;

out vec2 uvs;
out vec4 localPos;
out vec4 globalPos;
out vec3 norm;
out float _t;

void main (){
	float rx = rotvals.x;
	float ry = rotvals.y;

	mat4 rot_z = mat4(
	cos(rx), sin(rx), 0, 0,
	-sin(rx), cos(rx), 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	);
	
	mat4 rot_x = mat4(
	1, 0, 0, 0,
	0, cos(ry), -sin(ry), 0,
	0, sin(ry), cos(ry), 0,
	0, 0, 0, 1
	);
	
	/*
	mat4 rot_x = mat4(
	cos(ry), 0, -sin(ry), 0,
	0, 1, 0, 0,
	sin(ry), 0, cos(ry), 0,
	0, 0, 0, 1
	);
	*/
	vec4 p = rot_z * rot_x * vTransf * vec4 ( vPos.xyz, 1.0 );
	p.xyz += vals.xyz;

	_t = t + vals.w;
	
	globalPos = p;
	p = vCam * p;
	localPos = p;

	/*
	p = vTransf * vec4 ( vPos.xyz + vals.xyz, 1.0 );
	p.z *= pow(clamp((1.0-length(localPos.xz) / 360.0) + .9, 0, 1), 2);
	p = vCam * p;
	*/

	uvs = vUVs;
	norm = vNorm;

	gl_Position = vProj * p;
}
