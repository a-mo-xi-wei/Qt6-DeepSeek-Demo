#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Chat.h"

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

private slots:
    void on_pushButton_clicked();

    void getAnswer(const QString &word) const;

    void onStreamFinished();

private:
    Ui::Widget *ui;
    Chat m_deepSeek;

    QString m_fullAnswer;  // 新增：累积完整回答
};
#endif // WIDGET_H
