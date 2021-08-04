#pragma once
#include<list>
#include "Geometry.h"
struct HomingShot;

// OΥNX
class Trail
{
private:
	int handle_;
	HomingShot& owner_;
	std::list<Position2>history_; //κπ

public:
	Trail(HomingShot& owner);
	void SetHandle(int handle);
	void Draw();	// πΜ`ζ(Trail`ζ)
	void Update();	// πΜXV
	void Clear();
};

