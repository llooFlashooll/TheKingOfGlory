#include "SpriteBase.h"

bool SpriteBase::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	
	return true;
}

SpriteBase* SpriteBase::createWithSpriteFrameName(const std::string& filename)
{
	auto sprite = new SpriteBase();
	if (sprite&&sprite->initWithSpriteFrameName(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
