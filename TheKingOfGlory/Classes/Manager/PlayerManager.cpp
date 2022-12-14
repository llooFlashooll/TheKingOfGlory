#include"PlayerManager.h"
#include "Model/GameMap.h"
#include "Model/Hero.h"
#include "Model/User.h"

USING_NS_CC;



bool PlayerManager::init()
{
	if (!Layer::init())
	{
		return false;
	}
	isOnline = UserDefault::getInstance()->getBoolForKey("Network");

	addCustomEvent();


	scheduleOnce(schedule_selector(PlayerManager::initPlayer),0.1f);

	return true;
}

Player* PlayerManager::createPlayer(const std::string& id, int role,int color)
{
	Player *player;
	switch (role)
	{
	case 0: {player = Warrior::create(id, color);	break; }
	case 1: {player = Aviator::create(id, color);	break; }
	case 2: {player = Mage::create(id, color);	break; }
	case 3: {player = Paladin::create(id, color);	break; }
	case 4: {player = Ranger::create(id, color);	break; }
	case 5: {player = Cavalier::create(id, color);	break; }
	}
	if (player)
	{
		player->isLocal(false);
		this->_playerList.insert(id, player);
		return player;
	}

	CC_SAFE_DELETE(player);
	return nullptr;
}

Player* PlayerManager::createLocalPlayer(const std::string& id, int role,int color)
{
	Player *localPlayer;
	switch (role)
	{
	case 0: {localPlayer = Warrior::create(id, color);	break; }
	case 1: {localPlayer = Aviator::create(id, color);	break; }
	case 2: {localPlayer = Mage::create(id, color);	break; }
	case 3: {localPlayer = Paladin::create(id, color);	break; }
	case 4: {localPlayer = Ranger::create(id, color);	break; }
	case 5: {localPlayer = Cavalier::create(id, color);	break; }
	}
	if (localPlayer)
	{
		_localPlayer = localPlayer;
		localPlayer->isLocal(true);
		this->_playerList.insert(id, localPlayer);
	}

	return localPlayer;
}

void PlayerManager::setStatus(const std::string& id, Player::Status status)
{
	auto player = this->getPlayer(id);
	if (player)
	{
		player->setStatus(status);
	}
}

Player * PlayerManager::getPlayer(const std::string& id)
{
	return _playerList.at(id);
}


Map<std::string, Player*>& PlayerManager::getPlayerList()
{
	return _playerList;
}

Player * PlayerManager::getLocalPlayer()
{
	return _localPlayer;
}

void PlayerManager::addCustomEvent()
{
}

void PlayerManager::initPlayer(float delta)
{
	if (isOnline)
	{

	}
	else
	{
		srand(time(NULL));
		auto player = this->createPlayer("Enemy", rand()%4, BLUE);
		GameMap::getCurrentMap()->addSprite(player, GameMap::Type::Player_Blue);

		auto role = User::getInstance()->getRole();
		this->createLocalPlayer(UserDefault::getInstance()->getStringForKey("username"), role, RED);
		GameMap::getCurrentMap()->addSprite(this->getLocalPlayer(), GameMap::Type::Player_Red);
		GameMap::getCurrentMap()->addCenterSprite(this->getLocalPlayer());
	}


}
