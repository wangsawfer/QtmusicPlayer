#include "stdafx.h"
#include "MyWebEngineView.h"

MyWebEngineView::MyWebEngineView(QWidget *parent,QUrl url, QWebEngineProfile* profile)
	: QWebEngineView(parent)
{
	this->setObjectName("web");
	connect(this, &QWebEngineView::loadFinished, this, [this](bool ok) {
		if (ok) {
			emit viewloaded(this);
		}
		});
	m_profile = profile;
}

MyWebEngineView::~MyWebEngineView()
{
}

QWebEngineView* MyWebEngineView::createWindow(MyWebEnginePage::WebWindowType type)
{
	if(m_profile == nullptr) {
		qDebug() << "Error: m_profile is null in createWindow";
	}
	MyWebEngineView* newView = new MyWebEngineView(NULL, QUrl(), m_profile);
	MyWebEnginePage* newPage = new MyWebEnginePage(m_profile, newView);
	newView->setPage(newPage);
	emit newViewCreated(newView);   // 让主窗口处理嵌入
	return newView;
}
