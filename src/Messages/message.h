#ifndef MESSAGE_H
#define MESSAGE_H

#include "ChatWidget/channel.h"
#include "emotemanager.h"
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

    QString channel_id;

    QString raw_message;

    // Full parsed HTML Message (with emotes etc)
    QString message;

    // Whether it's a /me or not
    bool action;

    // IRCv3 flags --- switch over to enum or bitflags?
    bool moderator, global_moderator;
    bool staff;
    bool broadcaster;
    bool admin;
    bool bot;

    bool subMode;
    bool subscriber;
    bool turbo, premium;

    static MentionManager mention_manager;
    static EmoteManager emote_manager;

    static Message* onMessage(IrcPrivateMessage *message, Channel* channel);
    static Message* createNotSubscribedMessage();
    static bool twitchEmoteComparator(const struct EmoteReplacement &v1, const struct EmoteReplacement &v2);

    static int parseLinks(QString &htmlContent);
    static void parseTwitchEmotes(QString &message, QString &emotesString);
    static void parseBttvEmotes(QString& html_message);
    static void parseBttvChannelEmotes(QString& html_message, const QString channel);
    static void parseFfzEmotes(QString& html_message);
    static void parseFfzChannelEmotes(QString& html_message, const QString channel);

private:
    static void setGlobalBadges(QString& html_message, Message* msg);
    static void setSubBadges(QString& html_message, bool subscriber, QString subBadge);
};

struct EmoteReplacement {
    unsigned int index;
    unsigned int length;
    QString tag;
    EmoteReplacement(unsigned int idx, unsigned int len, QString _tag) : index(idx), length(len), tag(_tag)
    {}
};

#endif // MESSAGE_H
