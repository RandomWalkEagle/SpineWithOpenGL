#pragma once
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

class SpineVertex
{
public:
	SpineVertex();
	~SpineVertex();

public:
	static inline int posOffset() { return offsetof(SpineVertex, pos); }
	static inline int colorOffset() { return offsetof(SpineVertex, color); }
	static inline int texCordOffset() { return offsetof(SpineVertex, texCord); }

public:
	QVector3D pos;    //pos
	QVector4D color;  //color
	QVector2D texCord;   //texture cord
};

