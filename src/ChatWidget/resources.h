#ifndef RESOURCES_H
#define RESOURCES_H

#include <QColor>
#include <QString>

class Resources
{
public:
    static void load();

    static QColor calculateColor(QColor color);

    // badges
    static QString  getBadgeStaff(){
        return badgeStaff;
    }

    static QString  getBadgeAdmin(){
        return badgeAdmin;
    }

    static QString  getBadgeGlobalmod(){
        return badgeGlobalmod;
    }

    static QString  getBadgeModerator(){
        return badgeModerator;
    }

    static QString getBadgeTurbo(){
        return badgeTurbo;
    }

    static QString  getBadgeBroadcaster(){
        return badgeBroadcaster;
    }

    static QString  getBadgePremium(){
        return badgePremium;
    }
    static QString  getBadgeBot(){
        return badgeBot;
    }

private:
    Resources();
    static QString calculateColorBackground(QColor color);
    static QColor calculateColorReplacement(QColor color, QString backgroundColor);
    static QString  badgeStaff;
    static QString  badgeAdmin;
    static QString  badgeBot;
    static QString  badgeGlobalmod;
    static QString  badgeModerator;
    static QString  badgeTurbo;
    static QString  badgeBroadcaster;
    static QString  badgePremium;
};

#endif // RESOURCES_H
