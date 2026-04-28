#pragma once

#include <QMainWindow>
#include "ui_selectBackgroundwindow.h"
#include "slideRect.h"

class selectBackgroundwindow : public QMainWindow
{
	Q_OBJECT

public:
	selectBackgroundwindow(const QPixmap &pixmap,QWidget *parent = nullptr,double scale = 1.0);
	~selectBackgroundwindow();

private:
	Ui::selectBackgroundClass *ui;
	QGraphicsScene* scene;
	double scale;
};

