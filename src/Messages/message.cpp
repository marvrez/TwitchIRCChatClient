#include "message.h"


Message* Message::onMessage(IrcPrivateMessage *message) {
    Message* msg = new Message();

    //set raw message
    msg->raw_message = QString(message->toData());

    //set display name
    QString displayName = message->tags()["display-name"].toString();
    msg->username = (displayName.length() > 0) ? displayName : message->nick();

    //set color
    QString colorString = message->tags()["color"].toString();
    msg->username_color = (colorString.length() == 0) ? QColor("#aa6633") : QColor(colorString);

    QString emotesString = message->tags()["emotes"].toString();
    QString html_content = message->content();

    msg->action = message->isAction();
    return msg;
}
