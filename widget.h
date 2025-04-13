#ifndef WIDGET_H
#define WIDGET_H

#include "Chat.h"
#include <QWidget>
#include <QTimer>

class TextBubble;
class ChatItemBase;
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

    void dealMessageTime(QString curMsgTime);

private slots:
    void on_send_btn_clicked();

    void getAnswer(const QString &word);

    void onStreamFinished();

private:
    Ui::Widget *ui;
    Chat m_deepSeek;

    /*--------------------*/
    TextBubble* m_currentResponseBubble = nullptr; // 新增：当前正在更新的回答气泡
    ChatItemBase* m_currentResponseItem = nullptr; // 对应的聊天项
};
#endif // WIDGET_H
