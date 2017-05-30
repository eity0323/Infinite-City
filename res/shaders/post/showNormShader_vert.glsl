# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vUVs;

out vec2 uvs;

void main (){
	vec4 p = vec4 ( vPos.x, vPos.y, vPos.z, 1.0 );

	uvs = vUVs;

	gl_Position = p;
}
