#include <tuple>
#include <iostream>
#include "asede.h"

#include "umContainer.h"

class ex1
{
public:
	int a;
	float b;
	double c;
	bool y;
	std::pair <float, float> pos;
	umptr<std::pair<float, bool>> hegza;
	//asede abc;


	ex1(int a_, float b_, double c_, bool y_, std::pair<float, float> pos_);
	~ex1();
};

