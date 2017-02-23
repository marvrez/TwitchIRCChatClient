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
    this->channelName = channelName;
    QObject::connect(&MainWindow::read,
                     &IrcConnection::messageReceived,
                     this,
                     &Channel::onMessageReceived);
}

Channel::~Channel() {

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
            qDebug() << subBadges;
        });
        subBadgesLoaded = true;
    }
}

Channel& Channel::operator=(Channel channel) {
    std::swap(this->channelName, channel.channelName);
    return *this;
}

void Channel::onMessageReceived(IrcMessage *message) {
    QVariantMap tags = message->tags();
    qDebug() << message->toData();
    this->roomData["room-id"] = tags.contains("room-id") ? tags["room-id"].toString() : QString("");
    if(message->command() == "ROOMSTATE") {
        if(!tags.isEmpty()) {
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
    }
    else if (message->command() == "PRIVMSG") {

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
