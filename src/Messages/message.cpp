#include "message.h"

#include <QRegularExpression>
#include <QDebug>
//static variables
MentionManager Message::mention_manager;


Message* Message::onMessage(IrcPrivateMessage *message, QVariantMap* channelStates) {
    Message *msg = new Message();
    QVariantMap tags = message->tags();

    //set raw message
    msg->raw_message = QString(message->toData());

    //set display name
    QString displayName = tags["display-name"].toString();
    msg->username = (displayName.length() > 0) ? displayName : message->nick();

    //set user-id
    msg->channel_id = tags["room-id"].toString();
    qDebug() << msg->channel_id;

    //set color
    QString colorString = tags["color"].toString();
    msg->username_color = (colorString.length() == 0) ? QColor("#00f0a0") : QColor(colorString);

    //check if submode is on
    if(tags.find("subscriber") != tags.end())
        msg->subMode = tags["subscriber"].toBool();


    //get badges
    auto badges = tags.find("badges").value().toString().split(",");
    //qDebug() << "ADSASDSDA:" << badges;
    for(QString badge : badges) {
        //qDebug() << badge;
        if(badge.contains("subscriber"))
            msg->subscriber = true;
        if(badge.contains("premium"))
            msg->prime = true;
        if(badge.contains("staff"))
            msg->staff = true;
        if(badge.contains("moderator"))
            msg->moderator = true;
        if(badge.contains("broadcaster"))
            msg->broadcaster = true;
        if(badge.contains("global_mod"))
            msg->global_moderator = true;
        if(badge.contains("turbo"))
            msg->turbo = true;
    }

    QString emotesString = tags["emotes"].toString();
    QString html_content = message->content();

    parseLinks(html_content);

    QString mentionClass;
    for (int i = 0; i < mention_manager.mentions.count(); ++i) {
        Mention* mention = mention_manager.mentions.at(i);
        if (mention->regex.captureCount() >= 0) {
            mentionClass = QString("data-mention=\"%1\"").arg(mention->cssclass);
            break;
        }
    }

    QString html_message = QString("<div class=\"message%3\" data-emotes=\"%2\" %1>").arg(mentionClass).arg(emotesString);
    QString content = QString("<span class=\"content\" %2>%1</span>").arg(html_content);
    if (message->isAction()) {
        html_message = html_message.arg(" action");
        content = content.arg(QString("style=\"color: %1\"").arg(msg->username_color.name()));
    } else {
        html_message = html_message.arg("");
        content = content.arg("");
    }
    html_message.append(QString("<span class=\"timestamp\" style=\"color:#727272;\">%1 </span>").arg(QTime::currentTime().toString().mid(0,5)));
    if((channelStates->contains("subs-only") && (*channelStates)["subs-only"].toBool()) && (!msg->subscriber || !msg->moderator || !msg->broadcaster )) {
        html_message.append(QString("<span class=\"not subscribed\" style=\"color:#727272;\">%1 </span>").arg(" This room is in subscribers only mode. To talk, purchase a channel subscription."));
        html_message.append("</div>");
        msg->message = html_message;
        msg->action = message->isAction();
        return msg;
    }

    setGlobalBadges(html_message, msg);
    setSubBadges(html_message, msg);
    html_message.append(QString("<span class=\"username\" style=\"color: %1;\">%2</span>").arg(msg->username_color.name(), msg->username));

    html_message.append("<span class=\"colon\">:</span>");
    //qDebug() << html_message;

    html_message.append(content);

    html_message.append("</div>");


    msg->message = html_message;

    msg->action = message->isAction();
    return msg;
}

bool Message::variantByIndex(const EmoteReplacement &v1, const EmoteReplacement &v2) {
    return v1.index < v2.index;
}

int Message::parseLinks(QString &html_content) {
    int numLinks = 0, offset = 0;

    QRegularExpressionMatchIterator it = URL_REGEX.globalMatch(html_content);
    while(it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        numLinks++;
        QString url = match.captured(1);
        // Extra space so link doesn't carry over to next line
        QString new_url = QString("<a href=\"%1\">%1</a> ").arg(url);
        html_content.replace(match.capturedStart(1) + offset, match.capturedLength(1), new_url);
        offset += url.length() + 15 + 1;
    }
    return numLinks;
}

void Message::parseTwitchEmotes(QString &message, QString &emotesString) {

}

void Message::setGlobalBadges(QString &html_message, Message* msg) {
    //https://badges.twitch.tv/v1/badges/global/display?language=en
    if(msg->admin) {
        html_message.append(QString("<span class =\"admin\">"
                                    "<img src = \"https://static-cdn.jtvnw.net/badges/v1/9ef7e029-4cdf-4d4d-a0d5-e2b3fb2583fe/1\" \>  "
                                    "<\span>"));
    }
    if(msg->bot) {
        html_message.append(QString("<span class =\"bot\">"
                                    "<img src = \"https://static-cdn.jtvnw.net/badges/v1/df9095f6-a8a0-4cc2-bb33-d908c0adffb8/1\" \>  "
                                    "<\span>"));
    }
    if(msg->broadcaster) {
        html_message.append(QString("<span class =\"broadcaster\">"
                                    "<img src = \"https://static-cdn.jtvnw.net/badges/v1/5527c58c-fb7d-422d-b71b-f309dcb85cc1/1\" \>  "
                                    "<\span>"));
    }
    else if (msg->moderator) {
        html_message.append(QString("<span class =\"moderator\">"
                                    "<img src = \"https://static-cdn.jtvnw.net/badges/v1/3267646d-33f0-4b17-b3df-f923a41db1d0/1\" \>  "
                                    "<\span>"));
    }
    if(msg->global_moderator) {
        html_message.append(QString("<span class =\"global moderator\">"
                                    "<img src = \"hhttps://static-cdn.jtvnw.net/badges/v1/9384c43e-4ce7-4e94-b2a1-b93656896eba/1\" \>  "
                                    "<\span>"));
    }
    if(msg->turbo) {
        html_message.append(QString("<span class =\"turbo\">"
                                    "<img src = \"https://static-cdn.jtvnw.net/badges/v1/bd444ec6-8f34-4bf9-91f4-af1e3428d80f/1\" \>  "
                                    "<\span>"));
    }
    if(msg->prime) {
        html_message.append(QString("<span class =\"prime\">"
                                    "<img src = \"https://static-cdn.jtvnw.net/badges/v1/a1dd5073-19c3-4911-8cb4-c464a7bc1510/1\" \>  "
                                    "<\span>"));
    }

    if(msg->staff) {
        html_message.append(QString("<span class =\"staff\">"
                                    "<img src = \"https://static-cdn.jtvnw.net/badges/v1/d97c37bd-a6f5-4c38-8f57-4e4bef88af34/1\" \>  "
                                    "<\span>"));
    }
}

void Message::setSubBadges(QString &html_message, Message* msg)
{
    //https://badges.twitch.tv/v1/badges/channels/24991333/display
}
