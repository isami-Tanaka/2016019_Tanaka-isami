#pragma once
#include<list>
#include "Geometry.h"
struct HomingShot;

// ‹OÕƒNƒ‰ƒX
class Trail
{
private:
	int handle_;
	HomingShot& owner_;
	std::list<Position2>history_; //êŠ—š—ğ

public:
	Trail(HomingShot& owner);
	void SetHandle(int handle);
	void Draw();	// —š—ğ‚Ì•`‰æ(Trail•`‰æ)
	void Update();	// —š—ğ‚ÌXV
	void Clear();
};

