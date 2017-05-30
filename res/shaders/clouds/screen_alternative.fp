#version 400

#define DRAW_DISTANCE 256

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
	
	float destAlpha = .2;
	
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
	
	// --- START ---

	vec3 ro = vec3(ox, oy, oz);

	int d = 1;

	float dimLength = yd1;
	
	float ll = 1.f / abs(dimLength);
	vec3 rd = vec3(xd1 * ll, yd1 * ll, zd1 * ll);
	
	/*
	float initial = oy - int(oy);

	if (dimLength > 0)
		initial = 1 - initial;
			
	float dist = ll * initial;
	*/

	vec3 pos = floor(ro);
	vec3 ri = 1.0/rd;
	vec3 rs = sign(rd);
	vec3 dis = (pos-ro + 0.5 + rs*0.5) * ri;
	
	float res = -1.0;
	vec3 mm = vec3(0.0);
	for(int i=0; i<DRAW_DISTANCE; i++) {
		int tex = int(texture(maptex, pos / mapSize).r * 255.);
		if(tex > 0){
			
			float depth = length(pos - ro);

			if(depth > depthAt) break;
			
			vec3 toColor = vec3(.5, .7, .9);

			float distVal = (1.0-depth/m_far);

			distVal = clamp(distVal + .5, 0, 1);
						
			alpha += destAlpha*distVal;
			color += toColor*destAlpha*distVal;
		}
		mm = step(dis.xyz, dis.yxy) * step(dis.xyz, dis.zzx);
		dis += mm * rs * ri;
        pos += mm * rs;
	}
	// --- END ---
	
	
	// apply distance + fog effect
	color += (1-alpha)*vec3(1, 1, 1);

	gl_FragColor.x = color.r;
    gl_FragColor.y = color.g;
    gl_FragColor.z = color.b;
    gl_FragColor.w = alpha;
    
}
