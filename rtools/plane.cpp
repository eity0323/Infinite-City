#include "plane.h"

void PrimitivePlane::gen(float width, float height, int nfaces){
	numVertices = (nfaces + 1)*(nfaces + 1);
	
	va = 0;
	buf = 0;
	gen_vertex_arrays(1); // sphere's vba
	gen_buffers(3); // x,y,z; nx,ny,nz; uvx, uvy;

	float* _p = new float[numVertices*3]; // positions
	float* p = _p;

	float* _n = new float[numVertices*3]; // normals
	float* n = _n;
	
	float* _uvs = new float[numVertices*2]; // uvs
	float* uvs = _uvs;

	for(int i=0;i<nfaces + 1;i++){
		float x = width * (i/nfaces);
        for(int h=0;h<nfaces + 1;h++){
            float y = height * (h/nfaces);

            float z = 0;
                
            *(p++) = x;
			*(p++) = y;
			*(p++) = z;

			float nx = 0;
            float ny = 0;
			float nz = 1;

			*(n++) = nx;
			*(n++) = ny;
			*(n++) = nz;

			float uvx = x;
			float uvy = y;

			*(uvs++) = uvx;
			*(uvs++) = uvy;
        }
    }

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
	
	numIndices = nfaces*nfaces*6;

	indices = new int[numIndices];
	int *_indices = indices;
	for(int h=0;h<nfaces;h++){
        for(int i=0;i<nfaces;i++){
            int i_plus = i + 1;
            
            int h_plus = h + 1;

			*(_indices++) = i + h*(nfaces + 1);
			*(_indices++) = i + h_plus*(nfaces + 1);
			*(_indices++) = i_plus + h_plus*(nfaces + 1);

			*(_indices++) = i + h*(nfaces + 1);
			*(_indices++) = i_plus + h*(nfaces + 1);
			*(_indices++) = i_plus + h_plus*(nfaces + 1);
        }
    }

}
void PrimitivePlane::draw(){
	glBindVertexArray(va[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

	//glPointSize(3);

	//glDrawArrays(GL_POINTS, 0, numVertices);

	glBindVertexArray(0);
}