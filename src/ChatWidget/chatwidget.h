#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "Messages/message.h"

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
    //bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);
    int scrollValue;
    bool autoScroll;

    void linkClicked(const QUrl &url) const;

    //screen handling
    void updateMessageScreen(QList<Message*>* messages);
    void channelChanged(QList<Message*>* messages);
    void addMessage(Message *msg);
    void tryRemoveFirstMessage(QList<Message *> *messages);

    void setChannel(const QString &channel);
    QVariantMap* getChannelStates();
    ~ChatWidget();

private slots:
    void on_wSend_clicked();
    void onMessageReceived(IrcMessage *message);

private:
    Ui::ChatWidget *ui;
    int messageCount = 150;
    QString channel;
    void chatContentsSizeChanged(const QSize &size);
    QVariantMap channelStates;
};

#endif // CHATWIDGET_H
