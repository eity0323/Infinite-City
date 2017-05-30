#include "cube.h"

const float  PI_F = 3.14159265358979f;

void PrimitiveCube::gen(){
	numVertices = 12*5;
	
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
	
	*(p++) = 0; *(p++) = 0; *(p++) = 1;
	*(uvs++) = 0; *(uvs++) = 0;

	*(p++) = 1; *(p++) = 0; *(p++) = 1;
	*(uvs++) = 1; *(uvs++) = 0;

	*(p++) = 1; *(p++) = 1; *(p++) = 1;
	*(uvs++) = 1; *(uvs++) = 1;

	*(p++) = 0; *(p++) = 1; *(p++) = 1;
	*(uvs++) = 0; *(uvs++) = 1;

	//

	*(p++) = 0; *(p++) = 0; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 1;

	*(p++) = 1; *(p++) = 0; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 1;

	*(p++) = 1; *(p++) = 0; *(p++) = 1;
	*(uvs++) = 1; *(uvs++) = 0;

	*(p++) = 0; *(p++) = 0; *(p++) = 1;
	*(uvs++) = 0; *(uvs++) = 0;
	//
	*(p++) = 0; *(p++) = 1; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 1;

	*(p++) = 1; *(p++) = 1; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 1;

	*(p++) = 1; *(p++) = 1; *(p++) = 1;
	*(uvs++) = 1; *(uvs++) = 0;

	*(p++) = 0; *(p++) = 1; *(p++) = 1;
	*(uvs++) = 0; *(uvs++) = 0;
	//
	*(p++) = 0; *(p++) = 0; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 1;

	*(p++) = 0; *(p++) = 1; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 1;

	*(p++) = 0; *(p++) = 1; *(p++) = 1;
	*(uvs++) = 1; *(uvs++) = 0;

	*(p++) = 0; *(p++) = 0; *(p++) = 1;
	*(uvs++) = 0; *(uvs++) = 0;
	//
	*(p++) = 1; *(p++) = 0; *(p++) = 0;
	*(uvs++) = 0; *(uvs++) = 1;

	*(p++) = 1; *(p++) = 1; *(p++) = 0;
	*(uvs++) = 1; *(uvs++) = 1;

	*(p++) = 1; *(p++) = 1; *(p++) = 1;
	*(uvs++) = 1; *(uvs++) = 0;

	*(p++) = 1; *(p++) = 0; *(p++) = 1;
	*(uvs++) = 0; *(uvs++) = 0;

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
	
	numIndices = 6*5;

	indices = new int[numIndices];
	int *_indices = indices;
	

	for(int i=0;i<5;i++){
		*(_indices++) = 0 + i*4;
		*(_indices++) = 1 + i*4;
		*(_indices++) = 2 + i*4;
		*(_indices++) = 0 + i*4;
		*(_indices++) = 2 + i*4;
		*(_indices++) = 3 + i*4;
	}

}
void PrimitiveCube::draw(){
	glBindVertexArray(va[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

	//glPointSize(3);

	//glDrawArrays(GL_POINTS, 0, numVertices);

	glBindVertexArray(0);
}