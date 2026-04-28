#pragma once

#include <QWebEnginePage>

class MyWebEnginePage  : public QWebEnginePage
{
	Q_OBJECT

public:
	MyWebEnginePage(QWebEngineProfile* profile,QWidget *parent=nullptr);
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) override;
	~MyWebEnginePage();
};

