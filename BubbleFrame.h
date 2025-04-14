#ifndef BUBBLE_H
#define BUBBLE_H

#include <QFrame>
#include "global.h"
#include <QHBoxLayout>

class BubbleFrame : public QFrame {
    Q_OBJECT

public:
    explicit BubbleFrame(ChatRole role, const QString &time, QWidget *parent = nullptr);

    void setWidget(QWidget *w);

    QString time() {return m_time;}

protected:
    void paintEvent(QPaintEvent *e) override;

protected://给子类继承
    QString m_time;//时间戳

private:
    QHBoxLayout *m_pHLayout;
    ChatRole m_role;
    int m_margin;
    QString m_createTime;
};

#endif // BUBBLE_H
