#include "GameMap.h"
USING_NS_CC;


bool GameMap::initGrid()
{

	for (int i = 0; i < tileMap->getMapSize().width; i++)
	{
		Vector<Grid*> inner;
		for (int j = 0; j < tileMap->getMapSize().height; j++)
		{
			Grid *grid = Grid::create(i, j);
			grid->setAssess(isCanAssess(Vec2(i, j)));
			inner.pushBack(grid);
		}
		_gridVector.push_back(inner);
	}
	return true;

}

bool GameMap::init()
{
	this->setName("map");

	auto viewCenterListener = EventListenerCustom::create("ViewCenter", CC_CALLBACK_0(GameMap::setViewPointCenter, this));
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(viewCenterListener, 1);


	return true;
}

void GameMap::setMap(const std::string& mapName)
{
	std::string path = "Pictures/GameItem/map/" + mapName + ".tmx";
	tileMap = TMXTiledMap::create(path);
	this->addChild(tileMap, -1);

	collidable = tileMap->getLayer("collidable");
	collidable->setVisible(false);

	objectLayer = tileMap->getObjectGroup("objects");

	player_red = objectLayer->getObject("player_red");
	player_blue = objectLayer->getObject("player_blue");
	tower_red = objectLayer->getObject("tower_red");
	tower_blue = objectLayer->getObject("tower_blue");
	store_red = objectLayer->getObject("store_red");
	store_blue = objectLayer->getObject("store_blue");
	buff_red = objectLayer->getObject("red_buff");
	buff_blue = objectLayer->getObject("blue_buff");
	monster_red = objectLayer->getObject("red_monster");
	monster_blue = objectLayer->getObject("blue_monster");

	towers_red.push_back(tower_red);
	towers_blue.push_back(tower_blue);
	for (int i = 1; i < 6; i++)
	{
		towers_red.push_back(objectLayer->getObject("tower_red"+std::to_string(i)));
		towers_blue.push_back(objectLayer->getObject("tower_blue"+std::to_string(i)));
	}

	pathLayer= tileMap->getObjectGroup("soldierPath");
	{
		auto pathM1 = pathLayer->getObject("pathM1");
		{
			std::vector<ValueMap> pathM;
			pathM.push_back(pathM1);
			for (int i = 2; i <= 6; ++i)
			{
				pathM.push_back(pathLayer->getObject("pathM" + std::to_string(i)));
			}
			soldier_red_path.push_back(pathM);
		}
		{
			std::vector<ValueMap> pathL;
			pathL.push_back(pathM1);
			for (int i = 1; i <= 4; ++i)
			{
				pathL.push_back(pathLayer->getObject("pathL" + std::to_string(i)));
			}
			pathL.push_back(pathLayer->getObject("pathM6"));
			soldier_red_path.push_back(pathL);
		}
		{
			std::vector<ValueMap> pathR;
			pathR.push_back(pathM1);
			for (int i = 1; i <= 4; ++i)
			{
				pathR.push_back(pathLayer->getObject("pathL" + std::to_string(i)));
			}
			pathR.push_back(pathLayer->getObject("pathM6"));
			soldier_red_path.push_back(pathR);
		}
	}
	for (auto& path : soldier_red_path)
	{
		std::vector<ValueMap> pathblue;
		for (int i = path.size() - 1; i >= 0; --i)
		{
			pathblue.push_back(ValueMap(path.at(i)));
		}
		soldier_blue_path.push_back(pathblue);
	}

	mapInfo.resize(getMapSize().width);
	for (int i = 0; i < getMapSize().width; i++)
	{
		mapInfo.at(i).resize(getMapSize().height);
	}

	for (int i = 0; i < getMapSize().width; i++)
	{
		for (int j = 0; j < getMapSize().height; j++)
		{
			int tileGid = collidable->getTileGIDAt(Vec2(i, j));
			if (tileGid)
			{
				Value properties = tileMap->getPropertiesForGID(tileGid);
				ValueMap map = properties.asValueMap();
				std::string value = map.at("collidable").asString();
				if (value.compare("true") == 0)
				{
					mapInfo.at(i).at(j) = 1;
				}
			}

		}
	}

	initGrid();



}

