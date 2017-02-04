#ifndef IRCMANAGER_H
#define IRCMANAGER_H

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
    bool isInChannel(const QString *channelName);
    const QSet<QString>* getChannels() const;
    void getMessages();

private:
    QSet<QString> channels;
    QMap<QString, QList<QString*>> messages;
};

#endif // IRCMANAGER_H
