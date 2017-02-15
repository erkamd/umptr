#include <iostream>
#include <time.h>
#include <vector>
#include <memory>
#include <ctime>


#include "ex1.h"

using namespace std;

static umContainer<double> umc;
static umContainer<ex1> umc2;
static vector<uint64_t> uids;
static vector<uint64_t> uids2;

static vector<umptr<ex1>*> ptrs;
static vector<umptr<double>*> ptrs2;

std::vector<ex1*> dg;

void randomActions_DEF()
{
	int rnd = rand() % 234;

	if (rnd < 120)
	{
		dg.push_back(new ex1(rand(), 3, 4, false, std::make_pair(12.2, 13.2)));
	}
	else if (dg.size() > 0)
	{
		//REMOVE ELEMENT
		int elementNo = rnd % dg.size();
		delete dg[elementNo];
		dg.erase(dg.begin() + elementNo);
	}
}

void randomActions3()
{
	//get random number

	int rnd = rand() % 234;

	if (rnd < 120)
	{
		//ADD ELEMENT
		ex1* lcl = new ex1(rand(), 3, 4, false, std::make_pair(12.2, 13.2));

		ptrs.push_back(new umptr<ex1>(lcl));
		//ptrs.back()->init(lcl);

		//if (rnd == 24)
		//{
		//	umptr<ex1> *a2 = new umptr<ex1>();
		//	a2->copyFrom(*ptrs.back(), true);
		//}
		
		
		//umptr<ex1> *ptr2 = new umptr<ex1>(new ex1(1, 2, 3, true, std::make_pair( 14, 4)));
		//umptr<double> *ptr3 = new umptr<double>(new double(24.3422));

		/*umptr<ex1> ptr;
		ptr.pointTo(lcl);

		ptrs.push_back(umptr<ex1>());
		ptr.UnBound();*/

		//ptrs.push_back(ptr2);

		//ptrs2.push_back(ptr3);
	}
	else if (ptrs.size() > 0)
	{
		//REMOVE ELEMENT
		int elementNo = rnd % ptrs.size();
		ptrs[elementNo]->eraseData(__FILE__, __LINE__);
		delete ptrs[elementNo];
		ptrs.erase(ptrs.begin() + elementNo);
	}
}
void randomActions2()
{
	//get random number

	int rnd = rand() % 234;

	if (rnd < 120)
	{
		//ADD ELEMENT
		ex1* lcl = new ex1(rand(), 3, 4, false, std::make_pair(12.2, 13.2));
		/*umptr<ex1> *ptr = new umptr<ex1>(lcl);

		umptr<ex1> *ptr2 = new umptr<ex1>(new ex1(1, 2, 3, true, std::make_pair( 14, 4)));*/

		
		int a = umc2.addData(lcl);
		if (a != -1)
		{
			uids2.push_back(a);
		}
	}
	else if (uids2.size() > 0)
	{
		//REMOVE ELEMENT
		int elementNo = rnd % uids2.size();
		umc2.removeData(uids2[elementNo]);
		uids2.erase(uids2.begin() + elementNo);
	}
}

void randomActions()
{
	//get random number
	
	int rnd = rand() % 234;

	if (rnd < 210)
	{
		//ADD ELEMENT
		double* lcl = new double(123);
		int a = umc.addData(lcl);
		if (a != -1)
		{
			uids.push_back(a);
		}
	}
	else if (uids.size() > 0)
	{
		//REMOVE ELEMENT
		int elementNo = rnd % uids.size();
		umc.removeData(uids[elementNo]);
		//uids.erase(uids.begin() + elementNo);
	}
}

umptr<ex1> abc;
umptr<std::pair<float, bool>> abc2;

void example()
{
	/*umptr<ex1> xy = umptr<ex1>(new ex1(1, 1, 1, false, make_pair(2, 3)));
	abc = xy;
	abc2 = xy.getData()->hegza;*/

	//xy.UnBound();

	abc.init(new ex1(1, 1, 1, true, make_pair(89, 89)));

	umptr<ex1> a25;
	a25.copyFrom(abc);
	
}

int main()
{

	std::clock_t start;
	double duration;

	start = std::clock();

	
	example();

	srand(time(NULL));
	int a = 0;
	do
	{
		a++;
		//randomActions();
		//randomActions2();
		randomActions3();
		//randomActions_DEF();
	} while (a < 100000);

	
	

	/*for (size_t i = 0; i < 150; i++)
	{
		umc.getData(rand());
	}*/

	/*for (size_t i = 0; i < uids.size(); i++)
	{
		umc.getData(uids[i]);
	}
	for (size_t i = 0; i < uids2.size(); i++)
	{
		umc2.getData(uids2[i]);
	}*/

	//void* lcl = abc.getData();

	//std::vector<umContainer<ex1>*> aaa;
	//for (size_t i = 0; i < umContainerBase::allUmContainers[0].second.size(); i++)
	//{
	//	aaa.push_back((umContainer<ex1>*)umContainerBase::allUmContainers[0].second[i]);
	//} 
	
	/* Your algorithm here */

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "printf: " << duration << '\n';

	//cout << "Hello World!" << endl;
	return 0;
}


