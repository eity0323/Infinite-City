#include "car.h"

Car::Car()
{
	_numVertices = 0;
}

void Car::build(float unitsize)
{
	va = 0;
	buf = 0;
	gen_vertex_arrays(1);
	gen_buffers(5);

	vector<float> p;
	vector<float> n;
	vector<int> t;

	// up
	p.push_back(0);				p.push_back(0);		p.push_back(0);
	p.push_back(2 * unitsize);	p.push_back(0);		p.push_back(0);
	p.push_back(2 * unitsize);	p.push_back(0);		p.push_back(unitsize);

	p.push_back(0);				p.push_back(0);		p.push_back(0);
	p.push_back(2 * unitsize);	p.push_back(0);		p.push_back(unitsize);
	p.push_back(0);				p.push_back(0);		p.push_back(unitsize);

	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);

	t.push_back(0);	t.push_back(0);
	t.push_back(2);	t.push_back(0);
	t.push_back(2);	t.push_back(1);
	t.push_back(0);	t.push_back(0);
	t.push_back(2);	t.push_back(1);
	t.push_back(0);	t.push_back(1);

	// down
	p.push_back(0);				p.push_back(3 * unitsize);		p.push_back(0);
	p.push_back(2 * unitsize);	p.push_back(3 * unitsize);		p.push_back(unitsize);
	p.push_back(2 * unitsize);	p.push_back(3 * unitsize);		p.push_back(0);

	p.push_back(0);				p.push_back(3 * unitsize);		p.push_back(0);
	p.push_back(0);				p.push_back(3 * unitsize);		p.push_back(unitsize);
	p.push_back(2 * unitsize);	p.push_back(3 * unitsize);		p.push_back(unitsize);

	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);
	n.push_back(0);		n.push_back(-1);	n.push_back(0);

	t.push_back(0);	t.push_back(0);
	t.push_back(2);	t.push_back(1);
	t.push_back(2);	t.push_back(0);
	t.push_back(0);	t.push_back(0);
	t.push_back(0);	t.push_back(1);
	t.push_back(2);	t.push_back(1);

	// left
	p.push_back(0);		p.push_back(0);					p.push_back(0);
	p.push_back(0);		p.push_back(3 * unitsize);		p.push_back(unitsize);
	p.push_back(0);		p.push_back(3 * unitsize);		p.push_back(0);

	p.push_back(0);		p.push_back(0);					p.push_back(0);
	p.push_back(0);		p.push_back(0);					p.push_back(unitsize);
	p.push_back(0);		p.push_back(3 * unitsize);		p.push_back(unitsize);

	n.push_back(-1);	n.push_back(0);		n.push_back(0);
	n.push_back(-1);	n.push_back(0);		n.push_back(0);
	n.push_back(-1);	n.push_back(0);		n.push_back(0);
	n.push_back(-1);	n.push_back(0);		n.push_back(0);
	n.push_back(-1);	n.push_back(0);		n.push_back(0);
	n.push_back(-1);	n.push_back(0);		n.push_back(0);

	t.push_back(0);	t.push_back(0);
	
	t.push_back(3);	t.push_back(1);
	t.push_back(3);	t.push_back(0);
	t.push_back(0);	t.push_back(0);
	
	t.push_back(0);	t.push_back(1);
	t.push_back(3);	t.push_back(1);

	// right
	p.push_back(2 * unitsize);		p.push_back(0);					p.push_back(0);
	p.push_back(2 * unitsize);		p.push_back(3 * unitsize);		p.push_back(0);
	p.push_back(2 * unitsize);		p.push_back(3 * unitsize);		p.push_back(unitsize);

	p.push_back(2 * unitsize);		p.push_back(0);					p.push_back(0);
	p.push_back(2 * unitsize);		p.push_back(3 * unitsize);		p.push_back(unitsize);
	p.push_back(2 * unitsize);		p.push_back(0);					p.push_back(unitsize);

	n.push_back(1);		n.push_back(0);		n.push_back(0);
	n.push_back(1);		n.push_back(0);		n.push_back(0);
	n.push_back(1);		n.push_back(0);		n.push_back(0);
	n.push_back(1);		n.push_back(0);		n.push_back(0);
	n.push_back(1);		n.push_back(0);		n.push_back(0);
	n.push_back(1);		n.push_back(0);		n.push_back(0);

	t.push_back(0);	t.push_back(0);
	t.push_back(3);	t.push_back(0);
	t.push_back(3);	t.push_back(1);
	t.push_back(0);	t.push_back(0);
	t.push_back(3);	t.push_back(1);
	t.push_back(0);	t.push_back(1);

	// bottom
	p.push_back(0);				p.push_back(0);					p.push_back(0);
	p.push_back(2 * unitsize);	p.push_back(3 * unitsize);		p.push_back(0);
	p.push_back(2 * unitsize);	p.push_back(0);					p.push_back(0);

	p.push_back(0);				p.push_back(0);					p.push_back(0);
	p.push_back(0);				p.push_back(3 * unitsize);		p.push_back(0);
	p.push_back(2 * unitsize);	p.push_back(3 * unitsize);		p.push_back(0);

	n.push_back(0);		n.push_back(0);		n.push_back(-1);
	n.push_back(0);		n.push_back(0);		n.push_back(-1);
	n.push_back(0);		n.push_back(0);		n.push_back(-1);
	n.push_back(0);		n.push_back(0);		n.push_back(-1);
	n.push_back(0);		n.push_back(0);		n.push_back(-1);
	n.push_back(0);		n.push_back(0);		n.push_back(-1);

	t.push_back(0);	t.push_back(0);
	t.push_back(2);	t.push_back(3);
	t.push_back(2);	t.push_back(0);
	t.push_back(0);	t.push_back(0);
	t.push_back(0);	t.push_back(3);
	t.push_back(2);	t.push_back(3);

	// top
	p.push_back(0);				p.push_back(0);					p.push_back(unitsize);
	p.push_back(2 * unitsize);	p.push_back(0);					p.push_back(unitsize);
	p.push_back(2 * unitsize);	p.push_back(3 * unitsize);		p.push_back(unitsize);

	p.push_back(0);				p.push_back(0);					p.push_back(unitsize);
	p.push_back(2 * unitsize);	p.push_back(3 * unitsize);		p.push_back(unitsize);
	p.push_back(0);				p.push_back(3 * unitsize);		p.push_back(unitsize);

	n.push_back(0);		n.push_back(0);		n.push_back(1);
	n.push_back(0);		n.push_back(0);		n.push_back(1);
	n.push_back(0);		n.push_back(0);		n.push_back(1);
	n.push_back(0);		n.push_back(0);		n.push_back(1);
	n.push_back(0);		n.push_back(0);		n.push_back(1);
	n.push_back(0);		n.push_back(0);		n.push_back(1);

	t.push_back(0);	t.push_back(0);
	t.push_back(2);	t.push_back(0);
	t.push_back(2);	t.push_back(3);
	t.push_back(0);	t.push_back(0);
	t.push_back(2);	t.push_back(3);
	t.push_back(0);	t.push_back(3);

	_numVertices = p.size() / 3;

	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, _numVertices * 3 * sizeof(float), &p.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, _numVertices * 3 * sizeof(float), &n.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, _numVertices * 2 * sizeof(int), &t.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_INT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[3]); // pos
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, buf[4]); // rot
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);

	i_num = 0;
	i_uploaded = false;
}

