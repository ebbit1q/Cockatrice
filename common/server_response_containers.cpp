#include "server_response_containers.h"

#include "server_game.h"

#include <google/protobuf/descriptor.h>

GameEventStorageItem::GameEventStorageItem(const ::google::protobuf::Message &_event,
                                           int _playerId,
                                           EventRecipients _recipients)
    : event(new GameEvent), recipients(_recipients)
{
    event->GetReflection()->MutableMessage(event, _event.GetDescriptor()->FindExtensionByName("ext"))->CopyFrom(_event);
    event->set_player_id(_playerId);
}

GameEventStorageItem::~GameEventStorageItem()
{
    delete event;
}

GameEventStorage::GameEventStorage() : gameEventContext(nullptr), privatePlayerId(0)
{
}

GameEventStorage::~GameEventStorage()
{
    delete gameEventContext;
    for (auto *event : gameEventList)
        delete event;
}

void GameEventStorage::setGameEventContext(const ::google::protobuf::Message &_gameEventContext)
{
    delete gameEventContext;
    gameEventContext = new GameEventContext;
    gameEventContext->GetReflection()
        ->MutableMessage(gameEventContext, _gameEventContext.GetDescriptor()->FindExtensionByName("ext"))
        ->CopyFrom(_gameEventContext);
}

void GameEventStorage::enqueueGameEvent(const ::google::protobuf::Message &event,
                                        int playerId,
                                        GameEventStorageItem::EventRecipients recipients,
                                        int _privatePlayerId)
{
    gameEventList.append(new GameEventStorageItem(event, playerId, recipients));
    if (_privatePlayerId != -1)
        privatePlayerId = _privatePlayerId;
}

void GameEventStorage::sendToGame(Server_Game *game)
{
    if (gameEventList.isEmpty())
        return;

    GameEventContainer *contPrivate = new GameEventContainer;
    GameEventContainer *contOthers = new GameEventContainer;
    int id = privatePlayerId;
    if (forcedByJudge != -1) {
        contPrivate->set_forced_by_judge(forcedByJudge);
        contOthers->set_forced_by_judge(forcedByJudge);
        id = forcedByJudge;
    }
    for (int i = 0; i < gameEventList.size(); ++i) {
        const GameEvent &event = gameEventList[i]->getGameEvent();
        const GameEventStorageItem::EventRecipients recipients = gameEventList[i]->getRecipients();
        if (recipients.testFlag(GameEventStorageItem::SendToPrivate))
            contPrivate->add_event_list()->CopyFrom(event);
        if (recipients.testFlag(GameEventStorageItem::SendToOthers))
            contOthers->add_event_list()->CopyFrom(event);
    }
    if (gameEventContext) {
        contPrivate->mutable_context()->CopyFrom(*gameEventContext);
        contOthers->mutable_context()->CopyFrom(*gameEventContext);
    }
    game->sendGameEventContainer(contPrivate, GameEventStorageItem::SendToPrivate, id);
    game->sendGameEventContainer(contOthers, GameEventStorageItem::SendToOthers, id);
}

ResponseContainer::ResponseContainer(int _cmdId) : cmdId(_cmdId), responseExtension(nullptr)
{
}

ResponseContainer::~ResponseContainer()
{
    delete responseExtension;
    for (auto &item : preResponseQueue)
        delete item.second;
    for (auto &item : postResponseQueue)
        delete item.second;
}
