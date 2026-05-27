#include "stdafx.h"
#include "mainwindow.h"

#define MUSICDIR QApplication::applicationDirPath() + "/music"
#define BACKGROUNDDIR QApplication::applicationDirPath() + "/background"

//创建目录
void mainwindow::setDir() {
    QDir dir;
    dir.setPath(MUSICDIR);
    if (!dir.exists()) {
        dir.mkdir(MUSICDIR);
    }
    dir.setPath(BACKGROUNDDIR);
    if (!dir.exists()) {
        dir.mkdir(BACKGROUNDDIR);
    }
}

//读取背景地址文件
void mainwindow::readBackgroundDir() {
    QFile file(BACKGROUNDDIR + "/backGround.txt");
    if (file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        if (!in.atEnd()) {
            QString Dir = in.readLine();
            BackgroundDir = Dir;
            setBackgroud();
        }
        else {
            return;
        }
    }
}

//写入背景地址文件
void mainwindow::writeBackgroundDir() {
    QFile file(BACKGROUNDDIR + "/backGround.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(BackgroundDir.toUtf8());
    file.close();
}

void mainwindow::writeItem() {
    QString playListDir = MUSICDIR + "/playList.txt";
    QFile file(playListDir);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    for (int i = 0; i < playList.size(); i++) {
        file.write((playList.at(i) + '\n').toUtf8());
    }
    file.close();
}

void mainwindow::readItem() {
    QString playListDir = MUSICDIR + "/playList.txt";
    QFile file(playListDir);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();      //整行读取
        playList << line;
        QFileInfo fileInfo(line);
        QString fileName = fileInfo.fileName();
        ui->listWidget->addItem(fileName);
    }
}

void mainwindow::highlight(int index) {
    //------------------------------高亮当前播放项---------------------------------
    // 清除所有项的高亮
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        ui->listWidget->item(i)->setBackground(QBrush());  // 恢复默认背景
    }
    // 高亮当前播放项
    if (index >= 0 && index < ui->listWidget->count()) {
        ui->listWidget->item(index)->setBackground(QColor(180, 220, 255, 100));  // 浅蓝色
    }
    //-----------------------------------------------------------------------------
}

void mainwindow::init_player() {
	media_player = new QProcess(this);
	QString playerPath = QCoreApplication::applicationDirPath() + "/musicPlayer.exe";
    media_player->start(playerPath);
    if (!media_player->waitForStarted()) {
        new QWarningWindow(nullptr, QString("\u627E\u4E0D\u5230\u64AD\u653E\u5668\uFF01"));
    }
}

void mainwindow::initReplica() {

    m_node.connectToNode(QUrl("local:musicplayer"));
	media_controller = m_node.acquire<MediaControllerReplica>();
    // 等待副本有效，最多等待 2 秒
    if (media_controller->waitForSource(2000)) {
        qDebug() << "连接成功！";
        // 可以连接信号或启用 UI 控件
    }
    else {
        qDebug() << "连接失败，服务端未运行";
        // 可以提示用户启动播放器，或自动启动服务端进程
    }
}

