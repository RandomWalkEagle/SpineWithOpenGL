#include "glwindow.h"
#include<gl/glu.h>
#include <QTimer>

GLWindow::GLWindow(QGLWidget* parent)
	: QGLWidget(parent){
}

GLWindow::~GLWindow() {
	glShaderPGM.release();
	glVertex.release();
	glVertex.destroy();
}


QSize GLWindow::initTexture(const char* path)
{
	texImage = QImage(path);
	return texImage.size();
}


void GLWindow::resetSpineData(QVector<SpineVertex> vertexs)
{
	data = vertexs;
}

void GLWindow::initializeGL()
{
	//initFixedPipeLineEnv();
	initializeOpenGLFunctions();
	printVersionInformation();
	//initDemoData();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	bool result;
	result = glShaderPGM.addShaderFromSourceFile(QGLShader::Vertex, 
		"./shader/simple.vert");
	if (!result)
		qDebug() << glShaderPGM.log();
	result = glShaderPGM.addShaderFromSourceFile(QGLShader::Fragment, 
		"./shader/simple.frag");
	if (!result)
		qDebug() << glShaderPGM.log();

	glShaderPGM.bindAttributeLocation("vertex", 0);
	glShaderPGM.bindAttributeLocation("color", 1);
	glShaderPGM.bindAttributeLocation("texCoord", 2);

	result = glShaderPGM.link();
	if (!result)
		qDebug() << glShaderPGM.log();


	//texture = new QOpenGLTexture(texImage.mirrored);
	texture = new QOpenGLTexture(texImage);

	glShaderPGM.bind();
	glShaderPGM.setUniformValue("texture", 0);
}

void GLWindow::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);
	//initFixedPipeViewPort(width, height);
}

void GLWindow::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 100.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//drawFixedPipeLineElements();
	//drawDemoData();
	drawSpineData();
}

void GLWindow::printVersionInformation()
{
	QString glType;
	QString glVersion;
	QString glProfile;

	// Get Version Information
	//glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
	glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	// Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
	switch (format().profile())
	{
		CASE(NoProfile);
		CASE(CoreProfile);
		CASE(CompatibilityProfile);
	}
#undef CASE

	qDebug() << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}


void GLWindow::initSpineBuffer()
{
	glVertex.release();
	glVertex.destroy();

	glVertex.create();
	glVertex.bind();
	glVertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
	glVertex.allocate(data.constData(), data.size() * sizeof(SpineVertex));

	int offset = SpineVertex::posOffset();
	offset = SpineVertex::colorOffset();
	offset = SpineVertex::texCordOffset();
	offset = sizeof(SpineVertex);

	glShaderPGM.enableAttributeArray(0);
	glShaderPGM.enableAttributeArray(1);
	glShaderPGM.enableAttributeArray(2);

	glShaderPGM.setAttributeBuffer(0, GL_FLOAT, SpineVertex::posOffset(), 3, sizeof(SpineVertex));
	glShaderPGM.setAttributeBuffer(1, GL_FLOAT, SpineVertex::colorOffset(), 4, sizeof(SpineVertex));
	glShaderPGM.setAttributeBuffer(2, GL_FLOAT, SpineVertex::texCordOffset(), 2, sizeof(SpineVertex));
}


void GLWindow::drawSpineData()
{
	QMatrix4x4 m;
	m.ortho(-1000.0f, 1000.0f, 1000.0f, -1000.0f, 0.0f, 15.0f);
	m.translate(600.0f, 0.0f, -5.0f);
	QQuaternion rotate = QQuaternion::fromAxisAndAngle(0, 0, 1, 180);
	m.rotate(rotate);
	//m.scale(0.6);

	glShaderPGM.setUniformValue("matrix", m);

	initSpineBuffer();
	texture->bind();
	glDrawArrays(GL_TRIANGLES, 0, data.size());
}

void GLWindow::initFixedPipeLineEnv()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景  
	glShadeModel(GL_SMOOTH);                            //启用阴影平滑  

	glClearDepth(1.0);                                  //设置深度缓存  
	glEnable(GL_DEPTH_TEST);                            //启用深度测试  
	glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
}


void GLWindow::initFixedPipeViewPort(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);               //重置当前的视口  
	glMatrixMode(GL_PROJECTION);                        //选择投影矩阵  
	glLoadIdentity();                                   //重置投影矩阵  
														//设置视口的大小  
	//gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);                         //选择模型观察矩阵  
	glLoadIdentity();                                   //重置模型观察矩阵  
}

void GLWindow::drawFixedPipeLineElements()
{
	glLoadIdentity();                                   //重置当前的模型观察矩阵 

	glTranslatef(-1.5f, 0.0f, -6.0f);                   //左移1.5单位，并移入屏幕6.0单位  
	glBegin(GL_TRIANGLES);                              //开始绘制三角形  
	glVertex3f(0.0f, 1.0f, 0.0f);                   //上顶点  
	glVertex3f(-1.0f, -1.0f, 0.0f);                 //左下  
	glVertex3f(1.0f, -1.0f, 0.0f);                  //右下  
	glEnd();                                            //三角形绘制结束  

	glTranslatef(3.0f, 0.0f, 0.0f);                     //右移3.0单位  
	glBegin(GL_QUADS);                                  //开始绘制四边形  
	glVertex3f(-1.0f, 1.0f, 0.0f);                  //左上  
	glVertex3f(1.0f, 1.0f, 0.0f);                   //右上  
	glVertex3f(1.0f, -1.0f, 0.0f);                  //左下  
	glVertex3f(-1.0f, -1.0f, 0.0f);                 //右下  
	glEnd();                                        //四边形绘制结束 
}

void GLWindow::initDemoData()
{
	static const int coords[6][4][3] = {
		{ { 800, 400, 0 },{ 400, 400, 0 },{ 400, 800, 0 },{ 800, 800, 0 } },
		{ { 800, 800, 0 },{ 400, 800, 0 },{ 400, 800, 0 },{ 800, 800, 0 } },
		{ { 800, 400, 0 },{ 800, 400, 0 },{ 800, 800, 0 },{ 800, 800, 0 } },
		{ { 400, 400, 0 },{ 800, 400, 0 },{ 400, 800, 0 },{ 400, 800, 0 } },
		{ { 800, 400, 0 },{ 800, 400, 0 },{ 400, 400, 0 },{ 800, 400, 0 } },
		{ { 400, 800, 0 },{ 800, 400, 0 },{ 400, 800, 0 },{ 400, 800, 0 } }
	};


	QVector<GLfloat> vertData;
	int i = 0;
	for (int j = 0; j < 4; ++j) {
		// vertex position
		vertData.append(0.2 * coords[i][j][0]);
		vertData.append(0.2 * coords[i][j][1]);
		vertData.append(0.2 * coords[i][j][2]);
		// texture coordinate
		vertData.append(j == 0 || j == 3);
		vertData.append(j == 0 || j == 1);
	}

	glVertex.create();
	glVertex.bind();
	glVertex.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}

void GLWindow::drawDemoData()
{
	QMatrix4x4 m;
	m.ortho(0.0f, 400.0f, 400.0f, 0.0f, 4.0f, 15.0f);
	m.translate(0.0f, 0.0f, -10.0f);

	glShaderPGM.setUniformValue("matrix", m);
	glShaderPGM.enableAttributeArray(0);
	glShaderPGM.enableAttributeArray(2);
	glShaderPGM.setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
	glShaderPGM.setAttributeBuffer(2, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

	texture->bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
