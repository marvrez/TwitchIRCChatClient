#ifndef CHANNEL_H
#define CHANNEL_H

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
    Channel(QString channelName);

    QMap<QString, QString> getSubBadges();
    QVariantMap* getRoomData();

    void loadSubBadges(QString roomID);

    Channel& operator=(Channel channel);
    ~Channel();

private slots:
    void onMessageReceived(IrcMessage *message);

private:
    QString channelName;

    QNetworkAccessManager network_access_manager;

    //maps - connected to each room/channel
    QVariantMap roomData; //room states - submode, r9k, etc..
    QMap<QString, QString> subBadges; // month(of subscripment) - url

    bool subBadgesLoaded = false;
};

#endif // CHANNEL_H
