
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

mainwindow::mainwindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::mainwindowClass), warnning_ui(new Ui::warnningwidget), warning_window(new QMainWindow)
{
    ui->setupUi(this);
    warnning_ui->setupUi(warning_window);
    setWindowTitle("demo1");
    setDir();
    //媒体播放器初始化
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_device = new QMediaDevices(this);
    m_player->setAudioOutput(m_audioOutput);
    m_player->setLoops(QMediaPlayer::Infinite);//默认单曲循环

    //进度条初始化
    ui->ProcessBarSlider->setRange(0, 0);
    ui->ProcessBarSlider->setTracking(true);  // 启用跟踪，拖动时实时更新

    //设置播放列表的上下文菜单
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    
    //信号槽的连接

    connect(ui->action, &QAction::triggered, this, &mainwindow::select);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &mainwindow::play);
    connect(ui->Forward, &QPushButton::clicked, this, &mainwindow::forwardItem);
    connect(ui->Next, &QPushButton::clicked, this, &mainwindow::nextItem);
    connect(warnning_ui->pushButton, &QPushButton::clicked, this, &mainwindow::cancel);
    connect(ui->PlayButton, &QPushButton::toggled, this, &mainwindow::buttonToggled);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &mainwindow::mediaStateChanged);
    connect(ui->ProcessBarSlider, &QSlider::sliderPressed, this, &mainwindow::sliderPressed);
    connect(ui->ProcessBarSlider, &QSlider::sliderMoved, this, &mainwindow::sliderMoved);
    connect(ui->ProcessBarSlider, &QSlider::sliderReleased, this, &mainwindow::sliderReleased);
    connect(m_player, &QMediaPlayer::durationChanged, this, &mainwindow::totalDuration);
    connect(m_player, &QMediaPlayer::positionChanged, this, &mainwindow::setPosition);
    connect(m_player, &QMediaPlayer::positionChanged, this, &mainwindow::setPos);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &mainwindow::setPos);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &mainwindow::setPos);
    connect(m_device, &QMediaDevices::audioOutputsChanged, this, &mainwindow::changeDevice);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &mainwindow::modePlay);
    connect(ui->modeChange, &QPushButton::clicked, this, &mainwindow::modeChange);
    connect(ui->setbackground, &QAction::triggered, this, &mainwindow::selectBackground);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &mainwindow::playListMenu);

    //各样式表初始化
    //播放按钮
    ui->PlayButton->setFixedSize(50, 50); // 直径
    ui->PlayButton->setCheckable(true); // 可切换状态

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


    readItem();
    readBackgroundDir();
    update();
}

mainwindow::~mainwindow()
{
    delete ui;
    delete m_player;
    delete m_audioOutput;
    delete m_device;
    delete warnning_ui;
    delete warning_window;
}

void mainwindow::cancel() {
    warning_window->close();
}

//选择播放文件
void mainwindow::select() {
    QStringList files = QFileDialog::getOpenFileNames(
        this,                      
        "Select music files",
        QDir::homePath(),
        "MP3(*.mp3)"
    );
    QStringList copyFiles;
    for (int i = 0; i < files.count(); i++) {
        QString filePath = files.at(i);
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();
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
        "PNG(*.png),JPG(*.jpg)"
    );
    if (!files.isEmpty()) {
        QFileInfo fileInfo(files.at(0));
        QString filename = fileInfo.fileName();
        QFile::copy(files.at(0), BACKGROUNDDIR + '/' + filename);
        BackgroundDir = BACKGROUNDDIR + '/' + filename;
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
        m_player->pause();
        m_player->setSource(QUrl::fromLocalFile(filePath));
        m_player->play();
        highlight(index);
        playingItemIndex = index;
        item->setForeground(QColor(Qt::black));
    }
    else {
        warning_window->show();
        QApplication::beep();
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
                m_player->pause();
                m_player->setSource(QUrl::fromLocalFile(filePath));
                m_player->play();
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                warning_window->show();
                QApplication::beep();
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
                m_player->pause();
                m_player->setSource(QUrl::fromLocalFile(filePath));
                m_player->play();
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                warning_window->show();
                QApplication::beep();
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
                m_player->pause();
                m_player->setSource(QUrl::fromLocalFile(filePath));
                m_player->play();
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                warning_window->show();
                QApplication::beep();
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
                m_player->pause();
                m_player->setSource(QUrl::fromLocalFile(filePath));
                m_player->play();
                highlight(index);
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::black));
            }
            else {
                warning_window->show();
                QApplication::beep();
                ui->listWidget->item(playingItemIndex)->setForeground(QColor(Qt::red));
                nextItem();
            }
        }
    }
    else {
        m_player->stop();
    }
}

