#include "widget.h"
#include "ui_widget.h"
#include "TextBubble.h"
#include "ChatItemBase.h"

#include <QMouseEvent>
#include <QFile>
#include <qfontdatabase.h>
#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/deepseek.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    int id = QFontDatabase::addApplicationFont(GET_CURRENT_DIR + QStringLiteral("/dialog.ttf"));
    if (id != -1) {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont font(family);
        font.setPointSize(12);
        // 可选：设置为全局字体
        QApplication::setFont(font);
    } else {
        qDebug() << "Failed to load font.";
    }
    // 连接信号
    connect(&m_deepSeek, &Chat::answered, this, &Widget::getAnswer);
    connect(&m_deepSeek, &Chat::streamFinished, this, &Widget::onStreamFinished);
    connect(&m_deepSeek, &Chat::errorOccurred, this, [this](const QString& err){

        onStreamFinished();
    });

}

Widget::~Widget()
{
    delete ui;

}

QPixmap Widget::getRoundedPixmap(const QPixmap &src, const QSize &size, const int &radius) {
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);

    return dest;
}

void Widget::dealMessageTime(QString curMsgTime) {
    bool isShowTime = false;
    auto count = ui->chatView->getLayout()->count();
    qDebug()<<"count: "<<count;
    if(count > 1) {
        auto lastWidget = static_cast<TextBubble*>(ui->chatView->getLayout()->itemAt(count - 1)->widget());
        int lastTime = lastWidget->time().toInt();
        int curTime = curMsgTime.toInt();
        //qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = (curTime - lastTime) > 5; // 两个消息相差一分钟
        //        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        auto itemTime = new ChatItemBase(ChatRole::Time);
        auto messageTime = new TextBubble(ChatRole::Time,"",curMsgTime,ui->chatView);
        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setWidget(messageTime);
        ui->chatView->appendChatItem(itemTime);
    }
}

void Widget::on_send_btn_clicked() {
    const QString question = ui->question_textEdit->toPlainText().trimmed();
    if(question.isEmpty()) {
        qWarning() << "Empty question";
        return;
    }
    ui->send_btn->setEnabled(false);
    ui->send_btn->setCursor(Qt::ForbiddenCursor);
    //处理时间
    auto t = QString::number(QDateTime::currentSecsSinceEpoch());
    dealMessageTime(t);
    // 自己
    auto pChatItem = new ChatItemBase(ChatRole::Self);
    pChatItem->setUserName("我");
    pChatItem->setUserIcon(getRoundedPixmap(QPixmap(GET_CURRENT_DIR + QStringLiteral("/me.jpg")),{50,50},25));
    auto pBubble = new TextBubble(ChatRole::Self, question,t);
    pChatItem->setWidget(pBubble);
    ui->chatView->appendChatItem(pChatItem);
    // DeepSeek
    // 创建回答气泡
    m_currentResponseItem = new ChatItemBase(ChatRole::Other);
    m_currentResponseItem->setUserName("DeepSeek");
    m_currentResponseItem->setUserIcon(getRoundedPixmap(
        QPixmap(GET_CURRENT_DIR + QStringLiteral("/deepseek.png")).scaled(50,50), {50,50}, 25));

    m_currentResponseBubble = new TextBubble(ChatRole::Other, "",t);
    m_currentResponseBubble->startStreaming(); // 启动流式
    m_currentResponseItem->setWidget(m_currentResponseBubble);

    ui->chatView->appendChatItem(m_currentResponseItem);

    // 发送请求
    m_deepSeek.send(question);
    ui->question_textEdit->clear();
}

void Widget::getAnswer(const QString& chunk) {
    if (m_currentResponseBubble) {
        m_currentResponseBubble->appendStreamingContent(chunk); // 直接传递给气泡
    }
}

void Widget::onStreamFinished()
{
    if (m_currentResponseBubble) {
        m_currentResponseBubble->finishStreaming(); // 结束流式
    }
    ui->send_btn->setEnabled(true);
    ui->send_btn->setCursor(Qt::PointingHandCursor);
}