#ifndef WIDGET_H
#define WIDGET_H

#include "Chat.h"
#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE

namespace Ui {
    class Widget;
}

QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private:
    QPixmap getRoundedPixmap(const QPixmap &src, const QSize &size, const int &radius);

private slots:
    void on_send_btn_clicked();

    void getAnswer(const QString &word);

    void onStreamFinished();

private:
    Ui::Widget *ui;
    Chat m_deepSeek;

    /*--------------------*/
    QTimer m_streamUpdateTimer; // 流式更新定时器
    QString m_pendingContent; // 待显示内容缓冲
};
#endif // WIDGET_H
