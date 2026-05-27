#pragma once


#include <QRemoteObjectNode>

#include "stdafx.h"
#include "ui_mainwindow.h"


#include "QWarningWindow.h"
#include "WebWindow.h"
#include "downloadWindow.h"
#include "selectBackgroundwindow.h"
#include "media_controller_replica.h"


class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget* parent = nullptr);
    ~mainwindow();
    void setDir();
    void writeItem();
    void readItem();
    void highlight(int index);
    void readBackgroundDir();
    void writeBackgroundDir();

protected:
    void paintEvent(QPaintEvent* event) override;//背景重绘
    void closeEvent(QCloseEvent* event) override;
    //播放器槽函数
private slots:
    void select();//添加音乐文件
    //背景设置
    void selectBackground();
    void setBackgroud();
    void play(QListWidgetItem* item);//播放音乐
    void forwardItem();//上一首
    void nextItem();   //下一首
    //播放设备热重载
    void changeDevice();
    //进度条拖动
    void sliderPressed();
    void sliderMoved(qint64 value);
    void sliderReleased();
    //播放按钮与播放状态绑定
    void buttonToggled(bool checked);
    void mediaStateChanged(int state);
    //播放时长和进度条与播放进度绑定
    void totalDuration(qint64 duration);
    void setPosition(qint64 position);
    //播放模式切换
    void modeChange();
    void modePlay(int status);
    //播放列表右键菜单
    void playListMenu(const QPoint& pos);
    //快进与快退
    void Fast_forward();
    void rewind();
    //音量调节
    void soundSet();
    void soundSvgChange();
    void Mute();
    //刷新播放列表
    void refreshItem();
    void download();
private slots:
    //网络浏览器槽函数
    void webinit();
	void init_player();
    void initReplica();
public slots:
	void poschange(qint64 pos);
	void durchange(qint64 dur);
	void statechange(int state);
	void statuschange(int status);
	void soundChange(qreal sound);
	void loopsChange(int loops);
private:
    //ui指针和窗口指针
    Ui::mainwindowClass* ui;



    //页面相关变量
private:
    int playingItemIndex = -2;
    int playMode = 1;

    qint64 soundValue = 100;
    bool isplaying = 0;
    bool isseeking = 0;//是否正在拖到进度条
    bool ismute = 0;
    QPixmap background;
    QStringList playList = {};
    QString BackgroundDir;

    qint64 pos;
	qint64 dur;
	int state;
	int status;
    qreal sound;
	int loops;

    QMediaDevices* m_device;
    WebWindow* web_window;
    downloadWindow* download_page;
    //自定义控件实现
	QProcess* media_player;
    MediaControllerReplica* media_controller;
    QRemoteObjectNode m_node;
};

