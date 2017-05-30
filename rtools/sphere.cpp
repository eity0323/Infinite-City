#include "sphere.h"

const float  PI_F = 3.14159265358979f;

void PrimitiveSphere::gen(float radius, int nfaces){
	numVertices = nfaces*nfaces;
	
	va = 0;
	buf = 0;
	gen_vertex_arrays(1); // sphere's vba
	gen_buffers(4); // x,y,z; nx,ny,nz; ???, ???

	float* _p = new float[numVertices*3]; // positions
	float* p = _p;

	float* _n = new float[numVertices*3]; // normals
	float* n = _n;
	
	for(int i=0;i<nfaces;i++){
        float theta = (i/(float)nfaces)*(float)PI_F*2.0f;
        for(int h=0;h<nfaces;h++){
            float phi = (h/(float)(nfaces-1))*(float)PI_F + PI_F/2.0f;
            
            float x = sin(theta)*radius;
            float z = cos(theta)*radius;

            float y = sin(phi)*radius;

            x *= cos(phi);
            z *= cos(phi);
                
            *(p++) = x;
			*(p++) = y;
			*(p++) = z;

			float nx = sin(theta);
            float nz = cos(theta);

            float ny = sin(phi);

			*(n++) = nx;
			*(n++) = ny;
			*(n++) = nz;
        }
    }

	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(float), _p, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(float), _n, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _n, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _n, GL_STATIC_DRAW);

	delete _p;
	delete _n;

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	
	numIndices = nfaces*nfaces*6;

	indices = new int[numIndices];
	int *_indices = indices;
	for(int h=0;h<nfaces;h++){
        for(int i=0;i<nfaces;i++){
            int i_plus = i + 1;
			if(i_plus >= nfaces) i_plus = 0;
            
            int h_plus = h + 1;
            if(h_plus >= nfaces) h_plus = 0;

			*(_indices++) = i + h*nfaces;
			*(_indices++) = i + h_plus*nfaces;
			*(_indices++) = i_plus + h_plus*nfaces;

			
			*(_indices++) = i_plus + h*nfaces;
			*(_indices++) = i + h*nfaces;

			if(i_plus + h_plus*nfaces >= nfaces*nfaces){
				*(_indices++) = 0;
			}else{
				*(_indices++) = i_plus + h_plus*nfaces;
			}
        }
    }

}
void PrimitiveSphere::draw(){
	glBindVertexArray(va[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

	//glPointSize(3);

	//glDrawArrays(GL_POINTS, 0, numVertices);

	glBindVertexArray(0);
}