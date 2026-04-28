#include "stdafx.h"
#include "selectBackgroundwindow.h"

selectBackgroundwindow::selectBackgroundwindow(const QPixmap& pixmap,QWidget *parent,double scale)
	: QMainWindow(parent), ui(new Ui::selectBackgroundClass()), scale(scale)
{
	ui->setupUi(this);
	scene = new QGraphicsScene(this);
	ui->pictureContainer->setScene(scene);
	int w = pixmap.width();
	int h = pixmap.height();

	QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
	ui->pictureContainer->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	QScreen* screen = QApplication::primaryScreen();
	QRect screenGeometry = screen->availableGeometry();
	QSize maxSize = screenGeometry.size() * 0.8; // 窗口最大为屏幕80%

	QSize imageSize = pixmap.size();
	QSize viewSize = imageSize;
	if (viewSize.width() > maxSize.width() || viewSize.height() > maxSize.height()) {
		viewSize.scale(maxSize, Qt::KeepAspectRatio);
	}

	// 5. 设置视图的固定大小
	ui->pictureContainer->setFixedSize(viewSize);

	QTimer::singleShot(0, this, [this]() {
		ui->pictureContainer->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
		});

	adjustSize();
	setFixedSize(size());   // 固定窗口大小
	slideRect* rect = new slideRect(ui->pictureContainer,scale, this->size());
	scene->addItem(rect);

}

selectBackgroundwindow::~selectBackgroundwindow()
{}

