#version 400
uniform float xRot;
uniform float yRot;
uniform float yCos;
uniform float ySin;
uniform float xCos;
uniform float xSin;

uniform float ox;
uniform float oy;
uniform float oz;

uniform float ar;

uniform sampler3D maptex;
uniform sampler2D depthtex;

uniform float m_near;
uniform float m_far;

uniform float drawDistance;

uniform int mapWidth;
uniform int mapHeight;
uniform int mapDepth;

in vec2 uvs;
in vec2 screenPos;

void main(){
	float screenX = screenPos.x * ar;
	float screenY = screenPos.y;
	
	vec3 mapSize = vec3(float(mapWidth), float(mapHeight), float(mapDepth));
	
	vec3 color = vec3(0, 0, 0);
	float alpha = 0;
	
	float destAlpha = .15;
	
	float zd2 = 1;

	float zd3 = zd2 * yCos + screenY * ySin;
	float yd1 = screenY * yCos - zd2 * ySin;

	float xd1 = screenX * xCos + zd3 * xSin;
	float zd1 = zd3 * xCos - screenX * xSin;

	float depthAt = texture2D(depthtex, uvs).x;

	depthAt = depthAt*2.0 - 1.0;

	float depthw = -(2. * m_far * m_near) / (m_far - m_near);
	float depthz = -(m_far + m_near) / (m_far - m_near);

	depthAt = depthw / (depthAt + depthz);
	
	vec3 mps = 1. / mapSize;

	// unroll loop?
	for (int d = 0; d < 3; d++) {
		float dimLength = xd1;
		if (d == 1)
			dimLength = yd1;
		if (d == 2)
			dimLength = zd1;

		float ll = 1.f / abs(dimLength);
		vec3 dir = vec3(xd1 * ll, yd1 * ll, zd1 * ll);
		
		float initial = ox - int(ox);
		if (d == 1)
			initial = oy - int(oy);
		if (d == 2)
			initial = oz - int(oz);
		if (dimLength > 0)
			initial = 1 - initial;
			
		float dist = ll * initial;

		vec3 pos = vec3(ox, oy, oz) + dir * initial;
		
		if (dimLength < 0) {
			if (d == 0)
				pos.x -= 1.;
			if (d == 1)
				pos.y -= 1.;
			if (d == 2)
				pos.z -= 1.;
		}
		

		while (dist < drawDistance && dist <= depthAt) {
			if(pos.y < 0){
				pos += dir;
				dist += ll;
				
				continue;

			}
			
			int tex = int(texture(maptex, pos * mps).r * 255.);

			if (tex > 0) {
					
				vec3 toColor = vec3(.5, .7, .9)/3;
					
				float distVal = (1-dist/drawDistance);
						
				alpha += destAlpha*distVal;
				color += toColor*destAlpha*distVal;
						
				//destAlpha *= .8;
						
				//if(alpha >= 1.0) break;
			}

			pos += dir;
			dist += ll;
		}
		
	}
	
	// apply distance + fog effect
	color += (1-alpha)*vec3(1, 1, 1);

	gl_FragColor = vec4(color.r, color.g, color.b, alpha);
    
}
