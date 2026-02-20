#pragma once


#include <QtWidgets/QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QDateTime>
#include <QFileInfo>

#include "ui_mainwindow.h"
#include "ui_warnning.h"
#include "ui_web.h"

#include "QWarningWindow.h"
#include "MyWebEngineView.h"
#include "WebContain.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
    void setDir();
    void writeItem();
    void readItem();
    void highlight(int index);
    void readBackgroundDir();
    void writeBackgroundDir();

protected:
    void paintEvent(QPaintEvent* event) override;//背景重绘

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
    void setPos();
    //进度条拖动
    void sliderPressed();
    void sliderMoved(qint64 value);
    void sliderReleased();
    //播放按钮与播放状态绑定
    void buttonToggled(bool checked);
    void mediaStateChanged(QMediaPlayer::PlaybackState state);
    //播放时长和进度条与播放进度绑定
    void totalDuration(qint64 duration);
    void setPosition(qint64 position);
    //错误窗口关闭
    void cancel();
    //播放模式切换
    void modeChange();
    void modePlay();
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
	//void refreshItem();

    //网络浏览器槽函数
private slots:
    void webinit();
	void updateURL(int index);
    void newWeb();
	
	//ui指针和窗口指针
private:
    Ui::mainwindowClass *ui;
    Ui::warnningwidget* warnning_ui;
	Ui::webWindow* web_ui;
private:
    QWarningWindow* warning_window;
	QMainWindow* web_window;

	//页面相关变量
private:
    int playingItemIndex = 0;
    int playMode = 1;
    qint64 pos;
    qint64 soundValue = 100;
    bool isplaying = 0;
    bool isseeking = 0;//是否正在拖到进度条
    bool ismute = 0;
    QPixmap background;
    QStringList playList = {};
    QString BackgroundDir;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    QMediaDevices* m_device;

    //自定义控件实现
private:
	WebContain* webContain;
};

