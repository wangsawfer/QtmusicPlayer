#pragma once

#include <QMainWindow>
#include "ui_warnning.h"

class QWarningWindow  : public QMainWindow
{
	Q_OBJECT

public:
	QWarningWindow(QWidget* parent = nullptr, QString message="");
	~QWarningWindow();
	Ui::warnningwidget* warnning_ui;

public slots:
	void cancel();
};

