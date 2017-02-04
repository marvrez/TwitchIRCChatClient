#include "ircmanager.h"

IrcManager::IrcManager(QObject *parent)
    : IrcConnection(parent),
      channels()
{
}

void IrcManager::connect() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "marvin", "chat");

    this->setHost(settings.value("Server/host","irc.chat.twitch.tv").toString());
    this->setPort(settings.value("Server/port",6667).toInt());

    this->setPassword(settings.value("Credentials/password", "oauth:IN2017LUL").toString());
    this->setUserName(settings.value("Credentials/username", "realKKona123").toString());
    this->setNickName(settings.value("Credentials/username", "realKKona123").toString());
    this->setRealName("Twitch User");

    this->sendCommand(IrcCommand::createCapability("REQ","twitch.tv/membership"));
    this->sendCommand(IrcCommand::createCapability("REQ", "twitch.tv/commands"));
    this->sendCommand(IrcCommand::createCapability("REQ", "twitch.tv/tags"));

    this->open();
}

bool IrcManager::joinChannel(const QString *channelName) {
    if(this->channels.contains(channelName->toLower())) return false;

    if(this->sendCommand(IrcCommand::createJoin(channelName->toLower()))) {
        this->channels.insert(channelName->toLower());
        return true;
    }
    return false;
}

bool IrcManager::isInChannel(const QString *channelName) {
    return this->channels.contains(channelName->toLower());
}

const QSet<QString>* IrcManager::getChannels() const {
    return &this->channels;
}

void IrcManager::getMessages() {
    //this->rec
}
