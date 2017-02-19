#include "resources.h"

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
    //https://badges.twitch.tv/v1/badges/global/display?language=en
    badgeAdmin = "https://static-cdn.jtvnw.net/badges/v1/9ef7e029-4cdf-4d4d-a0d5-e2b3fb2583fe/1";
    badgeBot = "https://static-cdn.jtvnw.net/badges/v1/df9095f6-a8a0-4cc2-bb33-d908c0adffb8/1";
    badgeBroadcaster = "https://static-cdn.jtvnw.net/badges/v1/5527c58c-fb7d-422d-b71b-f309dcb85cc1/1";
    badgeModerator = "https://static-cdn.jtvnw.net/badges/v1/3267646d-33f0-4b17-b3df-f923a41db1d0/1";
    badgeGlobalmod = "https://static-cdn.jtvnw.net/badges/v1/9384c43e-4ce7-4e94-b2a1-b93656896eba/1";
    badgeTurbo = "https://static-cdn.jtvnw.net/badges/v1/bd444ec6-8f34-4bf9-91f4-af1e3428d80f/1";
    badgePremium = "https://static-cdn.jtvnw.net/badges/v1/a1dd5073-19c3-4911-8cb4-c464a7bc1510/1";
    badgeStaff = "https://static-cdn.jtvnw.net/badges/v1/d97c37bd-a6f5-4c38-8f57-4e4bef88af34/1";
}
