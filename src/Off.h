#pragma once
#include "Model.h"

class COff : public CModel
{
public:
	COff();
	~COff();
	void read_off(string path);
};