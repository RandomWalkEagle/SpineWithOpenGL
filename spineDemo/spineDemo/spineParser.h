#pragma once

#include "spineVertex.h"
#include <QVector>

class spineParser
{
public:
	spineParser();
	~spineParser();

public:
	void initSpine(const char* name);
	void update(float deltaTime);
	QVector<SpineVertex> parseSpineVertex();

	void initStretchyMan();
	void initTank();
	void initRaptor();
	void initgoblins();
	void initspineboy();

private:

};