Vec2 GameMap::tileCoordToPosition(const Vec2 & coord)
{
	auto mapSize = tileMap->getMapSize();
	auto tileSize = tileMap->getTileSize();

	int x = coord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - coord.y) * tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
}

Vec2 GameMap::positionToTileCoord(const Vec2 & pos)
{
	Size mapSize = tileMap->getMapSize();
	Size tileSize = tileMap->getTileSize();
	int x = pos.x / tileSize.width;
	int y = (mapSize.height * tileSize.height - pos.y) / tileSize.height;
	// edge case
	x = MIN(MAX(0, x), mapSize.width - 1);
	y = MIN(MAX(0, y), mapSize.height - 1);
	return Vec2(x, y);
}

Vec2 GameMap::centrePos(const Vec2 & pos)
{
	return tileCoordToPosition(positionToTileCoord(pos));
}

cocos2d::Size GameMap::getMapSize()
{
	return tileMap->getMapSize();
}

cocos2d::Size GameMap::getTileSize()
{
	return tileMap->getTileSize();
}

bool GameMap::isCanAssess(const cocos2d::Vec2 & coord)
{
	return coord.x >= 0 && coord.x < getMapSize().width
		&&coord.y >= 0 && coord.y < getMapSize().height
		&& ((mapInfo.at(coord.x).at(coord.y) == 1) ? false : true);
}

void GameMap::addSprite(cocos2d::Sprite * sprite, Type type)
{
	if (tileMap)
		tileMap->addChild(sprite);
	switch (type)
	{
	case Type::Player_Blue:
	{
		sprite->setPosition(Vec2(player_blue.at("x").asFloat(), player_blue.at("y").asFloat()));
		sprite->setLocalZOrder(3);
	}
	break;
	case Type::Player_Red:
	{
		sprite->setPosition(Vec2(player_red.at("x").asFloat(), player_red.at("y").asFloat()));
		sprite->setLocalZOrder(3);
	}
	break;
	case Type::Soldier_Red:
	{
		sprite->setPosition(Vec2(store_red.at("x").asFloat(), store_red.at("y").asFloat()));
		sprite->setLocalZOrder(2);
	}
	break;
	case Type::Solider_Blue:
	{
		sprite->setPosition(Vec2(store_blue.at("x").asFloat(), store_blue.at("y").asFloat()));
		sprite->setLocalZOrder(2);
	}
	break;
	case Type::Tower_Blue:
	{
		sprite->setPosition(Vec2(tower_blue.at("x").asFloat(), tower_blue.at("y").asFloat()));
		sprite->setLocalZOrder(1);
	}
	break;
	case Type::Tower_Red:
	{
		sprite->setPosition(Vec2(tower_red.at("x").asFloat(), tower_red.at("y").asFloat()));
		sprite->setLocalZOrder(1);
	}
	break;
	case Type::Buff_Red:
	{
		sprite->setPosition(Vec2(buff_red.at("x").asFloat(), buff_red.at("y").asFloat()));
		sprite->setLocalZOrder(2);
	}
	break;
	case Type::Buff_Blue:
	{
		sprite->setPosition(Vec2(buff_blue.at("x").asFloat(), buff_blue.at("y").asFloat()));
		sprite->setLocalZOrder(2);
	}
	break;
	case Type::Monster_Red:
	{
		sprite->setPosition(Vec2(monster_red.at("x").asFloat(), monster_red.at("y").asFloat()));
		sprite->setLocalZOrder(2);
	}
	break;
	case Type::Monster_Blue:
	{
		sprite->setPosition(Vec2(monster_blue.at("x").asFloat(), monster_blue.at("y").asFloat()));
		sprite->setLocalZOrder(2);
	}
	break;
	}

}


