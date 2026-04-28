#include "stdafx.h"
#include "MyWebEnginePage.h"

MyWebEnginePage::MyWebEnginePage(QWebEngineProfile* profile,QWidget *parent)
	: QWebEnginePage(profile, parent)
{}

MyWebEnginePage::~MyWebEnginePage()
{}

bool MyWebEnginePage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) {
    // 关键步骤：确保所有情况都调用父类方法，返回 true
    return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
}