#ifndef EAGLE_GL_WINDOW_H_
#define EAGLE_GL_WINDOW_H_

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include "spineVertex.h"

class GLWindow : public QGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	GLWindow(QGLWidget* parent = NULL);
	~GLWindow();

public:
	QSize initTexture(const char* path);
	void resetSpineData(QVector<SpineVertex> vertexs);

protected:
	//对3个纯虚函数的重定义  
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	void printVersionInformation();

	void initSpineBuffer();
	void drawSpineData();

	void initFixedPipeLineEnv();
	void initFixedPipeViewPort(int width, int height);
	void drawFixedPipeLineElements();
	
	void initDemoData();
	void drawDemoData();

private:
	QGLShaderProgram glShaderPGM;
	QOpenGLBuffer            glVertex;
	QOpenGLTexture*  texture;

	QImage texImage;
	
	QVector<SpineVertex> data;
};

#endif /* EAGLE_GL_WINDOW_H_ */
