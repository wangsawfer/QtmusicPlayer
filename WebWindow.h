#pragma once

#include <QMainWindow>
#include "ui_web.h"
#include "MyWebEngineView.h"
#include "WebContain.h"


class WebWindow  : public QMainWindow
{
	Q_OBJECT

public:
	WebWindow(QWidget *parent = nullptr);
	~WebWindow();
public:
	Ui::webWindow* web_ui;
	WebContain* webContain;

public:
	void closeEvent(QCloseEvent* event) override;

public slots:
	void updateURL(int index);
	void updatetitle(MyWebEngineView* view);
	void newWeb();
	void openWeb(MyWebEngineView* view);
	void download();
	void linkhover(const QUrl& url);
	void catchUrl();
signals:
	void downloaded();
public:
	QString linkhoverURL = {};
};

