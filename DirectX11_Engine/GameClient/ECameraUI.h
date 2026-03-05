#pragma once
#include "EComponentUI.h"

class ECameraUI
	: public EComponentUI
{

private:

public:
	virtual void Tick_UI() override;


	//============
	// 생성, 소멸자
	//============
	ECameraUI();
	virtual ~ECameraUI();
};

