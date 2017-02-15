#ifndef MENTIONMANAGER_H
#define MENTIONMANAGER_H

#include <QList>
#include <QRegularExpression>

struct Mention {
    QRegularExpression regex; // regex of mention
    QString channel; // channel where mention should popup, empty for same channel
    QList<QString> usernames; // only for certain usernames
    QString cssclass;
};


class MentionManager
{
public:
    QList<Mention*> mentions;
};

#endif // MENTIONMANAGER_H
