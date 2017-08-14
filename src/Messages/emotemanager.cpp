#include "emotemanager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

BttvEmote::BttvEmote(QString _id, QString _code) : id(_id), code(_code) {
    QString regex_str = QString("(?<![^ ])%1(?![^ ])").arg(QRegularExpression::escape(code)); //using negative lookbehind/ahead
    this->regex = QRegularExpression(regex_str);
}

FfzEmote::FfzEmote(int _id, QString _code) : id(_id), code(_code) {
    QString regex_str = QString("(?<![^ ])%1(?![^ ])").arg(QRegularExpression::escape(code));
    this->regex = QRegularExpression(regex_str);
}

TwitchEmote::TwitchEmote(int id, QString code) : id(id), code(code) {
    QString regex_str = QString("(?<![^ ])%1(?![^ ])").arg(QRegularExpression::escape(code));
    this->regex = QRegularExpression(regex_str);
}

void EmoteManager::loadTwitchEmotes() {
    QNetworkAccessManager* nam = new QNetworkAccessManager();
    QUrl url = QUrl(QString("https://twitchemotes.com/api_cache/v3/global.json"));
    QNetworkRequest req(url);
    QNetworkReply* reply = nam->get(req);
    connect(reply,
            &QNetworkReply::finished,
            [=]() {
            if (reply->error() == QNetworkReply::NetworkError::NoError) {
                QByteArray data = reply->readAll();

                QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
                QJsonObject emotes = jsonDoc.object();
                for(auto emote : emotes) {
                    const QJsonObject &emote_obj = emote.toObject();
                    qDebug() << emote_obj;
                    this->twitchEmotes.push_back(TwitchEmote(emote_obj.value("id").toInt(),
                                                         emote_obj.value("code").toString()));
                }
                qDebug() << "TWITCH GLOBAL EMOTES LOADED";
                reply->deleteLater();
                nam->deleteLater();
            }
    });
}

void EmoteManager::loadBttvEmotes() {
    QNetworkAccessManager* nam = new QNetworkAccessManager();
    QUrl url = QUrl(QString("https://api.betterttv.net/2/emotes"));
    QNetworkRequest req(url);
    QNetworkReply *reply = nam->get(req);
    connect(reply,
            &QNetworkReply::finished,
            [=]() {
            if (reply->error() == QNetworkReply::NetworkError::NoError) {
                QByteArray data = reply->readAll();

                QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
                QJsonObject root = jsonDoc.object();
                QJsonArray emotes = root.value("emotes").toArray();
                for(auto emote : emotes) {
                    const QJsonObject &emote_obj = emote.toObject();
                    this->bttvEmotes.push_back(BttvEmote(emote_obj.value("id").toString(),
                                                         emote_obj.value("code").toString()));
                }
                qDebug() << "BTTV GLOBAL EMOTES LOADED";
                reply->deleteLater();
                nam->deleteLater();
            }
    });
}

void EmoteManager::loadFfzEmotes() {

    QNetworkAccessManager* nam = new QNetworkAccessManager();
    QUrl url = QUrl("https://api.frankerfacez.com/v1/set/global");
    QNetworkRequest req(url);
    QNetworkReply *reply = nam->get(req);
    connect(reply,
            &QNetworkReply::finished,
            [=]() {
            if (reply->error() == QNetworkReply::NetworkError::NoError) {
                QByteArray data = reply->readAll();

                QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
                QJsonObject root = jsonDoc.object();
                QJsonArray default_sets = root.value("default_sets").toArray();
                for(auto default_set : default_sets) {
                    QJsonObject set = root.value("sets").toObject().value(QString::number(default_set.toDouble())).toObject();
                    QJsonArray emotes = set.value("emoticons").toArray();
                    for(auto emote : emotes) {
                        const QJsonObject &emote_obj = emote.toObject();
                        this->ffzEmotes.push_back(FfzEmote(emote_obj.value("id").toInt(),
                                                           emote_obj.value("name").toString()));
                    }
                 }
                qDebug() << "FFZ EMOTES LOADED";
                reply->deleteLater();
                nam->deleteLater();
            }
    });
}

