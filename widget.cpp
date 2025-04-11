#include "widget.h"
#include "ui_widget.h"

#include <QScrollBar>
#include <QMouseEvent>
#include <QFile>

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

void Widget::on_send_btn_clicked() {
    const QString question = ui->question_textEdit->toPlainText().trimmed();
    if(question.isEmpty()) {
        qWarning() << "Empty question";
        return;
    }
    ui->send_btn->setEnabled(false);
    ui->send_btn->setCursor(Qt::ForbiddenCursor);

    // 发送请求
    m_deepSeek.send(question);
    ui->question_textEdit->clear();
}

void Widget::getAnswer(const QString& chunk) {
    m_pendingContent += chunk;
    if (!m_streamUpdateTimer.isActive()) {
        m_streamUpdateTimer.start();
    }
}

void Widget::onStreamFinished()
{

    qDebug()<<"回答完毕";
}