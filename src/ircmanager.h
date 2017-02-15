#ifndef IRCMANAGER_H
#define IRCMANAGER_H

#include "Messages/message.h"

#include <QMap>
#include <QSet>
#include <QObject>
#include <QString>
#include <QSettings>
#include <IrcCommand>
#include <IrcConnection>

class IrcManager : public IrcConnection
{
    Q_OBJECT

public:
    IrcManager(QObject *parent = 0);

    void connect();
    bool joinChannel(const QString *channelName);
    bool isInChannel(const QString *channelName) const;
    bool removeChannel(QString channelName);
    void sendMessage(const QString channel, const QString message);
    const QSet<QString>* getChannels() const;
    QList<Message*>* getMessages(const QString &channelName);

private:
    QSet<QString> channels;
    QMap<QString, QList<Message*>> messages; //key:channel, value: messages
};

#endif // IRCMANAGER_H
