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
    void addMessage(Message *msg);
    void setChannel(const QString &channel);
    void linkClicked(const QUrl &url);
    int scrollValue;
    bool autoScroll;
    void updateMessageScreen(QList<Message*>* messages);
    void channelChanged(QList<Message*>* messages);

    ~ChatWidget();

private slots:
    void on_wSend_clicked();

private:
    Ui::ChatWidget *ui;
    int messageCount = 150;
    QString channel;
    void chatContentsSizeChanged(const QSize &size);
};

#endif // CHATWIDGET_H
