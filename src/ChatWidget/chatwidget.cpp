#include "chatwidget.h"
#include "ui_chatwidget.h"

#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QWebFrame>
#include <QWebElement>
#include <QScrollBar>

ChatWidget::ChatWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    this->layout()->setContentsMargins(0,0,0,0);
    this->ui->chatWindow->settings()->setUserStyleSheetUrl(QUrl("qrc:/ChatWindow/style.css"));
    this->ui->chatWindow->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);//Handle link clicks by yourself
    this->ui->chatWindow->setContextMenuPolicy(Qt::NoContextMenu);

    QObject::connect(this->ui->chatWindow,
                     &QWebView::linkClicked,
                     this,
                     &ChatWidget::linkClicked);
    QObject::connect(this->ui->chatWindow->page()->mainFrame(),
                     &QWebFrame::contentsSizeChanged,
                     this,
                     &ChatWidget::chatContentsSizeChanged);
}

void ChatWidget::addMessage(Message *msg) {
    this->ui->chatWindow->page()->mainFrame()->documentElement().findFirst("body").appendInside(QString("%1\n").arg(msg->raw_message));
}

void ChatWidget::linkClicked(const QUrl &url){
    QDesktopServices::openUrl(url);
}

void ChatWidget::updateMessageScreen(QList<Message*> *messages){
    QWebFrame *frame = this->ui->chatWindow->page()->mainFrame();
    this->scrollValue = frame->scrollBarValue(Qt::Vertical);
    this->autoScroll = (this->scrollValue >= frame->scrollBarMaximum(Qt::Vertical));
    if (this->autoScroll) {
        while (messages->count() > messageCount) {
            delete messages->takeFirst();
            QWebElement message = frame->findFirstElement("div.message");
            message.removeFromDocument();
        }
    }
}

void ChatWidget::channelChanged(QList<Message*> *messages) {
    QWebFrame* frame = this->ui->chatWindow->page()->mainFrame();
    frame->setHtml("");

    for (int i = 0; i < messages->count(); ++i)
        addMessage(messages->at(i));

    this->scrollValue = 0;
    this->autoScroll = true;
}


ChatWidget::~ChatWidget() {
    delete ui;
}

void ChatWidget::chatContentsSizeChanged(const QSize &size) {
    QWebFrame* frame = this->ui->chatWindow->page()->mainFrame();
    if (this->autoScroll)
        frame->setScrollBarValue(Qt::Vertical, size.height());
    else
        frame->setScrollBarValue(Qt::Vertical, this->scrollValue);
}

void ChatWidget::on_wSend_clicked()
{
    //write.sendCommand(IrcCommand::createMessage(this->ui->wChannel->text(), this->ui->wInput->text()));
    this->ui->wInput->clear();
}
