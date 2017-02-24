#include "message.h"

#include "ChatWidget/chatwidget.h"
#include <ChatWidget/resources.h>
#include "emotemanager.h"

#include <QRegularExpression>
#include <QUrl>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

//static variables
MentionManager Message::mention_manager;
EmoteManager Message::emote_manager;

Message* Message::onMessage(IrcPrivateMessage *message, Channel* channel) {
    Message *msg = new Message();
    QVariantMap tags = message->tags();
    QVariantMap* roomData = channel->getRoomData();
    QString subBadge = "";

    //set raw message
    msg->raw_message = QString(message->toData());

    //set display name
    QString displayName = tags["display-name"].toString();
    msg->username = (displayName.length() > 0) ? displayName : message->nick();

    //set color
    QString colorString = tags["color"].toString();
    msg->username_color = (colorString.length() == 0) ? Resources::calculateColor(QColor("#00f0a0")) :
                                                        Resources::calculateColor(QColor(colorString));


    //check if submode is on
    if(tags.find("subscriber") != tags.end())
        msg->subscriber = true;


    //get badges
    auto badges = tags.find("badges").value().toString().split(",");
    //qDebug() << "ADSASDSDA:" << badges;
    for(QString badge: badges) {
        if(badge.isEmpty()) continue;
        if(badge.contains("subscriber")) {
            msg->subscriber = true;
            subBadge = badge;
        }
        if(badge.contains("premium"))
            msg->premium = true;
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


    if (emotesString.length() > 0)
        parseTwitchEmotes(html_content, emotesString);
    else
        html_content = html_content.replace("<", "&lt;").replace(">", "&gt;");

    parseBttvEmotes(html_content);
    parseBttvChannelEmotes(html_content, message->target());
    parseFfzEmotes(html_content);
    parseFfzChannelEmotes(html_content, message->target());
    parseLinks(html_content);
/*
    for(auto c : Resources::channels) {
        qDebug() << c;
        parseBttvChannelEmotes(html_content,"#" + c);
        parseFfzChannelEmotes(html_content, "#" + c);
    }*/

    QString mentionClass;
    for (int i = 0; i < mention_manager.mentions.count(); ++i) {
        Mention* mention = mention_manager.mentions.at(i);
        if (mention->regex.match(message->content()).hasMatch()) {
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
    /*
    if((*roomData)["subs-only"].toBool() && !(msg->subscriber || msg->moderator || msg->broadcaster )) {
        html_message.append(QString("<span class=\"not subscribed\" style=\"color:#727272;\">%1 </span>").arg(" This room is in subscribers only mode. To talk, purchase a channel subscription."));
        html_message.append("</div>");
        msg->message = html_message;
        msg->action = message->isAction();
        return msg;
    }*/

    setGlobalBadges(html_message, msg);
    setSubBadges(html_message, msg->subscriber, channel->getSubBadges().value(subBadge));
    html_message.append(QString("<span class=\"username\" style=\"color: %1;\">%2</span>").arg(msg->username_color.name(), msg->username));

    html_message.append("<span class=\"colon\">:</span>");
    //qDebug() << html_message;

    html_message.append(content);

    html_message.append("</div>");


    msg->message = html_message;

    msg->action = message->isAction();
    return msg;
}

Message *Message::createNotSubscribedMessage() {
    Message* msg = new Message();
    QString html_message = QString("<div class=\"message\">");
    html_message.append(QString("<span class=\"not subscribed\" style=\"color:#727272;\">%1 </span>").arg(" This room is in subscribers only mode. To talk, purchase a channel subscription."));
    html_message.append("</div>");
    msg->message = html_message;
    return msg;
}

bool Message::twitchEmoteComparator(const EmoteReplacement &v1, const EmoteReplacement &v2) {
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
    QStringList unique_emotes = emotesString.split('/'); //gets the emotes and their positions in the emotes string
    QList<struct EmoteReplacement> replacements;
    for(auto unique_emote : unique_emotes) {
        int emote_id = unique_emote.section(":", 0, 0).toInt();
        QStringList emote_positions = unique_emote.section(":", 1, 1).split(",");
        for(auto emote_position : emote_positions) {
            unsigned int begin = emote_position.section("-",0,0).toInt(),
                end   = emote_position.section("-",1,1).toInt();
            QString img_tag = QString("<img src=\"http://static-cdn.jtvnw.net/emoticons/v1/%1/1.0\" />").arg(emote_id);
            replacements.append(EmoteReplacement(begin, end-begin+1,img_tag));
        }
    }

    int offset = 0, last_i = 0;
    qSort(replacements.begin(), replacements.end(), twitchEmoteComparator);

    for(auto replacement : replacements) {
        for(int i = last_i + offset; i < replacement.index + offset; ++i) {
            const QChar &c = message[i];
            if (c.isHighSurrogate()) offset += 1; //check if c is utf16
            if (c == '>' || c == '<') offset += 3; //just so emotes arent out of position
            //fix html entities
            if (c == '>') message = message.replace(i, 1, "&gt;");
            if (c == '<') message = message.replace(i, 1, "&lt;");
        }
        last_i = replacement.index + replacement.length;
        message = message.replace(replacement.index + offset, replacement.length, replacement.tag);
        offset += replacement.tag.length() - replacement.length;
/*
        for (int i = last_i+offset; i < message.length(); ++i) {
            const QChar &c = message[i];
            if (c == '>') message = message.replace(i, 1, "&gt;");
            if (c == '<') message = message.replace(i, 1, "&lt;");
        }*/
    }

}

void Message::parseBttvEmotes(QString &html_message) {
    for(auto emote : emote_manager.getBttvEmotes()) {
        QString img_tag = QString("<img src=\"https://cdn.betterttv.net/emote/%1/1x\" alt=\"%2\"/>")
                                 .arg(emote.id).arg(emote.code);
        html_message.replace(emote.regex, img_tag);
    }
}

void Message::parseBttvChannelEmotes(QString &html_message, const QString channel) {
    auto bttvChannelEmotes = emote_manager.getBttvChannelEmotes();
    if(bttvChannelEmotes.contains(channel)) {
        for(auto emote : bttvChannelEmotes.value(channel)) {
            QString img_tag = QString("<img src=\"https://cdn.betterttv.net/emote/%1/1x\" alt=\"%2\"/>")
                                   .arg(emote.id).arg(emote.code);
            html_message.replace(emote.regex, img_tag);
        }
    }
}

void Message::parseFfzEmotes(QString &html_message) {
    for(auto emote : emote_manager.getFfzEmotes()) {
        QString img_tag = QString("<img src=\"https://cdn.frankerfacez.com/emoticon/%1/1\" alt=\"%2\"/>")
                                 .arg(emote.id).arg(emote.code);
        html_message.replace(emote.regex, img_tag);
    }
}

void Message::parseFfzChannelEmotes(QString &html_message, const QString channel) {
    auto ffzChannelEmotes = emote_manager.getFfzChannelEmotes();
    if(ffzChannelEmotes.contains(channel)) {
        for(auto emote : ffzChannelEmotes.value(channel)) {
            QString img_tag = QString("<img src=\"https://cdn.frankerfacez.com/emoticon/%1/1\" alt=\"%2\"/>")
                                     .arg(emote.id).arg(emote.code);
            html_message.replace(emote.regex, img_tag);
        }
    }
}

void Message::setGlobalBadges(QString &html_message, Message* msg) {
    //https://badges.twitch.tv/v1/badges/global/display?language=en
    if(msg->admin) {
        html_message.append(QString("<span class =\"admin\">"
                                    "<img src = \"%1\" style = \"vertical-align:-4.5px;\"\>  "
                                    "<\span>").arg(Resources::getBadgeAdmin()));
    }
    if(msg->bot) {
        html_message.append(QString("<span class =\"bot\">"
                                    "<img src = \"%1\" style = \"vertical-align:-4.5px;\" \>  "
                                    "<\span>").arg(Resources::getBadgeBot()));
    }
    if(msg->broadcaster) {
        html_message.append(QString("<span class =\"broadcaster\">"
                                    "<img src = \"%1\" style = \"vertical-align:-4.5px;\" \>  "
                                    "<\span>").arg(Resources::getBadgeBroadcaster()));
    }
    else if (msg->moderator) {
        html_message.append(QString("<span class =\"moderator\">"
                                    "<img src = \"%1\" style = \"vertical-align:-4.5px;\" \>  "
                                    "<\span>").arg(Resources::getBadgeModerator()));
    }
    if(msg->global_moderator) {
        html_message.append(QString("<span class =\"global moderator\">"
                                    "<img src = \"%1\" style = \"vertical-align:-4.5px;\" \>  "
                                    "<\span>").arg(Resources::getBadgeGlobalmod()));
    }
    if(msg->turbo) {
        html_message.append(QString("<span class =\"turbo\">"
                                    "<img src = \"%1\" style = \"vertical-align:-4.5px;\" \>  "
                                    "<\span>").arg(Resources::getBadgeTurbo()));
    }
    if(msg->premium) {
        html_message.append(QString("<span class =\"prime\">"
                                    "<img src = \"%1\" style = \"vertical-align:-4.5px;\" \>  "
                                    "<\span>").arg(Resources::getBadgePremium()));
    }

    if(msg->staff) {
        html_message.append(QString("<span class =\"staff\">"
                                    "<img src = \"%1\" \> style = \"vertical-align:-4.5px;\"  "
                                    "<\span>").arg(Resources::getBadgeStaff()));
    }
}

void Message::setSubBadges(QString &html_message, bool subscriber, QString subBadge) {
    //https://badges.twitch.tv/v1/badges/channels/24991333/display
    if(!subscriber || subBadge.isEmpty()) return;
    html_message.append(QString("<span class =\"sub-badge\">"
                                "<img src = \"%1\" style = \"vertical-align:-4.5px;\" \>  "
                                "<\span>").arg(subBadge));
}
