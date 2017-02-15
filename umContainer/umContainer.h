#include <iostream>
#include <cstdint> // include this header for uint64_t
#include <tuple>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <string>

enum umContainerSizeEnum{small, medium, big, large, huge};


class umContainerBase
{
public:
	std::string umContainerType;
	umContainerSizeEnum ucSize;
	bool umContainerUsable = true;

	static std::vector <std::pair<std::string, std::vector<umContainerBase*>>> allUmContainers;
};


template <class T>
class umContainerData
{
public:
	T* pointer;
	uint64_t uid;

	~umContainerData()
	{
		delete pointer;
	}
};

template <class T>
class umContainer : public umContainerBase
{
	static const int smallSize = 750, mediumSize = 5000, bigSize = 75000, largeSize = 750000, hugeSize = 10000000;

	bool debugEnabled = false;
	//UID means "unique id"
    int umContainerSize = 1000;
	std::vector<umContainerData<T>*> allData;
	std::vector<uint64_t> freeSlots; //Data is last uid of the old member.
	std::vector<uint64_t> watchingUIDs;
	uint64_t lastUID = -1;
	uint64_t minUIDPossible = -1;
	int currentDataCount = 0, maximumDataCount = 0;
	double uidUsedPercentage = 0, maxLoadPercentage;

	void updateMaxDataCount()
	{
		if (currentDataCount > maximumDataCount)
		{
			maximumDataCount = currentDataCount;
		}

		uidUsedPercentage = (double)lastUID / uint64_t(-1);
		maxLoadPercentage = (maximumDataCount / (double)umContainerSize);

		//DECIDE WHETHER UMCONTAINER USABLE OR NOT
		if (lastUID >= uint64_t(-2))
		{
			umContainerUsable = false;
		}
		else if (!umContainerHasFreeSpace())
		{
			umContainerUsable = false;
		}
		else
		{
			umContainerUsable = true;
		}
	}
public:
	uint64_t addData(T* data, std::string FILE = "", int LINE = 0) //Returns UID
	{
		//CHECK WHETHER IS THERE ANY FREE SPACE IN CONTAINER
		if (!umContainerHasFreeSpace())
		{
			writeErrorMessage(error_ContainerFull, 0, FILE, LINE);
			return -1;
		}

		int loopChecker = 0;

		bool replaceWithOldUID = false;
		int oldUID = 0;

		//QUICK RESERVE
		if (freeSlots.size() > 0)
		{
			if (lastUID > freeSlots[0] + umContainerSize)
			{
				oldUID = lastUID;
				replaceWithOldUID = true;
			}

			lastUID = freeSlots[0] + umContainerSize - 1;
			freeSlots.erase(freeSlots.begin());
		}

	begin:
		if (loopChecker > umContainerSize)
		{
			//A PROBLEM ABOUT FREE SPACE CHECKER
			writeErrorMessage(error_FreeSpaceChecker, 0, FILE, LINE);
			return -1;
		}

		lastUID++;
		int position = lastUID % umContainerSize;

		if (allData[position] == 0)
		{
			//WRITE REQUESTED DATA TO UMCONTAINER
			allData[position] = new umContainerData<T>();
			allData[position]->pointer = data;
			allData[position]->uid = lastUID;

			//UPDATE PARMETERS
			currentDataCount++;
			updateMaxDataCount();

			//RETURN UID NUMBER OF REQUESTED DATA
			minUIDPossible++;
			
			if (replaceWithOldUID)
			{
				int uidToReturn = lastUID;
				lastUID = oldUID;

				return uidToReturn;
			}

			return lastUID;
		}
		else
		{
			loopChecker++;
			goto begin;
		}

		return -1;
		
	}
	void removeData(uint64_t dataUID, std::string FILE = "", int LINE = 0) //Removes and deletes data has specific uid (dataUID)
	{
		//DO NOTHING IF UINT64 MAX PASSED TO FUNCTION (UNINITIALIZED POINTER REQUESTED REMOVE DATA)
		if (dataUID == uint64_t(-1))
		{
			writeErrorMessage(error_RequestRemoveDataWithUnitilizedUMPTR, dataUID, FILE, LINE);
			return;
		}

		int position = dataUID % umContainerSize;

		if (allData[position] != 0)
		{
			if (allData[position]->uid == dataUID)
			{
				if (debugEnabled)
				{
					for (size_t i = 0; i < watchingUIDs.size(); i++)
					{
						if (watchingUIDs[i] == dataUID)
						{
							writeErrorMessage(debug_WatchingDataErased, dataUID, FILE, LINE);
							break;
						}
					}
				}

				currentDataCount--;
				delete allData[position];
				allData[position] = 0;

				//ADD TO FREE SLOTS
				if ((float)currentDataCount / (float)umContainerSize > 0.9f && freeSlots.size() < (float)umContainerSize / 20.0f)
				{
					freeSlots.push_back(dataUID);
				}
			}
			else
			{
				writeErrorMessage(error_MultipleRemoveRequest2, dataUID, FILE, LINE);
			}
		}
		else
		{
			writeErrorMessage(error_MultipleRemoveRequest, dataUID, FILE, LINE);
		}

		updateMaxDataCount();
	}
	T* getData(uint64_t dataUID, std::string FILE = "", int LINE = 0) //Gets the data has specific uid (dataUID)
	{
		//RETURN ZERO IF UINT64 MAX REQUESTED
		if (dataUID == uint64_t(-1))
		{
			writeErrorMessage(error_UnitializedUMPTRRequestGetData, dataUID, FILE, LINE);
			return 0;
		}

		int position = dataUID % umContainerSize;

		if (position < umContainerSize && position >= 0)
		{
			if (allData[position] != 0)
			{
				if (dataUID == allData[position]->uid)
				{
					//DATA IS DELIVERED
					return allData[position]->pointer;
				}
				else
				{
					//REQUESTED DATA IS OLD AND ERASED
					writeErrorMessage(error_DataReplaced, dataUID, FILE, LINE);
				}
			}
			else
			{
				//REQUESTED DATA IS "0"
				writeErrorMessage(error_DataErased, dataUID, FILE, LINE);
			}
		}
		else
		{
			//REQUEST OUT OF BOUNDS
			writeErrorMessage(error_DataRequestOutOfBounds, dataUID, FILE, LINE);
		}

		return 0;
	}
	bool umContainerHasFreeSpace()
	{
		if (currentDataCount < umContainerSize)
		{
			return true;
		}

		return false;
	}
	int getDataCount()
	{
		return currentDataCount;
	}
	int getMaxDataCount()
	{
		return maximumDataCount;
	}
	void registerUIDWatch(uint64_t dataUID)
	{
		debugEnabled = true;
		watchingUIDs.push_back(dataUID);
	}


