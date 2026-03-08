#include "stdafx.h"
#include "mainwindow.h"

void mainwindow::webinit() {
	if(web_window == nullptr) {
		web_window = new WebWindow();
		connect(web_window, &WebWindow::downloaded, this, &mainwindow::download);
	}
	web_window->show();
}

void mainwindow::download() {
	if (download_page == nullptr) {
		download_page = new downloadWindow();
	}
	download_page->show();
}