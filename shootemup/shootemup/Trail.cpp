#include<DxLib.h>
#include"HomingShot.h"
#include "Trail.h"

constexpr size_t history_lilmit = 10;

void Trail::SetHandle(int handle)
{
	handle_ = handle;
}


Trail::Trail(HomingShot& owner) :owner_(owner)
{

}



// óöóÇÃçXêV
void Trail::Update()
{
	history_.push_front(owner_.pos);	// ì™Ç…ì¸ÇÍÇÈ
	if (history_.size() > history_lilmit)
	{
		history_.pop_back();			// Ç®ÇµÇËÇ©ÇÁèoÇ∑
	}
}


// óöóÇÃï`âÊ(Trailï`âÊ)
void Trail::Draw()
{
	float tiickness = 20.0f;
	auto lastPos = owner_.pos;
	if (history_.empty())return;
	float div = 1.0f / static_cast<float>(history_.size());
	float u = 0.0f;
	for (const auto& pos : history_)
	{
		if (pos == lastPos)continue;
		//DrawLineAA(lastPos.x, lastPos.y, pos.x, pos.y, 0xffffff, tiickness);
		//tiickness *= 0.8f;

		auto v = pos - lastPos;
		v.Normalize();
		v = Vector2(-v.y, v.x);
		auto p1 = lastPos + v * 16;
		auto p2 = pos + v * 16;
		auto p3 = pos - v * 16;
		auto p4 = lastPos - v * 16;

		DrawRectModiGraph(
			p1.x, p1.y,
			p2.x, p2.y,
			p3.x, p3.y,
			p4.x, p4.y,
			u * 256, 0,
			div * 256, 64, handle_, true
			);
		u += div;
		lastPos = pos;
	}

}


void Trail::Clear()
{
	history_.clear();
}
