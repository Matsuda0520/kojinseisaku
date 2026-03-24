#pragma once
#include "appframe.h"

#include <vector>
#include <array>

class BillBoardGauge
{
public:
	BillBoardGauge();
	virtual ~BillBoardGauge();

	void Process();
	void Render();

	void SetPosition(VECTOR pos);
	void SetValue(int current, int max);

private:
	VECTOR _vPos;
	float _width;
	float _height;
	int _valuePercent;
	std::vector<VERTEX3D> _vertex;
	std::vector<unsigned short> _index;
};

