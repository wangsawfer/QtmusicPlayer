#pragma once

#include <QMainWindow>

class QWarningWindow  : public QMainWindow
{
	Q_OBJECT

public:
	QWarningWindow(QWidget *parent);
	~QWarningWindow();
};

