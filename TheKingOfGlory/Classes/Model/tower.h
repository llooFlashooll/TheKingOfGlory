#pragma once

#include <string>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SpriteBase.h"
#include "BulletBase.h"
#include "Scene/GameScene.h"

USING_NS_CC;
using namespace ui;

//³õÊ¼ÊôÐÔÖµ
const float TOWER_ATTACK_RADIUS =800;
const float TOWER_DAMAGE = 50;
const float TOWER_HPVALUE = 5000;
const int TOWER_ATTACK_INTERVAL = 180;
const float TOWER_DEFEND = 0.2;
//击杀经验和金钱
const int TOWER_KILL_EXPRIENCE = 50;
const int TOWER_KILL_MONEY = 100;
//
const float BUFF_ATTACK_RADIUS = 500;
const float BUFF_DAMAGE = 5;
const float BUFF_HPVALUE = 10;
const int BUFF_ATTACK_INTERVAL = 180;
const float BUFF_DEFEND = 0.1;
//击杀经验和金钱
const int BUFF_KILL_EXPRIENCE = 50;
const int BUFF_KILL_MONEY = 100;

const float BLUE_BUFF_ADD_DEFEND = 0.15;
const float RED_BUFF_ADD_DAMAGE = 100;


class Tower :public SpriteBase
{
public:

	static Tower* createWithSpriteFrameName(const std::string& filename,int color, TYPE type = TOWER);


	virtual bool init(int color,TYPE type= TOWER);

	virtual bool attack();
	virtual float beAttack(const float damage);
	void destroy();

	void setHPBar();
	void updateHPBar();

	void initAnimation();

	bool insideAttack(SpriteBase*beAttacker);

	void runAttackAnimation();
	void stopAttackAnimation();
	
	void runStandAnimation();
	void stopStandAnimation();

private:

};