#include "building_layer.h"

BuildingLayer::BuildingLayer()
{
	_numVertices = 0;
}

void BuildingLayer::build(vector<float>* p, vector<float>* n, vector<int>* t)
{
	va = 0;
	buf = 0;
	gen_vertex_arrays(1);
	gen_buffers(4);

	_numVertices = p->size() / 3;

	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
    glBufferData(GL_ARRAY_BUFFER, _numVertices * 3 * sizeof(float), &p->front(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
    glBufferData(GL_ARRAY_BUFFER, _numVertices * 3 * sizeof(float), &n->front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
    glBufferData(GL_ARRAY_BUFFER, _numVertices * 2 * sizeof(int), &t->front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_INT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]); // xyzt
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);

	numElements = 0;
	uploadedElements = false;
}

void BuildingLayer::draw()
{
	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numVertices);
	glBindVertexArray(0);
}
void BuildingLayer::addInstance(GsVec pos, float t){
	if (numElements >= 64000) return;
	elements[numElements++] = pos.x;
	elements[numElements++] = pos.y;
	elements[numElements++] = pos.z;
	elements[numElements++] = t;
}
void BuildingLayer::drawInstanced()
{
	if (numElements == 0) return;
	
	//float* entries = new float[elements.size()/100];

	/*
	int i;
	for (list<float>::iterator it = elements.begin(); it != elements.end(); ++it){
		entries[i++] = *it;
	}
	*/

	int num = numElements / 4;
	
	glBindVertexArray(va[0]);

	if (uploadedElements == false){
		glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
		glBufferData(GL_ARRAY_BUFFER, num * 4 * sizeof(float), elements, GL_DYNAMIC_DRAW); // x,y,z,t
		uploadedElements = true;
	}

	glDrawArraysInstanced(GL_TRIANGLES, 0, _numVertices, num);

	glBindVertexArray(0);
	
	//delete entries;

	
	

}
void BuildingLayer::clearInstances(){
	numElements = 0;
	uploadedElements = false;
}
int BuildingLayer::getPolygonCount(){
	return _numVertices / 3;
}