void GameMap::setSpritePosition(cocos2d::Sprite * sprite, Type type)
{
	switch (type)
	{
	case Type::Player_Blue:
	{
		sprite->setPosition(Vec2(player_blue.at("x").asFloat(), player_blue.at("y").asFloat()));
	}
	break;
	case Type::Player_Red:
	{
		sprite->setPosition(Vec2(player_red.at("x").asFloat(), player_red.at("y").asFloat()));

	}
	break;
	case Type::Soldier_Red:
	{
		sprite->setPosition(Vec2(store_red.at("x").asFloat(), store_red.at("y").asFloat()));
	}
	break;
	case Type::Solider_Blue:
	{
		sprite->setPosition(Vec2(store_blue.at("x").asFloat(), store_blue.at("y").asFloat()));
	}
	break;
	case Type::Tower_Blue:
	{
		sprite->setPosition(Vec2(tower_blue.at("x").asFloat(), tower_blue.at("y").asFloat()));
	}
	break;
	case Type::Tower_Red:
	{
		sprite->setPosition(Vec2(tower_red.at("x").asFloat(), tower_red.at("y").asFloat()));
	}
	break;
	}
}

cocos2d::Vec2 GameMap::getObjectPosition(Type type)
{
	Vec2 position;
	switch (type)
	{
	case Type::Player_Blue:
	{
		position=Vec2(player_blue.at("x").asFloat(), player_blue.at("y").asFloat());
	}
	break;
	case Type::Player_Red:
	{
		position = Vec2(player_red.at("x").asFloat(), player_red.at("y").asFloat());

	}
	break;
	case Type::Soldier_Red:
	{
		position = Vec2(store_red.at("x").asFloat(), store_red.at("y").asFloat());
	}
	break;
	case Type::Solider_Blue:
	{
		position = Vec2(store_blue.at("x").asFloat(), store_blue.at("y").asFloat());
	}
	break;
	case Type::Tower_Blue:
	{
		position = Vec2(tower_blue.at("x").asFloat(), tower_blue.at("y").asFloat());
	}
	break;
	case Type::Tower_Red:
	{
		position = Vec2(tower_red.at("x").asFloat(), tower_red.at("y").asFloat());
	}
	break;
	}

	return position;
}

std::vector<std::vector<ValueMap>>& GameMap::getSoldierPath(int color)
{
	if (color == 0)
		return soldier_red_path;
	else
		return soldier_blue_path;
}

void GameMap::setViewPointCenter()
{
	if (_centerSprite)
	{
		Vec2 spritePos = _centerSprite->getPosition();
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 mapPos = Vec2(visibleSize.width/2 - spritePos.x*getScaleX(),
			visibleSize.height/2 - spritePos.y*getScaleY());
		if (mapPos.x > 0)
			mapPos.x = 0;
		else if(mapPos.x < visibleSize.width-getMapSize().width*getTileSize().width*getScaleX() )
			mapPos.x = visibleSize.width - getMapSize().width*getTileSize().width*getScaleX();
		if (mapPos.y > 0)
			mapPos.y = 0;
		else if (mapPos.y < visibleSize.height - getMapSize().height*getTileSize().height*getScaleY())
			mapPos.y = visibleSize.height - getMapSize().height*getTileSize().height*getScaleY();
		this->setPosition(mapPos);
	}
}


GameMap * GameMap::getCurrentMap()
{
	auto map = dynamic_cast<GameMap*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName("GameScene")->getChildByName("map"));
	if (map)
	{
		return map;
	}
	return nullptr;
}

void GameMap::addSprite(cocos2d::Sprite * sprite, int zOrder)
{
	tileMap->addChild(sprite, zOrder);
}

void GameMap::addTower(cocos2d::Sprite * tower,int color, int tag)
{
	if (tileMap)
		tileMap->addChild(tower);
	ValueMap towerValue;
	if (color == 0)
		towerValue = towers_red.at(tag);
	else
		towerValue = towers_blue.at(tag);
	tower->setPosition(Vec2(towerValue.at("x").asFloat(), towerValue.at("y").asFloat()));
	tower->setLocalZOrder(1);
}