mainwindow::mainwindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::mainwindowClass)
{
    ui->setupUi(this);

	setWindowIcon(QIcon(":/mainwindow/icon.ico"));
    init_player();

    setWindowTitle("\345\245\266\351\276\231\346\222\255\346\224\276\345\231\250");
    setDir();
    //媒体播放器初始化

    m_device = new QMediaDevices(this);

    //进度条初始化
    ui->ProcessBarSlider->setRange(0, 0);
    ui->ProcessBarSlider->setTracking(true);  // 启用跟踪，拖动时实时更新
    ui->soundSlider->setRange(0, 100);
    ui->soundSlider->setTracking(true);
    ui->soundSlider->setValue(100);


    //设置播放列表的上下文菜单
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    
    //信号槽的连接
    initReplica();
    connect(ui->action, &QAction::triggered, this, &mainwindow::select);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &mainwindow::play);
    connect(ui->Forward, &QPushButton::clicked, this, &mainwindow::forwardItem);
    connect(ui->Next, &QPushButton::clicked, this, &mainwindow::nextItem);
    connect(ui->PlayButton, &QPushButton::toggled, this, &mainwindow::buttonToggled);
    connect(media_controller, &MediaControllerReplica::playbackStateChanged, this, &mainwindow::mediaStateChanged);
    connect(ui->ProcessBarSlider, &QSlider::sliderPressed, this, &mainwindow::sliderPressed);
    connect(ui->ProcessBarSlider, &QSlider::sliderMoved, this, &mainwindow::sliderMoved);
    connect(ui->ProcessBarSlider, &QSlider::sliderReleased, this, &mainwindow::sliderReleased);
    connect(media_controller, &MediaControllerReplica::durationChanged, this, &mainwindow::totalDuration);
    connect(media_controller, &MediaControllerReplica::positionChanged, this, &mainwindow::setPosition);
    connect(m_device, &QMediaDevices::audioOutputsChanged, this, &mainwindow::changeDevice);
    connect(media_controller, &MediaControllerReplica::mediaStatusChanged, this, &mainwindow::modePlay);

	connect(media_controller, &MediaControllerReplica::positionChanged, this, &mainwindow::poschange);
	connect(media_controller, &MediaControllerReplica::durationChanged, this, &mainwindow::durchange);
    connect(media_controller, &MediaControllerReplica::playbackStateChanged, this, &mainwindow::statechange);
	connect(media_controller, &MediaControllerReplica::mediaStatusChanged, this, &mainwindow::statuschange);
	connect(media_controller, &MediaControllerReplica::volumeChanged, this, &mainwindow::soundChange);
	connect(media_controller, &MediaControllerReplica::loopsChanged, this, &mainwindow::loopsChange);

    connect(ui->modeChange, &QPushButton::clicked, this, &mainwindow::modeChange);
    connect(ui->setbackground, &QAction::triggered, this, &mainwindow::selectBackground);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &mainwindow::playListMenu);
    connect(ui->left, &QPushButton::clicked, this, &mainwindow::rewind);
    connect(ui->right, &QPushButton::clicked, this, &mainwindow::Fast_forward);
    connect(ui->soundSlider, &QSlider::valueChanged, this, &mainwindow::soundSet);
    connect(ui->soundSlider, &QSlider::valueChanged, this, &mainwindow::soundSvgChange);
    connect(ui->soundChange, &QPushButton::clicked, this, &mainwindow::Mute);
    connect(ui->refresh, &QAction::triggered, this, &mainwindow::refreshItem);


    //网络相关实现
	connect(ui->webbotton, &QAction::triggered, this, &mainwindow::webinit);
    connect(ui->actiondownload, &QAction::triggered, this, &mainwindow::download);
    

    //各样式表初始化
    //播放按钮
    ui->PlayButton->setFixedSize(50, 50); // 直径
    ui->PlayButton->setCheckable(true); // 可切换状态
    ui->PlayButton->setShortcut(QKeySequence(tr("Space")));//快捷键

    QIcon icon1;
    icon1.addFile(":/mainwindow/play.svg", QSize(60, 60), QIcon::Normal, QIcon::Off);
    icon1.addFile(":/mainwindow/pause.svg", QSize(60, 60), QIcon::Normal, QIcon::On);
    ui->PlayButton->setIcon(icon1);
    ui->PlayButton->setIconSize(QSize(60, 60));
    //上一首
    QIcon icon2;
    icon2.addFile(":/mainwindow/front.svg", QSize(30, 30), QIcon::Normal);
    ui->Forward->setIcon(icon2);
    ui->Forward->setIconSize(QSize(30, 30));
    //下一首
    QIcon icon3;
    icon3.addFile(":/mainwindow/back.svg", QSize(30, 30), QIcon::Normal);
    ui->Next->setIcon(icon3);
    ui->Next->setIconSize(QSize(30, 30));
    //模式切换
    QIcon icon4;
    icon4.addFile(":/mainwindow/mode1.svg", QSize(30, 30), QIcon::Normal);
    ui->modeChange->setIcon(icon4);
    ui->modeChange->setIconSize(QSize(30, 30));
    //快进和快退
    ui->left->setIcon(QIcon(":/mainwindow/left.svg"));
    ui->right->setIcon(QIcon(":/mainwindow/right.svg"));
    ui->left->setIconSize(QSize(30, 30));
    ui->right->setIconSize(QSize(30, 30));
    ui->left->setShortcut(QKeySequence(tr("Left")));
    ui->right->setShortcut(QKeySequence(tr("Right")));
    //音量按钮
    ui->soundChange->setIcon(QIcon(":/mainwindow/sound3.svg"));
    ui->soundChange->setIconSize(QSize(30, 30));

    readItem();
    readBackgroundDir();
    update();

}

