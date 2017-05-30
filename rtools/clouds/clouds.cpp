#include "clouds.h"

const float  PI_F = 3.14159265358979f;
const float  PI = 3.14159265358979;
#define random() (((double) rand() / (RAND_MAX)))

void CubeClouds::initMap(){
	mapWidth = mapDepth = 256;
	mapHeight = 64;
		
	map = new uint8_t[mapWidth*mapHeight*mapDepth];

	int i=0;
	for (int z = 0; z < mapDepth; z++) {
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				double dbottom = abs((double)y/(double)mapHeight -.5);
				
				map[i] = 0;

				if(dbottom < .5){
					if(random() > .9998){
						if(random()*.5 > dbottom){
							map[i] = (uint8_t) (random() * 3 + 1);
						}
					}
				}
				i++;
			}
		}
	}
	
	
	for(int num=0;num<6;num++){
		double xp = random()*mapWidth;
		double yp = random()*mapHeight*.9 + mapHeight*.1;
		double zp = random()*mapDepth;
			
		double xrot = random()*PI*2.0 / 20.;
		double yrot = random()*PI*2.0;
			
		double xrotd = (random() * 2. - 1.0)*PI*2.0/500.0;
		double yrotd = (random() * 2. - 1.0)*PI*2.0/1000;
			
		double length = random()*random()*10.0 + 3.0;
			
		double radius = random()*10.0 + 1.;
		
		for(double pos = 0;pos < length;pos+=1.){
				
			xrotd *= .9;
			yrotd *= .8;
				
			xrotd += (random() * 2.0 - 1.0)*PI*2.0/10.0;
			yrotd += (random() * 2.0 - 1.0)*PI*2.0/500.0;
				
			xp += cos(xrot)*radius/2.0;
			zp += sin(xrot)*radius/2.0;
				
			yp += sin(yrot)*radius/20.0;
				
			xrot += xrotd;
			yrot += yrotd;
				
			radius += random()*4.0 - 3.5;
			if(radius < 0.0 || radius > 40.0) radius = random()*10.0 + 1.;
				
			double wr = random()*1.0;
			double hr = random()*.2;
			double dr = random()*1.0;
			
			for(int x = (int)(xp - radius*wr);x < xp + radius;x++){
				for(int y = (int)(yp - radius*hr);y < yp + radius;y++){
					for(int z = (int)(zp - radius*dr);z < zp + radius;z++){
							
						//if(Math.sqrt((x-xp)*(x-xp) + (y-yp)*(y-yp) + (z-zp)*(z-zp)) < radius + Math.random()*2. - 1.){
							uint8_t fill =  (uint8_t)(1.0 + (sin(num/10.0)+.5)*15.0);
								
							if(sqrt((x-xp)*(x-xp)/wr + (y-yp)*(y-yp)/hr + (z-zp)*(z-zp)/dr) > random()*radius*1.2){
								fill = (uint8_t)0;
							}else{
								if(random() > .5){
									fill += (uint8_t)1; 
									if(random() > .5){
										fill += (uint8_t)1; 
									}
								}
							}

							if(x >=0 && x<mapWidth && y>=0 && y< mapHeight && z>=0 && z<mapDepth){
								int i = z*mapWidth*mapHeight + y*mapWidth + x;
								if (i >= mapWidth*mapHeight*mapDepth || i < 0){
									continue;
								}

								if(map[i] == 0){
									map[i] = fill;
								}
							}
						//}
					}
				}
			}
		}
	}
	for(int num=0;num<3;num++){
		double xp = random()*mapWidth;
		double yp = random()*mapHeight/2.0;
		double zp = random()*mapDepth;
			
		double xrot = random()*PI*2.0 / 20.;
		double yrot = random()*PI*2.0;
			
		double xrotd = (random() * 2. -1.0)*PI*2.0/500.0;
		double yrotd = (random() * 2. -1.0)*PI*2.0/1000.0;
			
		double length = random()*random()*10.0 + 5.;
			
		double radius = random()*15.0 + 1.;
			
		for(double pos = 0;pos < length;pos+=1.){
				
			xrotd *= .9;
			yrotd *= .8;
				
			xrotd += (random() * 2. - 1.0)*PI*2.0/10.0;
			yrotd += (random() * 2. - 1.0)*PI*2.0/500.0;
				
			xp += cos(xrot)*radius/2.0;
			zp += sin(xrot)*radius/2.0;
				
			yp += sin(yrot)*radius/20.0;
				
			xrot += xrotd;
			yrot += yrotd;
				
			radius += random()*7.0 - 3.5;
			if(radius < 0.0 || radius > 30.0) radius = random()*15.0 + 1.;
				
			double wr = random()*2.0;
			double hr = random()*.3;
			double dr = random()*1.0;
			
			for(int x = (int)(xp - radius*wr);x < xp + radius;x++){
				for(int y = (int)(yp - radius*hr);y < yp + radius;y++){
					for(int z = (int)(zp - radius*dr);z < zp + radius;z++){
							
						//if(Math.sqrt((x-xp)*(x-xp) + (y-yp)*(y-yp) + (z-zp)*(z-zp)) < radius + Math.random()*2. - 1.){
							uint8_t fill =  (uint8_t)(1.0 + (sin(num/10.0)+.5)*15.0);
								
							if(sqrt((x-xp)*(x-xp)/wr + (y-yp)*(y-yp)/hr + (z-zp)*(z-zp)/dr) > random()*radius*1.2){
								fill = (uint8_t)0;
							}else{
								if(random() > .5){
									fill += (uint8_t)1; 
									if(random() > .5){
										fill += (uint8_t)1; 
									}
								}
							}
							
							if (x >= 0 && x<mapWidth && y >= 0 && y< mapHeight && z >= 0 && z<mapDepth){
								int i = z*mapWidth*mapHeight + y*mapWidth + x;
								if (i >= mapWidth*mapHeight*mapDepth || i < 0){
									continue;
								}

								if (map[i] == 0){
									map[i] = fill;
								}
							}

						//}
					}
				}
			}
		}
	}

}

