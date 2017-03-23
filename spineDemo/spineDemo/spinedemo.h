#ifndef SPINEDEMO_H
#define SPINEDEMO_H

#include <QtWidgets/QMainWindow>
#include "ui_spinedemo.h"

class spineDemo : public QMainWindow
{
	Q_OBJECT

public:
	spineDemo(QWidget *parent = 0);
	~spineDemo();

private:
	Ui::spineDemoClass ui;
};

#endif // SPINEDEMO_H