mainwindow::~mainwindow()
{
    delete ui;
	delete media_player;
    delete m_device;
	delete web_window;
    delete download_page;
	delete media_controller;
}

void mainwindow::closeEvent(QCloseEvent* event) {
    if (web_window != nullptr) {
        web_window->close();
    }
    if(download_page != nullptr) {
        download_page->close();
	}
    event->accept();
}
//选择播放文件
void mainwindow::select() {
    QStringList files = QFileDialog::getOpenFileNames(
        this,                      
        "Select music files",
        QDir::homePath(),
        "MP3 (*.mp3);;MP4 (*.mp4)"
    );
    QStringList copyFiles;
    for (int i = 0; i < files.count(); i++) {
        QString filePath = files.at(i);
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();
        if(ui->listWidget->findItems(fileName, Qt::MatchExactly).count() > 0) {
            continue;
		}
        copyFiles.append(MUSICDIR + "/" + fileName);
        QFile::copy(filePath, copyFiles.at(i));
        ui->listWidget->addItem(fileName);  
    }
    playList << copyFiles;
    writeItem();
}

//选择背景
void mainwindow::selectBackground() {
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Select background",
        QDir::homePath(),
        "PNG(*.png);;JPG(*.jpg)"
    );
    if (!files.isEmpty()) {
        QFileInfo fileInfo(files.at(0));
        QString filename = fileInfo.fileName();
        QFile::copy(files.at(0), BACKGROUNDDIR + '/' + filename);
        BackgroundDir = BACKGROUNDDIR + '/' + filename;

        QPixmap pix(BackgroundDir);

		double scale = static_cast<double>(this->size().height()) / static_cast<double>(this->size().width());
        selectBackgroundwindow* sel = new selectBackgroundwindow(pix,nullptr,scale);
        sel->show();

        writeBackgroundDir();
        setBackgroud();
    }
    else {
        return;
    }
}

//设置背景
void mainwindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!background.isNull()) {
        QPixmap scaled = background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        int x = (scaled.width() - width()) / 2;
        int y = (scaled.height() - height()) / 2;
        painter.drawPixmap(0, 0, scaled, x, y, width(), height());
    }
    else {
        //painter.fillRect(rect(), Qt::white);
        return;
    }
}

void mainwindow::setBackgroud() {
    background.load(BackgroundDir);
    update();
}

//双击播放
void mainwindow::play(QListWidgetItem *item) {
    int index = ui->listWidget->row(item);
    QString filePath = playList.at(index);
    QFileInfo fileinfo(filePath);
    if (fileinfo.exists()) {
        //m_player->pause();
        //m_player->setSource(QUrl::fromLocalFile(filePath));
        //qDebug() << filePath;
       // m_player->play();

		media_controller->setSource(filePath);
        QTimer::singleShot(500, this, [this]() {
            media_controller->play();
            });

        highlight(index);
        playingItemIndex = index;
        item->setForeground(QColor(Qt::black));
    }
    else {
        new QWarningWindow(nullptr, QString("文件不存在"));
        item->setForeground(QColor(Qt::red));
    }
}

//上一首
void mainwindow::forwardItem() {
    if (!playList.isEmpty()) {
        if (playingItemIndex > 0) {
            int index = playingItemIndex - 1;
            playingItemIndex = index;
            QString filePath = playList.at(index);
            QFileInfo fileinfo(filePath);
            if (fileinfo.exists()) {

                media_controller->setSource(filePath);
                QTimer::singleShot(500, this, [this]() {
                    media_controller->play();
                    });
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                new QWarningWindow(nullptr, QString("文件不存在"));
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::red));
                forwardItem();
            }
        }
        else {
            int index = ui->listWidget->count() - 1;
            playingItemIndex = index;
            QString filePath = playList.at(index);
            QFileInfo fileinfo(filePath);
            if (fileinfo.exists()) {

                media_controller->setSource(filePath);
                QTimer::singleShot(500, this, [this]() {
                    media_controller->play();
                    });
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                new QWarningWindow(nullptr, QString("文件不存在"));
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::red));
                forwardItem();
            }
        }
    }
}

