#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "Messages/message.h"
#include "channel.h"

#include <QDialog>
#include <QWebView>
#include <QNetworkRequest>

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = 0);
    ChatWidget(QString channelName);
    //bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);

    void linkClicked(const QUrl &url) const;

    //screen handling
    void updateMessageScreen(QList<Message*>* messages);
    void channelChanged(QList<Message*>* messages);
    void addMessage(Message *msg);
    void tryRemoveFirstMessage(QList<Message *> *messages);

    //void loadSubBadges(QString roomID);
    ~ChatWidget();

    //getters
    //QMap<QString, QString> getSubBadges();
    //QVariantMap* getRoomData();
    Channel* getChannel();

    //setters
    void setChannelName(const QString &channelName);
private slots:
    void on_wSend_clicked();
    //void onMessageReceived(IrcMessage *message);

private:
    Ui::ChatWidget *ui;

    int messageCount = 150;
    QString channelName;

    int scrollValue;
    bool autoScroll;

    Channel* channel;

    void chatContentsSizeChanged(const QSize &size);

    //QNetworkAccessManager network_access_manager;

    /*
    //maps - connected to each room/channel
    QVariantMap roomData; //room states - submode, r9k, etc..
    QMap<QString, QString> subBadges; // month(of subscripment) - url

    bool subBadgesLoaded = false;*/
};

#endif // CHATWIDGET_H