void CubeClouds::gen(){
	initMap();

	// initaize stuff for the camera position and draw distance

	xRot = PI / 2;
	yRot = 0;

	ox = mapWidth/2.;
	oy = mapHeight/2.;
	oz = mapDepth/2.;

	ar = 1;
		
	drawDistance = 128;
	// 

	mapTexID = genTextureID();


	glBindTexture(GL_TEXTURE_3D, mapTexID);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, mapWidth, mapHeight, mapDepth, 0, GL_RED, GL_UNSIGNED_BYTE, map);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	cloudShader = GLUtils::compileShader("../res/shaders/clouds/screen.vp", "../res/shaders/clouds/screen.fp");

	shader_xRot = glGetUniformLocationARB(cloudShader->id, "xRot");
	shader_yRot = glGetUniformLocationARB(cloudShader->id, "yRot");
		
	shader_yCos = glGetUniformLocationARB(cloudShader->id, "yCos");
	shader_ySin = glGetUniformLocationARB(cloudShader->id, "ySin");
	shader_xCos = glGetUniformLocationARB(cloudShader->id, "xCos");
	shader_xSin = glGetUniformLocationARB(cloudShader->id, "xSin");
		
	shader_ox = glGetUniformLocationARB(cloudShader->id, "ox");
	shader_oy = glGetUniformLocationARB(cloudShader->id, "oy");
	shader_oz = glGetUniformLocationARB(cloudShader->id, "oz");

	shader_ar = glGetUniformLocationARB(cloudShader->id, "ar");
		
	shader_maptex = glGetUniformLocationARB(cloudShader->id, "maptex");
	shader_depthtex = glGetUniformLocationARB(cloudShader->id, "depthtex");
	
		
	shader_drawDistance = glGetUniformLocationARB(cloudShader->id, "drawDistance");
	shader_m_near = glGetUniformLocationARB(cloudShader->id, "m_near");
	shader_m_far = glGetUniformLocationARB(cloudShader->id, "m_far");

	
		
	shader_mapWidth = glGetUniformLocationARB(cloudShader->id, "mapWidth");
	shader_mapHeight = glGetUniformLocationARB(cloudShader->id, "mapHeight");
	shader_mapDepth = glGetUniformLocationARB(cloudShader->id, "mapDepth");
	
	numVertices = 4;
	
	va = 0;
	buf = 0;
	gen_vertex_arrays(1); // sphere's vba
	gen_buffers(3); // x,y,z; nx,ny,nz, uvx, uvy; 

	float* _p = new float[numVertices*3]; // positions
	float* p = _p;

	float* _n = new float[numVertices*3]; // normals
	float* n = _n;

	float* _uvs = new float[numVertices*2]; // uvs
	float* uvs = _uvs;
	
	*(p++) = -1; *(p++) = -1; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 0;

	*(p++) = 1; *(p++) = -1; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 0;

	*(p++) = 1; *(p++) = 1; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 1;

	*(p++) = -1; *(p++) = 1; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 1;
	//

	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(float), _p, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(float), _n, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*2*sizeof(float), _uvs, GL_STATIC_DRAW);

	delete _p;
	delete _n;
	delete _uvs;

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	
	numIndices = 6;

	indices = new int[numIndices];
	int *_indices = indices;

	*(_indices++) = 0;
	*(_indices++) = 1;
	*(_indices++) = 2;
	*(_indices++) = 0;
	*(_indices++) = 2;
	*(_indices++) = 3;
}
void CubeClouds::draw(int depthTexId, float m_near, float m_far){
	yCos = cos(yRot);
	ySin = sin(yRot);
	xCos = cos(xRot);
	xSin = sin(xRot);

	glUseProgram(cloudShader->id);

	glUniform1fARB(shader_xRot, (float)xRot);
	glUniform1fARB(shader_yRot, (float)yRot);
			
	glUniform1fARB(shader_yCos, (float)yCos);
	glUniform1fARB(shader_ySin, (float)ySin);
	glUniform1fARB(shader_xCos, (float)xCos);
	glUniform1fARB(shader_xSin, (float)xSin);
			
	glUniform1fARB(shader_ox, (float)ox);
	glUniform1fARB(shader_oy, (float)(oy - 4.0));
	glUniform1fARB(shader_oz, (float)oz);

	glUniform1fARB(shader_ar, ar);
			
	glUniform1fARB(shader_drawDistance, (float)drawDistance);

	glUniform1fARB(shader_m_near, m_near);
	glUniform1fARB(shader_m_far, m_far);

	
	glUniform1iARB(shader_mapWidth, mapWidth);
	glUniform1iARB(shader_mapHeight, mapHeight);
	glUniform1iARB(shader_mapDepth, mapDepth);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_3D, mapTexID);
	glUniform1iARB(shader_maptex, 0);
	
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, depthTexId);
	glUniform1iARB(shader_depthtex, 1);
	
	glBindVertexArray(va[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

	glBindVertexArray(0);
}