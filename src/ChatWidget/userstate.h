#ifndef USERSTATE_H
#define USERSTATE_H

#include <QString>

class UserState
{
public:
    UserState();
    UserState(const QString _displayName, bool _mod, bool _sub, bool _turbo);

    QString getDisplayName() const;
    bool isMod() const;
    bool isSub() const;
    bool isTurbo() const;

private:
    QString displayName;
    bool mod;
    bool sub;
    bool turbo;
};

#endif // USERSTATE_H
