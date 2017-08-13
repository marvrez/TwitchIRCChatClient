#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "Messages/message.h"
#include "channel.h"
#include "resizingtextedit.h"
#include "completionmanager.h"

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
    explicit ChatWidget(QWidget* parent = 0);
    ChatWidget(QString channelName);
    //bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);

    void linkClicked(const QUrl &url) const;

    //screen handling
    void updateMessageScreen(QList<Message*>* messages);
    void channelChanged(QList<Message*>* messages);
    void addMessage(Message* msg);
    void tryRemoveFirstMessage(QList<Message *>* messages);

    ~ChatWidget();

    //getters
    Channel* getChannel();

    //setters
    void setChannelName(const QString &channelName);
private slots:
    void on_wSend_clicked();

private:
    Ui::ChatWidget* ui;

    int messageCount = 150;
    QString channelName;

    int scrollValue;
    bool autoScroll;

    Channel* channel; //every widget has its own channel it's connected to..

    void chatContentsSizeChanged(const QSize &size);
};

#endif // CHATWIDGET_H