//下一首
void mainwindow::nextItem(){
    if (!playList.isEmpty()) {
        if (playingItemIndex < (ui->listWidget->count() - 1)) {
            int index = playingItemIndex + 1;
            playingItemIndex = index;
            QString filePath = playList.at(index);
            QFileInfo fileinfo(filePath);
            if (fileinfo.exists()) {

                media_controller->setSource(filePath);
                QTimer::singleShot(500, this, [this]() {
                    media_controller->play();
                    });
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                new QWarningWindow(nullptr, QString("文件不存在"));
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::red));
                nextItem();
            }
        }
        else {
            int index = 0;
            playingItemIndex = index;
            QString filePath = playList.at(index);
            QFileInfo fileinfo(filePath);
            if (fileinfo.exists()) {

                media_controller->setSource(filePath);
                QTimer::singleShot(500, this, [this]() {
                    media_controller->play();
                    });
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                new QWarningWindow(nullptr, QString("文件不存在"));
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::red));
                nextItem();
            }
        }
    }
    else {
		media_controller->stop();
    }
}

//播放按钮绑定
void mainwindow::buttonToggled(bool checked) {
    if (playingItemIndex != -1) {
        if (checked && !playList.isEmpty()) {
            //未指定媒体源初始化为第一个
            if (playingItemIndex == -2) {
                media_controller->setSource(playList.at(0));
                highlight(0);
                QTimer::singleShot(500, this, [this]() {
                    media_controller->play();
                    });
                playingItemIndex = 0;
            }
			media_controller->play();
        }
        else {
            media_controller->pause();
        }
    }
}//
void mainwindow::mediaStateChanged(int state) {
    switch (state)
    {
    case 0:
        ui->PlayButton->setChecked(0);
        media_controller->pushPosition(0);
        media_controller->pause();
        break;
    case 1:
        ui->PlayButton->setChecked(1);
        break;
    case 2:
        ui->PlayButton->setChecked(0);
        break;
    default:
        break;
    }
}

//设置总时长和进度条最大值
void mainwindow::totalDuration(qint64 duration) {
    if (duration > 0) {
        QString time;

        //时间格式化   ms -> hh:mm:ss
        qint64 totalseconds = duration/1000;
        qint64 hours = totalseconds / 3600;
        qint64 minutes = (totalseconds % 3600) / 60;
        qint64 seconds = (totalseconds % 3600) % 60;
        time = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

        ui->duration->setText("/"+time);
        ui->ProcessBarSlider->setRange(0, duration);
    }
    else {
        ui->duration->setText("/00:00:00");
        ui->ProcessBarSlider->setRange(0, 0);
    }
}

//进度条拖动
void mainwindow::sliderPressed() {
    isseeking = true;
}
void mainwindow::sliderMoved(qint64 value) {
    QString time;
    //时间格式化   ms -> hh:mm:ss
    qint64 totalseconds = value / 1000;
    qint64 hours = totalseconds / 3600;
    qint64 minutes = (totalseconds % 3600) / 60;
    qint64 seconds = (totalseconds % 3600) % 60;
    time = QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
    ui->position->setText(time);
}
void mainwindow::sliderReleased() {
    qint64 value = ui->ProcessBarSlider->value();
    media_controller->pushPosition(value);
    isseeking = false;
    QString time;
    //时间格式化   ms -> hh:mm:ss
    qint64 totalseconds = value / 1000;
    qint64 hours = totalseconds / 3600;
    qint64 minutes = (totalseconds % 3600) / 60;
    qint64 seconds = (totalseconds % 3600) % 60;
    time = QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
    ui->position->setText(time);
}