void EmoteManager::loadBttvChannelEmotes(const QString channel) {
    //https://api.betterttv.net/2/channels/{channel_name}
    QUrl url = QUrl(QString("https://api.betterttv.net/2/channels/%1")
                           .arg(channel.right(channel.length() - 1)));

    QNetworkAccessManager* nam = new QNetworkAccessManager();
    QNetworkRequest req(url);
    QNetworkReply *reply = nam->get(req);

    QList<BttvEmote> *list;

    if (this->bttvChannelEmotes.contains(channel))
        list = &this->bttvChannelEmotes[channel];
    else {
        this->bttvChannelEmotes.value(channel, QList<BttvEmote>());
        list = &this->bttvChannelEmotes[channel];
    }

    QObject::connect(reply,
                     &QNetworkReply::finished,
                     [=]() {
                         if(reply->error() == QNetworkReply::NetworkError::NoError) {
                             QByteArray data = reply->readAll();

                             QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
                             QJsonObject root = jsonDoc.object();

                             QJsonArray emotes = root.value("emotes").toArray();
                             //QString urlTemplate = "https:" + root.value("urlTemplate").toString();
                             for(auto emote : emotes) {
                                 QString id = emote.toObject().value("id").toString(),
                                         code = emote.toObject().value("code").toString();
                                 list->push_back(BttvEmote(id,code));
                             }
                         }
                         reply->deleteLater();
                         nam->deleteLater();
                         qDebug() << "LOADED BTTV CHANNELEMOTES FROM CHANNEL" << channel;
                     });
}

void EmoteManager::loadFfzChannelEmotes(const QString channel) {
    //http://api.frankerfacez.com/v1/room/{channel_name}
    QUrl url = QUrl(QString("http://api.frankerfacez.com/v1/room/%1")
                           .arg(channel.right(channel.length() - 1)));
    QNetworkAccessManager* nam = new QNetworkAccessManager();
    QNetworkRequest req(url);
    QNetworkReply *reply = nam->get(req);

    QList<FfzEmote> *list;

    if (this->ffzChannelEmotes.contains(channel))
        list = &this->ffzChannelEmotes[channel];
    else {
        this->ffzChannelEmotes.value(channel, QList<FfzEmote>());
        list = &this->ffzChannelEmotes[channel];
    }
    QObject::connect(reply,
                     &QNetworkReply::finished,
                     [=]() {
                         if(reply->error() == QNetworkReply::NetworkError::NoError) {
                             QByteArray data = reply->readAll();
                             QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
                             QJsonObject root = jsonDoc.object();

                             auto sets = root.value("sets").toObject();
                             for(const QJsonValue &set : sets) {
                                 QJsonArray emotes = set.toObject().value("emoticons").toArray();
                                 for(const QJsonValue &emote : emotes) {
                                    QJsonObject object = emote.toObject();

                                    int id = object.value("id").toInt();
                                    QString code = object.value("name").toString();
                                    list->push_back(FfzEmote(id,code));
                                 }
                             }
                         }
                         nam->deleteLater();
                         reply->deleteLater();
                         qDebug() << "LOADED FFZ CHANNELEMOTES FROM CHANNEL" << channel;
                     });
}

QMap<QString, QList<BttvEmote> > EmoteManager::getBttvChannelEmotes() const {
    return bttvChannelEmotes;
}

QMap<QString, QList<FfzEmote> > EmoteManager::getFfzChannelEmotes() const {
    return ffzChannelEmotes;
}

QList<BttvEmote> EmoteManager::getBttvEmotes() const {
    return bttvEmotes;
}

QList<FfzEmote> EmoteManager::getFfzEmotes() const {
    return ffzEmotes;
}

QList<TwitchEmote> EmoteManager::getTwitchEmotes() const {
    return twitchEmotes;
}

