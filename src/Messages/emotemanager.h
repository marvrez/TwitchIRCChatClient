#ifndef EMOTEMANAGER_H
#define EMOTEMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QRegularExpression>

class BttvEmote {
public:
    BttvEmote(QString _id, QString _code);

    QString id, code;
    QRegularExpression regex;
};

class FfzEmote {
public:
    FfzEmote(int _id, QString _code );

    int id;
    QString code;
    QRegularExpression regex;
};

class TwitchEmote {
public:
    TwitchEmote(int id, QString code);

    int id;
    QString code;
    QRegularExpression regex;
};

class EmoteManager : public QObject
{
    Q_OBJECT
public:
    //void getTwitchEmotes();
    void loadBttvEmotes();
    void loadFfzEmotes();
    void loadTwitchEmotes();
    void loadBttvChannelEmotes(const QString channel);
    void loadFfzChannelEmotes(const QString channel);
    //void loadTwitchChannelEmotes(const QString& channel);

    //getters of variables
    QMap<QString, QList<BttvEmote> > getBttvChannelEmotes() const;
    QMap<QString, QList<FfzEmote> > getFfzChannelEmotes() const;
    QList<BttvEmote> getBttvEmotes() const;
    QList<FfzEmote> getFfzEmotes() const;
    QList<TwitchEmote> getTwitchEmotes() const;

private:
    //QMap<int, QString> emote_ids; //maps emote key to emote name

    //mapping from channel to list of emotes
    QMap<QString, QList<BttvEmote> > bttvChannelEmotes;
    QMap<QString, QList<FfzEmote> > ffzChannelEmotes;
    QMap<QString, QList<TwitchEmote> > twitchChannelEmotes;

    //global emotes
    QList<TwitchEmote> twitchEmotes;
    QList<BttvEmote> bttvEmotes;
    QList<FfzEmote> ffzEmotes;
};

#endif // EMOTEMANAGER_H
