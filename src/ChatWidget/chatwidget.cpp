#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "mainwindow.h"

#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QWebFrame>
#include <QWebElement>
#include <QScrollBar>
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrl>

ChatWidget::ChatWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    this->layout()->setContentsMargins(0,0,0,0);
    this->ui->wSend->setStyleSheet(("background-color: #6441A5"));
    this->ui->chatWindow->settings()->setUserStyleSheetUrl(QUrl("qrc:/ChatWindow/style.css"));
    this->ui->chatWindow->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);//Handle link clicks by yourself
    this->ui->chatWindow->setContextMenuPolicy(Qt::NoContextMenu);

    this->ui->wInput->setFocus();

    QObject::connect(this->ui->chatWindow,
                     &QWebView::linkClicked,
                     this,
                     &ChatWidget::linkClicked);
    QObject::connect(this->ui->chatWindow->page()->mainFrame(),
                     &QWebFrame::contentsSizeChanged,
                     this,
                     &ChatWidget::chatContentsSizeChanged);
    QObject::connect(this->ui->wInput,
                     &QLineEdit::returnPressed,
                     this->ui->wSend,
                     &QPushButton::click);
}

ChatWidget::ChatWidget(QString channelName) : ChatWidget(){
    this->channelName = channelName;
    channel = new Channel(channelName);
}
/*
void ChatWidget::loadSubBadges(QString roomID) {
    if(!subBadgesLoaded) {
        QUrl url = QUrl(QString("https://badges.twitch.tv/v1/badges/channels/%1/display").arg(roomID));
        QNetworkRequest req(url);
        QNetworkReply *reply = network_access_manager.get(req);

        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            QByteArray data = reply->readAll();

            QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
            QJsonObject root = jsonDoc.object();
            QJsonValue versions = root.value("badge_sets").toObject().value("subscriber").toObject().value("versions");
            QVariantMap versionMap = versions.toObject().toVariantMap();

            for(auto it = versionMap.constBegin(); it != versionMap.constEnd(); ++it) {
                    QString link = it.value().toMap().value("image_url_1x").toString();
                    subBadges.insert(QString("subscriber/%1").arg(it.key()), link);
            }
            qDebug() << subBadges;
        });
        subBadgesLoaded = true;
    }
}
*/
void ChatWidget::addMessage(Message *msg) {
    //qDebug() << msg->raw_message << QTime::currentTime().toString();
    this->ui->chatWindow->page()->mainFrame()->documentElement().findFirst("body").appendInside(QString("%1\n").arg(msg->message));
}
/*
void ChatWidget::onMessageReceived(IrcMessage *message) {
    QVariantMap tags = message->tags();
    qDebug() << message->toData();
    this->roomData["room-id"] = tags.contains("room-id") ? tags["room-id"].toString() : QString("");
    if(message->command() == "ROOMSTATE") {
        if(!tags.isEmpty()) {
            //this->roomData.clear();
            if(tags.contains("emote-only"))
                this->roomData["emote-only"] = tags["emote-only"].toBool();
            if(tags.contains("followers-only"))
                this->roomData["followers-only"] = tags["followers-only"].toBool();
            if(tags.contains("r9k"))
                this->roomData["r9k"] = tags["r9k"].toBool();
            if(tags.contains("slow"))
                this->roomData["slow"] = tags["slow"].toBool();
            if(tags.contains("subs-only")) {
                this->roomData["subs-only"] = tags["subs-only"].toBool();
            }
        }
    }
    else if (message->command() == "PRIVMSG") {

    }
}
*/
void ChatWidget::setChannelName(const QString &channelName){
    this->channelName = channelName;
    qDebug() << this->channelName;
}

void ChatWidget::linkClicked(const QUrl &url) const {
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

void ChatWidget::tryRemoveFirstMessage(QList<Message*> *messages) {
    if (messages->count() > messageCount)
        delete messages->takeFirst();
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

Channel *ChatWidget::getChannel() {
    return this->channel;
}

void ChatWidget::chatContentsSizeChanged(const QSize &size) {
    QWebFrame* frame = this->ui->chatWindow->page()->mainFrame();
    if (this->autoScroll)
        frame->setScrollBarValue(Qt::Vertical, size.height());
    else
        frame->setScrollBarValue(Qt::Vertical, this->scrollValue);
}

/*
QVariantMap* ChatWidget::getRoomData() {
    if(!roomData.empty())
        return &this->roomData;
    return new QVariantMap();
}


QMap<QString, QString> ChatWidget::getSubBadges() {
    return subBadges;
}*/

void ChatWidget::on_wSend_clicked() {
    MainWindow::write.sendMessage(this->channelName, this->ui->wInput->text());
    this->ui->wInput->clear();
}
