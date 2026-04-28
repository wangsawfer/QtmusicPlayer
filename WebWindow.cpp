#include "stdafx.h"
#include "WebWindow.h"

void WebWindow::setupProfile() {
	QString dataPath = QApplication::applicationDirPath() + "/cookies";
	QDir().mkpath(dataPath);
	QDir().mkpath(dataPath + "/cache");
	g_profile = new QWebEngineProfile(dataPath, this);
	g_profile->setPersistentStoragePath(dataPath);
	g_profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
	g_profile->setCachePath(dataPath + "/cache");
}

//构建函数，初始化UI和WebContain，并连接信号槽
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
	connect(webContain, &QTabWidget::tabCloseRequested, this, &WebWindow::closeWeb);

	setupProfile();
}

WebWindow::~WebWindow()
{
	delete web_ui;
	delete webContain;
}

//更新URL显示和按钮状态
void WebWindow::updateURL(int index) {
	if (index != -1) {

		web_ui->URL->setText(qobject_cast<MyWebEngineView*>(webContain->widget(index))->url().toString());
		disconnect(web_ui->stop);
		disconnect(web_ui->back);
		disconnect(web_ui->forward);
		disconnect(web_ui->refresh);
		connect(web_ui->back, &QPushButton::clicked, qobject_cast<MyWebEngineView*>(webContain->widget(index)), &QWebEngineView::back);
		connect(web_ui->forward, &QPushButton::clicked, qobject_cast<MyWebEngineView*>(webContain->widget(index)), &QWebEngineView::forward);
		connect(web_ui->refresh, &QPushButton::clicked, qobject_cast<MyWebEngineView*>(webContain->widget(index)), &QWebEngineView::reload);
		connect(web_ui->stop, &QPushButton::clicked, qobject_cast<MyWebEngineView*>(webContain->widget(index)), &QWebEngineView::stop);
		
	}
	else {
		web_ui->URL->setText("");
		disconnect(web_ui->stop);
		disconnect(web_ui->back);
		disconnect(web_ui->forward);
		disconnect(web_ui->refresh);
	}
}

//创建新标签页并加载默认页面，连接相关信号槽
void WebWindow::newWeb() {
	int index = webContain->currentIndex();
	MyWebEngineView* web = new MyWebEngineView(NULL, QUrl(), g_profile);
	QWebEnginePage* page = new QWebEnginePage(g_profile, web);
	web->setPage(page);
	web->setUrl(QUrl("https://www.bilibili.com"));
	webContain->addTab(web, web->url().toString());
	connect(web, &MyWebEngineView::viewloaded, this, &WebWindow::updatetitle);
	connect(web, &MyWebEngineView::newViewCreated, this, &WebWindow::openWeb);
	connect(web->page(), &QWebEnginePage::linkHovered, this, &WebWindow::linkhover);
	webContain->setCurrentIndex(index + 1);
}

//在标签页中打开链接，更新标题和URL显示
void WebWindow::openWeb(MyWebEngineView* view) {
	int index = webContain->currentIndex();
	webContain->insertTab(index + 1, view, "laoding...");
	connect(view, &MyWebEngineView::newViewCreated, this, &WebWindow::openWeb);
	connect(view, &MyWebEngineView::viewloaded, this, &WebWindow::updatetitle);
	connect(view->page(), &QWebEnginePage::linkHovered, this, &WebWindow::linkhover);
	webContain->setCurrentIndex(index + 1);
}

//关闭窗口，停止加载所有页面并释放资源
void WebWindow::closeEvent(QCloseEvent* event) {
	for (int i = 0; i < webContain->count(); i++) {
		MyWebEngineView* web = qobject_cast<MyWebEngineView*>(webContain->widget(i));
		web->stop();
		web->deleteLater();
	}
	event->accept();
}

//更新标签页标题
void WebWindow::updatetitle(MyWebEngineView* view) {
	int index = webContain->indexOf(view);
	if (index != -1) {
		webContain->setTabText(index, view->title());
		if(index == webContain->currentIndex()) {
			updateURL(index);
		}
	}
}

//打开下载页面
void WebWindow::download() {
	emit(downloaded());
}

//捕捉URL
void WebWindow::linkhover(const QUrl& url) {
	linkhoverURL = url.toString();
}

//显示捕捉到的URL
void WebWindow::catchUrl() {
	if (linkhoverURL != "") {
		web_ui->detectoutput->appendPlainText(linkhoverURL);
	}
}

//关闭标签页，停止加载并释放资源
void WebWindow::closeWeb(int index) {
	QWidget* widget = webContain->widget(index);
	webContain->removeTab(index);
	MyWebEngineView* webView = qobject_cast<MyWebEngineView*>(widget);
	if (webView) {
		webView->stop();
		webView->deleteLater();
	}
}