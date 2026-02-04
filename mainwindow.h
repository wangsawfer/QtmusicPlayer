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
private:
    Ui::mainwindowClass *ui;
    Ui::warnningwidget* warnning_ui;
private:
    QMainWindow* warning_window;
private:
    int playingItemIndex = 0;
    int playMode = 1;
    qint64 pos;
    bool isplaying = 0;
    bool isseeking = 0;//是否正在拖到进度条
    QPixmap background;
    QStringList playList = {};
    QString BackgroundDir;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    QMediaDevices* m_device;
};

