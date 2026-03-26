#pragma once
#include "appframe.h"

class ModeTitle : public ModeBase
{
public:
	bool Initialize() override { return true; }
	bool Terminate() override { return true; }
	bool Process() override;
	bool Render() override;
};

