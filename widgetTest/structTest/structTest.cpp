#include <cstdlib>
#include <iostream>

struct colour returnStructure(float _r, float _g, float _b)
{
	struct colour temp;
	temp.m_r=_r;
	temp.m_g=_g;
	temp.m_b=_g;
	return temp;
}


int main()
{
	typedef struct colour
	{
		float m_r;
		float m_g;
		float m_b;
	} myCol;

colour a;



}
