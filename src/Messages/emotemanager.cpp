#include "emotemanager.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>


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
