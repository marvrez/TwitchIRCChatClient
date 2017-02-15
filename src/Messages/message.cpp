#include "message.h"

#include <QRegularExpression>
#include <QDebug>
//static variables
MentionManager Message::mention_manager;


Message* Message::onMessage(IrcPrivateMessage *message, QMap<QString, bool> &channelStates) {
    Message *msg = new Message();
    QVariantMap tags = message->tags();

    //set raw message
    msg->raw_message = QString(message->toData());

    //set display name
    QString displayName = tags["display-name"].toString();
    msg->username = (displayName.length() > 0) ? displayName : message->nick();

    //set color
    QString colorString = tags["color"].toString();
    msg->username_color = (colorString.length() == 0) ? QColor("#aa6633") : QColor(colorString);

    //check if user is sub
    if(tags.find("subscriber") != tags.end()) {
        msg->subMode = tags["subscriber"].toBool();
        //qDebug() << "LULLLL::" << msg->subMode;
    }
    //qDebug() << msg->message;

    //get badges
    auto badges = tags.find("badges").value().toString().split(",");
    //qDebug() << "ADSASDSDA:" << badges;
    for(QString badge : badges) {
        //qDebug() << badge;
        if(badge.contains("subscriber"))
            msg->subscriber = true;
        if(badge.contains("moderator"))
            msg->moderator = true;
        if(badge.contains("broadcaster"))
            msg->broadcaster = true;
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
    if((channelStates.find("subs-only") != channelStates.end() && channelStates["subs-only"]) && (!msg->subscriber || !msg->moderator || !msg->broadcaster )) {
        html_message.append(QString("<span class=\"not subscribed\" style=\"color:#727272;\">%1 </span>").arg(" This room is in subscribers only mode. To talk, purchase a channel subscription."));
        html_message.append("</div>");
        msg->message = html_message;
        msg->action = message->isAction();
        return msg;
    }

    if(msg->broadcaster) {
        html_message.append(QString("<span class =\"broadcaster\">"
                                    "<img src = \"https://assistly-production.s3.amazonaws.com/31172/portal_attachments/349942/broadcaster-background_original.png?AWSAccessKeyId=AKIAJNSFWOZ6ZS23BMKQ&Expires=1487127862&Signature=9jWdzLqwoQRv9wZkat9Ev6DAE84%3D&response-content-disposition=filename%3D%22broadcaster-background.png%22&response-content-type=image%2Fpng\"\> <\span>"));
    }
    else if (msg->moderator) {

    }
    if(msg->subscriber) {

    }
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
