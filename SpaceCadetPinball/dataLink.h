#pragma once
#include "TPinballTable.h"

class DataLink {
public:
	DataLink();
	DataLink(TPinballTable* t);
	~DataLink();
	void setTable(TPinballTable* t);
	bool init();
	void update(float time);
private:
	void readIncomingAndAct(float time);
};
