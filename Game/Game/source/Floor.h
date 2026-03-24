#pragma once
#include "GameComposite.h"

// PrimitivePlaneを使って床オブジェクトとして実装する
class Floor : public GameComposite
{
public:
	Floor(int grHandle, float tileSize, int tileX, int tileZ, const char* name = "Floor");
	~Floor() override = default;

	void Render() override;

private:
	PrimitivePlane _plane;
	int _grHandle;

};

