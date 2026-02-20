#pragma once

#include <QWebEngineView>

class MyWebEngineView  : public QWebEngineView
{
	Q_OBJECT

public:
	MyWebEngineView(QWidget *parent);
	~MyWebEngineView();
signals:
	void newViewCreated(MyWebEngineView* view);
};

