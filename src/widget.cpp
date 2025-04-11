#include "widget.h"
#include "ui_widget.h"

#include <QScrollBar>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->answer_textEdit->setStyleSheet(R"(
        QTextEdit {
            background-color: #1E1E1E;
            color: #D4D4D4;
            font-family: "Consolas";
            border: 1px solid #3E3E3E;
            border-radius: 5px;
            padding: 10px;
        }
        /*-------------------滚动区域滚动条----------------------*/
        QScrollBar:vertical {
            border: 0px solid rgb(231,166,190);
            border-radius:4px;
            background-color: white;
            width: 10px;
            margin: 20px 0px 20px 0px;
        }
        QScrollBar:vertical:hover {
            border: 1px solid rgb(231,166,190);
            border-radius:4px;
            background-color: rgb(231,166,190);
            width: 10px;
            margin: 20px 0px 20px 0px;
        }
        /*--滑块---*/
        QScrollBar::handle:vertical {
            background-color: rgb(237,188,206);
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: rgb(248,129,161);
        }
        /*---箭头---*/
        QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {
            background: none; /* 确保没有箭头 */
        }
        /*---空白区域---*/
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none; /* 防止长按时的背景色 */
        }
        QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical {
            background: none;
        }
    )");

    // 初始化文本框
    ui->answer_textEdit->setReadOnly(true);
    ui->answer_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);

    // 连接信号
    connect(&m_deepSeek, &Chat::answered, this, &Widget::getAnswer);
    connect(&m_deepSeek, &Chat::streamFinished, this, &Widget::onStreamFinished);
    connect(&m_deepSeek, &Chat::errorOccurred, this, [this](const QString& err){
        ui->answer_textEdit->append("\n[错误] " + err);
    });
}

Widget::~Widget()
{
    delete ui;

}
void Widget::on_pushButton_clicked()
{
    // 开始新会话时重置状态
    m_fullAnswer.clear();
    ui->answer_textEdit->clear();

    const auto question = ui->question_textEdit->toPlainText();

    //ui->question_textEdit->clear();可选

    if (!question.isEmpty())m_deepSeek.send(question);
    else {
        qWarning()<<"询问为空";
    }
}

void Widget::getAnswer(const QString& word) const {
    // 追加内容并自动滚动
    QTextCursor cursor = ui->answer_textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(word);
    // 自动滚动到底部
    QScrollBar *scrollbar = ui->answer_textEdit->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}

void Widget::onStreamFinished()
{
    // 重置状态
    m_fullAnswer.clear();
}