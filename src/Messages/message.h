#ifndef MESSAGE_H
#define MESSAGE_H

#include "mentionmanager.h"
#include <IrcMessage>
#include <QColor>


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
    bool moderator;
    bool staff;
    bool broadcaster;
    bool bot;

    static MentionManager mention_manager;

    static Message* onMessage(IrcPrivateMessage *message);
};

#endif // MESSAGE_H
