#include "stdafx.h"
#include "WebWindow.h"

WebWindow::WebWindow(QWidget *parent)
	: QMainWindow(parent), web_ui(new Ui::webWindow),webContain(new WebContain(NULL))
{
	web_ui->setupUi(this);

	web_ui->gridLayout->addWidget(webContain, 1, 0, 1, 1);
	QPushButton* newWebButton = new QPushButton("+");
	newWebButton->setFixedSize(25, 25);
	webContain->setCornerWidget(newWebButton);

	connect(newWebButton, &QPushButton::clicked, this, &WebWindow::newWeb);
	connect(webContain, &WebContain::currentChanged, this, &WebWindow::updateURL);
	connect(web_ui->actiondownload, &QAction::triggered, this, &WebWindow::download);
	connect(web_ui->actioncatch, &QAction::triggered, this, &WebWindow::catchUrl);
}

WebWindow::~WebWindow()
{
	delete web_ui;
	delete webContain;

}

void WebWindow::updateURL(int index) {
	if (index != -1) {
		web_ui->URL->setText(webContain->webList.at(index)->url().toString());
		disconnect(web_ui->stop);
		disconnect(web_ui->back);
		disconnect(web_ui->forward);
		disconnect(web_ui->refresh);
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

void WebWindow::newWeb() {
	webContain->webList.append(new MyWebEngineView(NULL,QUrl("https://www.bilibili.com")));
	webContain->addTab(webContain->webList.last(), webContain->webList.last()->url().toString());
	connect(webContain->webList.last(), &MyWebEngineView::viewloaded, this, &WebWindow::updatetitle);
	connect(webContain->webList.last(), &MyWebEngineView::newViewCreated, this, &WebWindow::openWeb);
	connect(webContain->webList.last()->page(), &QWebEnginePage::linkHovered, this, &WebWindow::linkhover);
}

void WebWindow::openWeb(MyWebEngineView* view) {
	int index = webContain->currentIndex();
	webContain->insertTab(index + 1, view, "laoding...");
	connect(view, &MyWebEngineView::newViewCreated, this, &WebWindow::openWeb);
	connect(view, &MyWebEngineView::viewloaded, this, &WebWindow::updatetitle);
	connect(view->page(), &QWebEnginePage::linkHovered, this, &WebWindow::linkhover);
	webContain->webList.insert(index + 1, view);
	webContain->setCurrentIndex(index + 1);
}

void WebWindow::closeEvent(QCloseEvent* event) {
	for (int i = 0; i < webContain->webList.size(); i++) {
		webContain->webList.at(i)->close();
	}
	event->accept();
}

void WebWindow::updatetitle(MyWebEngineView* view) {
	int index = webContain->indexOf(view);
	if (index != -1) {
		webContain->setTabText(index, view->title());
		if(index == webContain->currentIndex()) {
			updateURL(index);
		}
	}
}

void WebWindow::download() {
	emit(downloaded());
}

void WebWindow::linkhover(const QUrl& url) {
	linkhoverURL = url.toString();
}

void WebWindow::catchUrl() {
	if (linkhoverURL != "") {
		web_ui->detectoutput->appendPlainText(linkhoverURL);
	}
}