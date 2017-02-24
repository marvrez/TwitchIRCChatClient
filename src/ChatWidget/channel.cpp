#include "channel.h"

#include "mainwindow.h"

#include <IrcMessage>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QUrl>

#include <algorithm>

Channel::Channel(QObject *parent) : QObject(parent)
{ }

Channel::Channel(QString channelName) {
    this->channel_name = channelName;
    QObject::connect(&MainWindow::read,
                     &IrcConnection::messageReceived,
                     this,
                     &Channel::onMessageReceived);
}

Channel::~Channel() {
    delete user_state;
}

UserState *Channel::getUserState() const{
    return user_state;
}

void Channel::loadSubBadges(QString roomID) {
    if(!subBadgesLoaded) {
        QUrl url = QUrl(QString("https://badges.twitch.tv/v1/badges/channels/%1/display").arg(roomID));
        QNetworkRequest req(url);
        QNetworkReply *reply = network_access_manager.get(req);

        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            QByteArray data = reply->readAll();

            QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
            QJsonObject root = jsonDoc.object();
            QJsonValue versions = root.value("badge_sets").toObject().value("subscriber").toObject().value("versions");
            QVariantMap versionMap = versions.toObject().toVariantMap();

            for(auto it = versionMap.constBegin(); it != versionMap.constEnd(); ++it) {
                    QString link = it.value().toMap().value("image_url_2x").toString();
                    subBadges.insert(QString("subscriber/%1").arg(it.key()), link);
            }
        });
        subBadgesLoaded = true;
    }
}

Channel& Channel::operator=(Channel channel) {
    std::swap(this->channel_name, channel.channel_name);
    return *this;
}

void Channel::onMessageReceived(IrcMessage *message) {
    QVariantMap tags = message->tags();
    qDebug() << message->toData();
    this->roomData["room-id"] = tags.contains("room-id") ? tags["room-id"].toString() : QString("");
    if(!tags.isEmpty()) {
        if(message->command() == "ROOMSTATE") {
                //this->roomData.clear();
                if(tags.contains("emote-only"))
                    this->roomData["emote-only"] = tags["emote-only"].toBool();
                if(tags.contains("followers-only"))
                    this->roomData["followers-only"] = tags["followers-only"].toBool();
                if(tags.contains("r9k"))
                    this->roomData["r9k"] = tags["r9k"].toBool();
                if(tags.contains("slow"))
                    this->roomData["slow"] = tags["slow"].toBool();
                if(tags.contains("subs-only")) {
                    this->roomData["subs-only"] = tags["subs-only"].toBool();
                }
        }
        else if (message->command() == "USERSTATE") {
            bool mod, sub, turbo;
            QString displayName = "";
            if(tags.contains("display-name"))
                displayName = tags["display-name"].toString();
            if(tags.contains("subscriber"))
                sub = tags["subscriber"].toBool();
            if(tags.contains("mod"))
                mod = tags["mod"].toBool();
            if(tags.contains("turbo"))
                turbo = tags["mod"].toBool();
            user_state = new UserState(displayName, mod, sub, turbo);
        }
        else if (message->command() == "CLEARCHAT") {
            //TODO
            qDebug() << "LUL SOMEONE GOT BANNED";
        }
        else if (message->command() == "USERNOTICE") {
            //TODO
            qDebug() << "PogChamp someone just resubbed";
        }
    }
}


QMap<QString, QString> Channel::getSubBadges(){
    return subBadges;
}


QVariantMap* Channel::getRoomData() {
    if(!roomData.empty())
        return &this->roomData;
    return new QVariantMap();
}
