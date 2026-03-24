#pragma once
#include "appframe.h"

class ApplicationMain : public ApplicationBase
{
public:
	using base = ApplicationBase;

	bool Initialize(HINSTANCE hInstance) override;
	bool Terminate() override;
	bool Input() override;
	bool Process() override;
	bool Render() override;

	bool AppWindowed() override { return true; }
	int DispSizeW() override { return 1280; }
	int DispSizeH() override { return 720; }
};