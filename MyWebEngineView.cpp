#include "stdafx.h"
#include "MyWebEngineView.h"

MyWebEngineView::MyWebEngineView(QWidget *parent,QUrl url)
	: QWebEngineView(parent)
{
	this->setUrl(url);
	this->setObjectName("web");
	connect(this, &QWebEngineView::loadFinished, this, [this](bool ok) {
		if (ok) {
			emit viewloaded(this);
		}
		});
}

MyWebEngineView::~MyWebEngineView()
{
}

QWebEngineView* MyWebEngineView::createWindow(QWebEnginePage::WebWindowType type)
{
	MyWebEngineView* newView = new MyWebEngineView(NULL, QUrl(""));
	emit newViewCreated(newView);   // 让主窗口处理嵌入
	return newView;
}
