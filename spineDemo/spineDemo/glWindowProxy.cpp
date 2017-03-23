#include "glWindowProxy.h"
#include "glwindow.h"
#include "spineParser.h"
#include <QTimer>
#include <string.h>

static int timeDelta = 30;

GLWindowProxy::GLWindowProxy()
{
	gl_window = new GLWindow();
}


GLWindowProxy::~GLWindowProxy()
{
	gl_window->close();
	delete gl_window;
}

void GLWindowProxy::showWindow()
{
	gl_window->show();
}

void GLWindowProxy::initSpine(const char* name)
{
	spine_parser->initSpine(name);
	gl_window->resize(640, 640);

	QString path = QString("./data/%1.png").arg(name);
	std::string temp = path.toStdString();
	gl_window->initTexture(temp.c_str());

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateSpine()));
	timer->start(timeDelta);
}

void GLWindowProxy::updateSpine()
{
	spine_parser->update((float)timeDelta/1000.0);

	QVector<SpineVertex> vertexs = spine_parser->parseSpineVertex();
	gl_window->resetSpineData(vertexs);

	gl_window->updateGL();
}