//设置播放进度
void mainwindow::setPosition(qint64 position) {
    if (!isseeking) {
        QString time;
        //时间格式化   ms -> hh:mm:ss
        qint64 totalseconds = position / 1000;
        qint64 hours = totalseconds / 3600;
        qint64 minutes = (totalseconds % 3600) / 60;
        qint64 seconds = (totalseconds % 3600) % 60;
        time = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

        ui->position->setText(time);
        ui->ProcessBarSlider->setValue(position);
    }
    else {
        return;
    }
}

//输出设备热插拔处理   初步解决

void mainwindow::changeDevice() {
    if (media_player) {
        media_player->kill();
        delete media_player;
    }
	init_player();    
    QTimer::singleShot(100, this, [this]() {
        initReplica();
        media_controller->setSource(playList.at(playingItemIndex));
        QTimer::singleShot(500, this, [this]() {
            media_controller->play();
			media_controller->pushVolume(sound);
			media_controller->pushPosition(pos);
			media_controller->pushLoops(loops);
			});
        });
    
}

//播放模式切换
void mainwindow::modePlay(int status) {
    if (status == QMediaPlayer::EndOfMedia) {
        switch (playMode)
        {
        case 1: {
            break;
        }
        case 2: {
            nextItem();
            break;
        }
        case 3: {
            if (playingItemIndex < ui->listWidget->count() - 1) {
                nextItem();
            }
            else {
                media_controller->stop();
            }
            break;
        }
        case 4: {
            int index = QRandomGenerator::global()->bounded(0, ui->listWidget->count());
            play(ui->listWidget->item(index));
            break;
        }
        default:
            break;
        }
    }
}
void mainwindow::modeChange() {
    if (playMode != 4) {
        playMode += 1;
    }
    else {
        playMode = 1;
    }
    QIcon ic;
    switch (playMode)
    {
    case 1:
        media_controller->pushLoops(QMediaPlayer::Infinite);
        ic.addFile(":/mainwindow/mode1.svg", QSize(30, 30), QIcon::Normal);
        ui->modeChange->setIcon(ic);
        ui->modeChange->setIconSize(QSize(30, 30));
        break;
    case 2:
        media_controller->pushLoops(1);
        ic.addFile(":/mainwindow/mode2.svg", QSize(30, 30), QIcon::Normal);
        ui->modeChange->setIcon(ic);
        ui->modeChange->setIconSize(QSize(30, 30));
        break;
    case 3:
        media_controller->pushLoops(1);
        ic.addFile(":/mainwindow/mode3.svg", QSize(30, 30), QIcon::Normal);
        ui->modeChange->setIcon(ic);
        ui->modeChange->setIconSize(QSize(30, 30));
        break;
    case 4:
        media_controller->pushLoops(1);
        ic.addFile(":/mainwindow/mode4.svg", QSize(30, 30), QIcon::Normal);
        ui->modeChange->setIcon(ic);
        ui->modeChange->setIconSize(QSize(30, 30));
        break;
    default:
        break;
    }
}

//播放列表右键菜单
void mainwindow::playListMenu(const QPoint &pos) {
    QMenu menu;
    QAction* deleteAction = menu.addAction("\345\210\240\351\231\244\n\n\n\n\n\n\n\n\n\n\n");
    deleteAction->setShortcut(QKeySequence::Delete);
    if (ui->listWidget->selectedItems().isEmpty()) {
        deleteAction->setEnabled(false);
    }

    QAction* selectedAction = menu.exec(ui->listWidget->viewport()->mapToGlobal(pos));
    if (selectedAction == deleteAction) {
        QList<QListWidgetItem*> selectItems = ui->listWidget->selectedItems();
        if (!selectItems.isEmpty()) {
            foreach(QListWidgetItem * Item, selectItems) {
                int row = ui->listWidget->row(Item);
                if (row != playingItemIndex) {
                    QFile::remove(playList.at(row));
                    playList.removeAt(row);
                    delete ui->listWidget->takeItem(row);
					playingItemIndex = ui->listWidget->row(ui->listWidget->currentItem());
                    writeItem();
                }
                else  {
                    if (ui->listWidget->count() > 1) {
						media_controller->stop();
                        delete ui->listWidget->takeItem(row);
                        if (row == ui->listWidget->count()) {
                            playingItemIndex -= 1;
							media_controller->setSource(playList.at(playingItemIndex));
                        }
                        else {
                            media_controller->setSource(playList.at(playingItemIndex));
                        }
                        highlight(playingItemIndex);
                        QFile::remove(playList.at(row));
                        playList.removeAt(row);
                        QTimer::singleShot(500, this, [this]() {
                            media_controller->play();
                            });
                        writeItem();
                    }
                    else {
                        playingItemIndex = -1;
                        media_controller->stop();
                        delete ui->listWidget->takeItem(row);
                        media_controller->setSource("");
                        QFile::remove(playList.at(row));
                        playList.removeAt(row);
                        writeItem();
                    }
                }
            }
        }
    }
}

