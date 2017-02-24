#ifndef CHANNEL_H
#define CHANNEL_H

#include "userstate.h"

#include <IrcMessage>
#include <QMap>
#include <QNetworkAccessManager>
#include <QString>
#include <QVariantMap>

//Contains data about the current channel
class Channel : public QObject
{
    Q_OBJECT

public:
    explicit Channel(QObject *parent = nullptr);
    Channel(QString channel_name);

    QMap<QString, QString> getSubBadges();
    QVariantMap* getRoomData();

    void loadSubBadges(QString roomID);

    Channel& operator=(Channel channel);
    ~Channel();

    UserState* getUserState() const;

private slots:
    void onMessageReceived(IrcMessage *message);

private:
    QString channel_name;
    UserState* user_state;

    QNetworkAccessManager network_access_manager;

    //maps - connected to each room/channel
    QVariantMap roomData; //room states - submode, r9k, etc..
    QMap<QString, QString> subBadges; // month(of subscripment) - url

    bool subBadgesLoaded = false;
};

#endif // CHANNEL_H
