#ifndef MESSAGE_H
#define MESSAGE_H

#include "mentionmanager.h"
#include <IrcMessage>
#include <QRegularExpression>
#include <QColor>

const QRegularExpression URL_REGEX = QRegularExpression("(?<![^ ])((?:https?|ftp)://\\S+)(?![^ ])");

class Message
{
public:
    // Username with proper capitalization
    QString username;

    QColor username_color;

    QString raw_message;

    // Full parsed HTML Message (with emotes etc)
    QString message;

    // Whether it's a /me or not
    bool action;

    // IRCv3 flags
    bool moderator, global_moderator;
    bool staff;
    bool broadcaster;
    bool admin;
    bool bot;

    bool subMode;
    bool subscriber;
    bool turbo, prime;

    static MentionManager mention_manager;

    static Message* onMessage(IrcPrivateMessage *message, QMap<QString, bool> *channelStates);
    static bool variantByIndex(const struct EmoteReplacement &v1, const struct EmoteReplacement &v2);
    static int parseLinks(QString &htmlContent);
    static void parseTwitchEmotes(QString &message, QString &emotesString);
private:
    static void setGlobalBadges(QString& html_message, Message* msg);
    static void setSubBadges(QString& html_message, Message* msg);
};

struct EmoteReplacement {
    unsigned int index;
    unsigned int length;
    QString tag;
};

#endif // MESSAGE_H
