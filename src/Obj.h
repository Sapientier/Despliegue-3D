#pragma once
#include "Model.h"

class CObj : public CModel
{
public:
	CObj();
	~CObj();
	void read_obj(string path);
	vector<string> split(const string &s, char delim);
};