	//UmContainer free space checking is totally legal and efficient but it is not the right way to use umcontainer it is just for debugging. UmContainer must not be full all the time. 
	//If it is reaching its capacity sometimes please use a larger umContainer. You can check currentDataCount and maximumDataCount variables for finding best umcontainer size for the specific purpose.
	
	umContainer(int size = 1)
	{
		this->ucSize = (umContainerSizeEnum)size;
		this->umContainerType = typeid(T).name();

		switch (size)
		{
		case 0:
			umContainerSize = smallSize;
			break;
		case 1:
			umContainerSize = mediumSize;
			break;
		case 2:
			umContainerSize = bigSize;
			break;
		case 3:
			umContainerSize = largeSize;
			break;
		case 4:
			umContainerSize = hugeSize;
			break;
		default:
			break;
		}

		allData = std::vector<umContainerData<T>*>(umContainerSize);
	}
	~umContainer() {}

	static void writeErrorMessage(std::string errorMessage, uint64_t UID = -1, std::string FILE = "", int LINE = 0)
	{
		//return;
		//WRITE ERROR MESSAGE TO CONSOLE OR LOG
		if (UID == -1)
		{
			std::cout << errorMessage.c_str() << " | at " << FILE << " - " << LINE << std::endl;
		}
		else
		{
			std::cout << errorMessage.c_str() << " - " << UID << " | at " << FILE << " - " << LINE << std::endl;
		}
		
	}
};

static std::string error_ContainerFull = "Container is full. So data cannot be added.";
static std::string error_DataErased = "The requeted data has been erased before.";
static std::string error_DataReplaced = "The requeted data has been replaced before.";
static std::string error_DataRequestOutOfBounds = "The request is out of bounds.";
static std::string error_DataRemoved = "Data successfully removed from container.";
static std::string error_RequestRemoveDataWithUnitilizedUMPTR = "Unitiliazed umptr is requesting remove data.";
static std::string error_UnitializedUMPTRRequestGetData= "Unitiliazed umptr is requesting get data.";
static std::string error_FreeSpaceChecker = "Free space checker is not working properly.";
static std::string error_MultipleRemoveRequest = "Multiple remove request obtained. This is not going to corrupt or cause any other problem for umContainer.";
static std::string error_MultipleRemoveRequest2 = "Multiple remove request obtained. This is not going to corrupt or cause any other problem for umContainer. (Another data is on the place.)";
static std::string debug_WatchingDataErased = "Erased watching uid.";


