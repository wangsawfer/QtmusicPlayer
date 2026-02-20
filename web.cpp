#include "stdafx.h"
#include "mainwindow.h"

void mainwindow::webinit() {
	
	web_window->show();
}

void mainwindow::updateURL(int index) {
	if (index != -1) {
		web_ui->URL->setText(webContain->webList.at(index)->url().toString());
		connect(web_ui->back, &QPushButton::clicked, webContain->webList.at(index), &QWebEngineView::back);
		connect(web_ui->forward, &QPushButton::clicked, webContain->webList.at(index), &QWebEngineView::forward);
		connect(web_ui->refresh, &QPushButton::clicked, webContain->webList.at(index), &QWebEngineView::reload);
		connect(web_ui->stop, &QPushButton::clicked, webContain->webList.at(index), &QWebEngineView::stop);
	}
	else {
		web_ui->URL->setText("");
		disconnect(web_ui->stop);
		disconnect(web_ui->back);
		disconnect(web_ui->forward);
		disconnect(web_ui->refresh);
	}
}

void mainwindow::newWeb() {
	webContain->webList.append(new MyWebEngineView(NULL));
	MyWebEngineView* NewWeb = webContain->webList.last();
	NewWeb->setUrl(QUrl("https://www.bilibili.com/"));
	webContain->addTab(NewWeb, NewWeb->url().toString());
}