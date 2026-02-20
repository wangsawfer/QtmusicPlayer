#include "stdafx.h"
#include "MyWebEngineView.h"

MyWebEngineView::MyWebEngineView(QWidget *parent)
	: QWebEngineView(parent)
{
	this->setUrl(QUrl("https://www.bilibili.com/"));
	this->setObjectName("web");
}

MyWebEngineView::~MyWebEngineView()
{
	
}

