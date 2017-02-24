#include "userstate.h"

UserState::UserState()
{
}

UserState::UserState(const QString _displayName, bool _mod, bool _sub, bool _turbo)
                    : displayName(_displayName)
                    , mod(_mod)
                    , sub(_sub)
                    , turbo(_turbo)
{
}

QString UserState::getDisplayName() const {
    return displayName;
}

bool UserState::isMod() const
{
    return mod;
}

bool UserState::isSub() const
{
    return sub;
}

bool UserState::isTurbo() const
{
    return turbo;
}