//播放按钮绑定
void mainwindow::buttonToggled(bool checked) {
    if (playingItemIndex != -1) {
        if (checked) {
            //未指定媒体源初始化为第一个
            if (m_player->mediaStatus() == QMediaPlayer::NoMedia()) {
                m_player->setSource(QUrl::fromLocalFile(playList.at(0)));
                highlight(0);
                playingItemIndex = 0;
            }
            m_player->play();
        }
        else {
            m_player->pause();
        }
    }
}
void mainwindow::mediaStateChanged(QMediaPlayer::PlaybackState state) {
    switch (state)
    {
    case QMediaPlayer::StoppedState:
        ui->PlayButton->setChecked(0);
        m_player->setPosition(0);
        m_player->pause();
        break;
    case QMediaPlayer::PlayingState:
        ui->PlayButton->setChecked(1);
        break;
    case QMediaPlayer::PausedState:
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
    m_player->setPosition(value);
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
    if (m_player->isPlaying()&& !isseeking) {
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
void mainwindow::setPos() {

    if (m_player->playbackState()==QMediaPlayer::PlayingState && m_player->mediaStatus() == QMediaPlayer::BufferedMedia && !(m_player->position() == qint64(0)))
    {
        pos = m_player->position();
        isplaying = true;
    }
    else if (m_player->playbackState() == QMediaPlayer::PausedState && m_player->mediaStatus() == QMediaPlayer::BufferedMedia && !(m_player->position() == qint64(0)))
    {
        pos = m_player->position();
        isplaying = false;
    }
}
void mainwindow::changeDevice() {
    qDebug() << "11111111111111";
    m_player->stop();
    m_audioOutput->setDevice(m_device->defaultAudioOutput());
    m_player->setSource(playList.at(playingItemIndex));
    while (!m_player->isSeekable() || !m_player->mediaStatus()==QMediaPlayer::LoadedMedia) {
    }
    m_player->setPosition(pos);
    if (isplaying) {
        m_player->play();
    }
    else {
        m_player->pause();
    }
}

//播放模式切换
void mainwindow::modePlay() {
    if (m_player->mediaStatus() == QMediaPlayer::EndOfMedia) {
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
                m_player->stop();
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
        m_player->setLoops(QMediaPlayer::Infinite);
        ic.addFile(":/mainwindow/mode1.svg", QSize(30, 30), QIcon::Normal);
        ui->modeChange->setIcon(ic);
        ui->modeChange->setIconSize(QSize(30, 30));
        break;
    case 2:
        m_player->setLoops(1);
        ic.addFile(":/mainwindow/mode2.svg", QSize(30, 30), QIcon::Normal);
        ui->modeChange->setIcon(ic);
        ui->modeChange->setIconSize(QSize(30, 30));
        break;
    case 3:
        m_player->setLoops(1);
        ic.addFile(":/mainwindow/mode3.svg", QSize(30, 30), QIcon::Normal);
        ui->modeChange->setIcon(ic);
        ui->modeChange->setIconSize(QSize(30, 30));
        break;
    case 4:
        m_player->setLoops(1);
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
    QAction* deleteAction = menu.addAction("\345\210\240\351\231\244");
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
                    writeItem();
                }
                else  {
                    if (ui->listWidget->count() > 1) {
                        m_player->stop();
                        qDebug() << "11111";
                        delete ui->listWidget->takeItem(row);
                        if (row == ui->listWidget->count()) {
                            qDebug() << "11111";
                            playingItemIndex -= 1;
                            m_player->setSource(playList.at(playingItemIndex));
                        }
                        else {
                            m_player->setSource(playList.at(playingItemIndex));
                        }
                        highlight(playingItemIndex);
                        m_player->play();
                        QFile::remove(playList.at(row));
                        playList.removeAt(row);
                        writeItem();
                    }
                    else {
                        playingItemIndex = -1;
                        m_player->stop();
                        delete ui->listWidget->takeItem(row);
                        m_player->setSource(QString(""));
                        QFile::remove(playList.at(row));
                        playList.removeAt(row);
                        writeItem();
                    }
                }
            }
        }
    }
}