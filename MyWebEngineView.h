#pragma once

#include <QWebEngineView>
#include "MyWebEnginePage.h"

class MyWebEngineView  : public QWebEngineView
{
	Q_OBJECT

public:
	MyWebEngineView(QWidget* parent, QUrl url, QWebEngineProfile* profile);
	~MyWebEngineView();
	QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
signals:
	void newViewCreated(MyWebEngineView* view);
	void viewloaded(MyWebEngineView* view);
public:
	QWebEngineProfile* m_profile = nullptr;
};

