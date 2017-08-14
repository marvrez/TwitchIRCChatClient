#include "completionmanager.h"
#include "Messages/message.h"
#include <memory>

void CompletionModel::addString(const QString& str)
{
    // Always add a space at the end of completions
    this->emotes.push_back(str + " ");
}

CompletionModel* CompletionManager::createModel(const QString& channelName)
{
    CompletionModel* model = new CompletionModel();
    model->emotes.clear();

    EmoteManager* manager = &Message::emote_manager;

    auto twitchEmotes = manager->getTwitchEmotes();
    for (const auto& emote : twitchEmotes) model->addString(emote.code);

    auto bttvGlobalEmotes = manager->getBttvEmotes();
    for (const auto& emote : bttvGlobalEmotes) model->addString(emote.code);

    auto ffzGlobalEmotes = manager->getFfzEmotes();
    for (const auto& emote : ffzGlobalEmotes) model->addString(emote.code);

    auto bttvChannelEmotes = manager->getBttvChannelEmotes().value(channelName);
    for (const auto& emote : bttvChannelEmotes) model->addString(emote.code);

    auto ffzChannelEmotes = manager->getFfzChannelEmotes().value(channelName);
    for (const auto& emote : ffzChannelEmotes) model->addString(emote.code);

    return model;
}