//快进和快退
void mainwindow::rewind() {
    int step = ui->stepedit->text().toInt();
    if ((pos - step >= 0) && step >= 0) {
        media_controller->pushPosition(pos - step);
    }
}
void mainwindow::Fast_forward() {
    int step = ui->stepedit->text().toInt();
    if (pos + step <= dur && step >= 0) {
        media_controller->pushPosition(pos + step);
    }
}

//音量调节
void mainwindow::soundSet() {
    qint64 value = ui->soundSlider->value();

    if (value == 0) {
        ismute = 1;
    }
    else {
        ismute = 0;
    }
    ui->soundLabel->setText(QString::number(value));
	media_controller->pushVolume(value / 100.0);
}
void mainwindow::soundSvgChange() {
    int value = ui->soundSlider->value();
    if (value == 0) {
        ui->soundChange->setIcon(QIcon(":/mainwindow/sound0.svg"));
        ui->soundChange->setIconSize(QSize(30, 30));
    }
    else if (value > 0 && value <= 33) {
        ui->soundChange->setIcon(QIcon(":/mainwindow/sound1.svg"));
        ui->soundChange->setIconSize(QSize(30, 30));
    }
    else if (value > 33 && value <= 66) {
        ui->soundChange->setIcon(QIcon(":/mainwindow/sound2.svg"));
        ui->soundChange->setIconSize(QSize(30, 30));
    }
    else if (value > 66 && value <= 100) {
        ui->soundChange->setIcon(QIcon(":/mainwindow/sound3.svg"));
        ui->soundChange->setIconSize(QSize(30, 30));
    }
}
void mainwindow::Mute() {
    if (!ismute) {
        soundValue = ui->soundSlider->value();
        ui->soundSlider->setValue(0);
        media_controller->pushVolume(0);
        ismute = 1;
    }
    else {
        ui->soundSlider->setValue(soundValue);
        media_controller->pushVolume(soundValue / 100.0);
        ismute = 0;
    }
}

void mainwindow::refreshItem() {
    QString directoryPath = MUSICDIR;
    QDir directory(directoryPath);

    QStringList nameFilters;
	nameFilters << "*.mp3" << "*.mp4";//设置过滤器，指定要查找的文件类型
    QStringList files = directory.entryList(nameFilters, QDir::Files);

	//完整路径列表
    QStringList fullPaths;
    for (const QString& fileName : files) {
        if (!playList.contains(directory.absoluteFilePath(fileName))) {
            playList << directory.absoluteFilePath(fileName);
			ui->listWidget->addItem(fileName);
		}
    }
    writeItem();
}

void mainwindow::poschange(qint64 pos) {
	this->pos = pos;
	qDebug() << "Position changed: " << pos;
}
void mainwindow::durchange(qint64 dur) {
	this->dur = dur;
	qDebug() << "Duration changed: " << dur;
}
void mainwindow::statechange(int state) {
	this->state = state;
	qDebug() << "State changed: " << state;
}
void mainwindow::statuschange(int status) {
	this->status = status;
	qDebug() << "Status changed: " << status;
}
void mainwindow::soundChange(qreal sound) {
	this->sound = sound;
	qDebug() << "Sound changed: " << sound;
}
void mainwindow::loopsChange(int loops) {
    this->loops = loops;
	qDebug() << "Loops changed: " << loops;
}