#include "stdafx.h"
#include "downloadWindow.h"

downloadWindow::downloadWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::downloadpage), process(new QProcess(this))
{
	ui->setupUi(this);
	connect(process,&QProcess::readyReadStandardOutput, [this]() {
		QString output = process->readAllStandardOutput();
		ui->output->insertPlainText(output);
		});

	connect(process, &QProcess::readyReadStandardError, [this]() {
		QString errorOutput = process->readAllStandardError();
		ui->output->insertPlainText(errorOutput);
		});

	connect(ui->start, &QPushButton::clicked, this, &downloadWindow::startDownload);
}

downloadWindow::~downloadWindow()
{
	delete ui;
	if (process->state() == QProcess::Running) {
        process->terminate();
        process->waitForFinished(3000);
    }
	delete process;
}

void downloadWindow::startDownload() {
	QString luxPath = QCoreApplication::applicationDirPath() + "/lux.exe";
	QFileInfo luxCheck(luxPath);
	if (!luxCheck.exists()) {
		new QWarningWindow(nullptr, QString("lux.exe不存在"));
		return;
	}

	//设置环境变量
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();;
	QString ffmpegDir = QCoreApplication::applicationDirPath() + "/ffmpeg/bin";
	QString path = env.value("PATH");
	env.insert("PATH", ffmpegDir + ";" + path);
	process->setProcessEnvironment(env);

	//构建运行参数
	QStringList arg;
	QStringList videoUrlList = ui->input->toPlainText().split("\n", Qt::SkipEmptyParts);
	QStringList downloadoption;
	QStringList showOption;
	QStringList threadOption;
	QStringList qualityOption;

	switch (ui->downloadoption->currentIndex())
	{
	case 0:
		break;
	case 1:
		downloadoption << "-p";
		break;
	default:
		break;
	}

	switch (ui->showitem->currentIndex())
	{
	case 0:
		break;
	case 1:
		showOption << "-i";
		break;
	default:
		break;
	}

	switch (ui->thread->currentIndex())
	{
	case 0:
		break;
	case 1:
		threadOption << "-n" << "1";
		break;
	case 2:
		threadOption << "-n" << "4";
		break;
	case 3:
		threadOption << "-n" << "8";
		break;
	case 4:
		threadOption << "-n" << "16";
		break;
	case 5:
		threadOption << "-n" << "32";
		break;
	default:
		break;
	}

	switch (ui->selquality->currentIndex())
	{
	case 0:
		break;
	case 1:
		qualityOption << "-f" << ui->quality->text();
		break;
	default:
		break;
	}
	arg << showOption << "-o" << QCoreApplication::applicationDirPath() + "/music" << downloadoption << threadOption << qualityOption << videoUrlList;
	process->start(luxPath, arg);
}