void Car::draw()
{
	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numVertices);
	glBindVertexArray(0);
}
void Car::addInstance(GsVec pos, float t, GsVec rot){
	i_pos[i_num * 4 + 0] = pos.x;
	i_pos[i_num * 4 + 1] = pos.y;
	i_pos[i_num * 4 + 2] = pos.z;
	i_pos[i_num * 4 + 3] = t;

	i_rot[i_num * 4 + 0] = rot.x;
	i_rot[i_num * 4 + 1] = rot.y;
	i_rot[i_num * 4 + 2] = rot.z;
	i_rot[i_num * 4 + 3] = 0;

	i_num++;
}
void Car::drawInstances(){
	glBindVertexArray(va[0]);


	if (i_uploaded == false){
		glBindBuffer(GL_ARRAY_BUFFER, buf[3]);
		glBufferData(GL_ARRAY_BUFFER, i_num * 4 * sizeof(float), &i_pos, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buf[4]);
		glBufferData(GL_ARRAY_BUFFER, i_num * 4 * sizeof(float), &i_rot, GL_DYNAMIC_DRAW);

		i_uploaded = true;
	}

	glDrawArraysInstanced(GL_TRIANGLES, 0, _numVertices, i_num);

	glBindVertexArray(0);

}
void Car::clearInstances(){
	i_num = 0;
	i_uploaded = false;
}