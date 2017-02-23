#include "resources.h"

#include <QDebug>
#include <algorithm>


QString Resources::badgeAdmin;
QString Resources::badgeBroadcaster;
QString Resources::badgeBot;
QString Resources::badgeGlobalmod;
QString Resources::badgeModerator;
QString Resources::badgePremium;
QString Resources::badgeStaff;
QString Resources::badgeTurbo;


Resources::Resources(){

}


void Resources::load() {
    badgeAdmin = "qrc:/badges/admin.png";
    badgeBot = "https://static-cdn.jtvnw.net/badges/v1/df9095f6-a8a0-4cc2-bb33-d908c0adffb8/1";
    badgeBroadcaster = "qrc:/badges/broadcaster.png";
    badgeModerator = "qrc:/badges/moderator.png";
    badgeGlobalmod = "qrc:/badges/globalmod.png";
    badgeTurbo = "qrc:/badges/turbo.png";
    badgePremium = "qrc:/badges/premium.png";
    badgeStaff = "qrc:/badges/staff.png";

    /*
    https://badges.twitch.tv/v1/badges/global/display?language=en
    badgeAdmin = "https://static-cdn.jtvnw.net/badges/v1/9ef7e029-4cdf-4d4d-a0d5-e2b3fb2583fe/1";
    badgeBroadcaster = "https://static-cdn.jtvnw.net/badges/v1/5527c58c-fb7d-422d-b71b-f309dcb85cc1/1";
    badgeBroadcaster = "qrc:/ChatWindow/broadcaster_bg.png";
    badgeModerator = "https://static-cdn.jtvnw.net/badges/v1/3267646d-33f0-4b17-b3df-f923a41db1d0/1";
    badgeGlobalmod = "https://static-cdn.jtvnw.net/badges/v1/9384c43e-4ce7-4e94-b2a1-b93656896eba/1";
    badgeTurbo = "https://static-cdn.jtvnw.net/badges/v1/bd444ec6-8f34-4bf9-91f4-af1e3428d80f/1";
    badgePremium = "https://static-cdn.jtvnw.net/badges/v1/a1dd5073-19c3-4911-8cb4-c464a7bc1510/1";
    badgeStaff = "https://static-cdn.jtvnw.net/badges/v1/d97c37bd-a6f5-4c38-8f57-4e4bef88af34/1";
    */
}

QColor Resources::calculateColor(QColor color) {
    while (calculateColorBackground(color) == "light")
        color = calculateColorReplacement(color, calculateColorBackground(color));
    return color;
}

QString Resources::calculateColorBackground(QColor color) {
    // Converts HEX to YIQ to judge what color background the color would look best on
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    double yiq = ((r * 299) + (g * 587) + (b * 114)) / 1000;
    return (yiq >= 128) ? "dark" : "light";
}

QColor Resources::calculateColorReplacement(QColor color, QString backgroundColor) {
    bool light = backgroundColor == "light";
    double r = color.red() / 255, g = color.green() / 255, b = color.blue() / 255;
    double factor = light ? 0.1 : -0.1,
            h = color.hslHueF(),
            s = color.hslSaturationF(),
            l = color.lightnessF();
    //lightens dark colors, while still not affecting pure black.
    l = light ? 1 - ((1 - factor) * (1 - l)) : (1 + factor) * l;
    l = std::min(std::max(0.0, l), 1.0);
    color.setHslF(h, s, l);
    return color;
}
