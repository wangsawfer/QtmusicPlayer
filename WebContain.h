#pragma once

#include <QTabWidget>
#include "MyWebEngineView.h"

class WebContain  : public QTabWidget
{
	Q_OBJECT

public:
	WebContain(QWidget *parent);
	~WebContain();
	void updateWebList(int index);
public:
	QList<MyWebEngineView*> webList;
};

