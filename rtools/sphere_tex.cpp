#include "sphere_tex.h"

const float  PI_F = 3.14159265358979f;

void TexturedSphere::gen(float radius, int nfaces){
	numVertices = (nfaces + 1)*(nfaces + 1);
	
	va = 0;
	buf = 0;
	gen_vertex_arrays(1); // sphere's vba
	gen_buffers(2); // x,y,z; nx,ny,nz; ???, ???

	float* _p = new float[numVertices*3]; // positions
	float* p = _p;

	float* _uv = new float[numVertices*2]; // uvs
	float* uv = _uv;
	
	for(int i=0;i<=nfaces;i++){
        float theta = (i/(float)nfaces)*(float)PI_F*2.0f;
        for(int h=0;h<=nfaces;h++){
            float phi = (h/(float)(nfaces-1))*(float)PI_F + PI_F/2.0f;
            
            float x = sin(theta)*radius;
            float y = cos(theta)*radius;

            float z = sin(phi)*radius;

            x *= cos(phi);
            y *= cos(phi);
                
            *(p++) = x;
			*(p++) = y;
			*(p++) = z;

			float uvx = float(i) / float(nfaces);
			float uvy = float(h) / float(nfaces);
			*(uv++) = uvx;
			*(uv++) = uvy;
        }
    }

	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(float), _p, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*2*sizeof(float), _uv, GL_STATIC_DRAW);

	delete _p;
	delete _uv;

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // uvs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	
	numIndices = nfaces*nfaces*6;

	indices = new int[numIndices];
	int *_indices = indices;
	for(int h=0;h<nfaces;h++){
        for(int i=0;i<nfaces;i++){
			nfaces++;


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

			nfaces--;
        }
    }

}
void TexturedSphere::draw(){
	glBindVertexArray(va[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

	//glPointSize(3);

	//glDrawArrays(GL_POINTS, 0, numVertices);

	glBindVertexArray(0);
}