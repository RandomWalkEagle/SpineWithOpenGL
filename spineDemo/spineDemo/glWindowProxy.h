#pragma once
#include <QObject>

class GLWindow;
class spineParser;

class GLWindowProxy : public QObject
{
Q_OBJECT
public:
	GLWindowProxy();
	~GLWindowProxy();

public:
	void showWindow();
	void initSpine(const char* name);

private slots:
	void updateSpine();

private:
	GLWindow*     gl_window;
	spineParser*  spine_parser;
};

