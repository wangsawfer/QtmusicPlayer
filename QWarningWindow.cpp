#include "stdafx.h"
#include "QWarningWindow.h"

QWarningWindow::QWarningWindow(QWidget *parent,QString message)
	: QMainWindow(parent), warnning_ui(new Ui::warnningwidget)
{
	warnning_ui->setupUi(this);
	warnning_ui->lineEdit->setText(message);
	connect(warnning_ui->pushButton, &QPushButton::clicked, this, &QWarningWindow::cancel);
	this->show();
	QApplication::beep();
}

QWarningWindow::~QWarningWindow()
{
	delete warnning_ui;
}

void QWarningWindow::cancel() {
	this->close();
}