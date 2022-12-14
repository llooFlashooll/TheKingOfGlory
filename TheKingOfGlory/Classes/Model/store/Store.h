#pragma once

#include "../Tower.h"
#include "EquipmentBase.h"
#include "../GameMap.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../StatusList.h"

USING_NS_CC;
using namespace ui;

static bool VIP = false;

class EquipmentBase;

class Store:public Tower
{
public:
	static Store* createWithSpriteFrameName(const std::string& filename, int color);
	virtual bool init(int color);

	void addEquipment(EquipmentBase*equip) { _equipmentList.pushBack(equip); }//AAAAAAAAAAAAA

	//CREATE_FUNC(Store);
private:
	EventListenerTouchOneByOne* listener=nullptr;
	void createListener();
	void onTouchEnded(Touch*touch, Event*event);
	bool onTouchBegan(Touch*touch, Event*event);

	Vector<EquipmentBase*> _equipmentList;



	void createBg();
	void removeBg();
	void destroy();
	Size _visibleSize;
	Sprite* _bg;
	Button* _closeButton;
	Button* _vipButton;

};