template <class T>
class umptr
{
	//PLEASE READ BEFORE USING UMPTR. (UMPTR is non-copyable class)
	//You cannot use std::vector<umptr>, if you want to use "umptr"s in a vector you have to use pointers of it (std::vector<umptrs*>)
	//You cannot use assign operator (umptr<someClass> a = x;), use copyFrom function instead of it. (a.copyFrom(x);)
	//You cannot use copy constructor of umptr.

private:
	//This variable determines will actual data be erased or not when this umptr's destructor called. It is not about reaching actual data, you can reach actual data even this variable is false.
	//(Actual data is what you get when you call getData() function)
	bool bounded = false;
	bool notBoundable = false;

	uint64_t uid = -1;
	umContainer<T>* container = 0;

	void findUmContainer()
	{
		std::string umContainerType = typeid(T).name();

		//SEARCH FOR PROPER UMCONTAINER
		for (size_t i = 0; i < umContainerBase::allUmContainers.size(); i++)
		{
			if (umContainerBase::allUmContainers[i].first == umContainerType)
			{
				for (size_t a = 0; a < umContainerBase::allUmContainers[i].second.size(); a++)
				{
					if (umContainerBase::allUmContainers[i].second[a]->umContainerUsable)
					{
						//FOUND A PROPER UMCONTAINER
						container = (umContainer<T>*)umContainerBase::allUmContainers[i].second[a];
						return;
					}
				}

				//IF PROPER UMCONTAINER NOT FOUND CREATE NEW ONE
				umContainerBase::allUmContainers[i].second.push_back(new umContainer<T>(std::min(4, (int)umContainerBase::allUmContainers[i].second.size())));
				container = (umContainer<T>*)umContainerBase::allUmContainers[i].second.back();
				return;
			}
		}

		//IF THERE IS NO CONTAINER SLOT FOR THIS OBJECT TYPE CREATE NEW SLOT
		umContainerBase::allUmContainers.push_back(std::make_pair(umContainerType, std::vector<umContainerBase*>()));
		umContainerBase::allUmContainers.back().second.push_back(new umContainer<T>(0));
		container = (umContainer<T>*)umContainerBase::allUmContainers.back().second.back();
	}
	umContainer<T>* getContainer()
	{
		return container;
	}

	umptr(umptr& other)
	{
		this->uid = other.uid;
		this->container = other.container;
		this->bounded = other.bounded;

		//std::cout << "USED" << std::endl;

		other.UnBound();
		this->UnBound();
	}
	umptr& operator=(umptr& other)
	{
		this->uid = other.uid;
		this->container = other.container;
		this->bounded = other.bounded;

		other.UnBound();
		this->UnBound();

		return *this;
	}

	//EXTRA INFORMATION: When you use std::vector<someObject>, push_back or erase functions will call copy constructors, move assignment operator and destructor of the someObject.
public:
	umptr() {}
	umptr(T* data)
	{
		init(data);
	}
	~umptr()
	{
		if (bounded && container != 0)
		{
			container->removeData(uid);
		}
		
	}

	//Initializes umptr and bounds with the data.
	void init(T* data)
	{
		findUmContainer();
		uid = container->addData(data);

		this->Bound();
	}
	void copyFrom(umptr<T> &from, bool willBeBounded = false)
	{
		this->uid = from.uid;
		this->container = from.container;
		this->bounded = willBeBounded;
	}

	//Erases actual data
	void eraseData()
	{
		container->removeData(uid);
		notBoundable = true;
		UnBound();
	}
	void eraseData(std::string FILE, int LINE)
	{
		container->removeData(uid, FILE, LINE);
		notBoundable = true;
		UnBound();
	}

	//This is totally safe function. There is not any danger to call this function even actual data is erased. You will get nullptr(0) if actual data erased before.
	T* getData()
	{
		return container->getData(uid);
	}
	T* getData(std::string FILE, int LINE)
	{
		return container->getData(uid, FILE, LINE);
	}

	//This function unbounds umptr and data. It means the actual data will not be erased when umpointer's destructor called. However, you can still reach the actual data.
	void UnBound()
	{
		bounded = false;
	}
	//This function bounds umptr and data. It means the actual data will be erased when umpointer's destructor called.
	void Bound()
	{
		if (!notBoundable)
		{
			bounded = true;
		}
	}
	bool isBounded()
	{
		return bounded;
	}
	
	//Registers to container for getting information when its actual data erased. (It is just for debugging purposes.)
	void RegisterForDebugging()
	{
		if (container != 0) container->registerUIDWatch(uid);
	}
};

#define getumptr(ptr) ptr.getData(__FILE__, __LINE__);
#define removeumptr(ptr) ptr.removeData(__FILE__, __LINE__);