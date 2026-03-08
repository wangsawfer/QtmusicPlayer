#pragma once

#include <QWebEngineView>

class MyWebEngineView  : public QWebEngineView
{
	Q_OBJECT

public:
	MyWebEngineView(QWidget* parent, QUrl url);
	~MyWebEngineView();
	QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
signals:
	void newViewCreated(MyWebEngineView* view);
	void viewloaded(MyWebEngineView* view);
};

