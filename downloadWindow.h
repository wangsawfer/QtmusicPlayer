#pragma once
#include <QProcess>
#include <QMainWindow>
#include "ui_download.h"
#include "QWarningWindow.h"


class downloadWindow  : public QMainWindow
{
	Q_OBJECT

public:
	downloadWindow(QWidget *parent = nullptr);
	~downloadWindow();
public:
	Ui::downloadpage* ui;
	QProcess* process;
public slots:
	void startDownload();
};

