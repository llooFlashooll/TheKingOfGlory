#pragma once

#include <string>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SpriteBase.h"
#include "BulletBase.h"

USING_NS_CC;
using namespace ui;

//��ʼ����ֵ
const float TOWER_ATTACK_RADIUS =1500;
const float TOWER_DAMAGE = 40;
const float TOWER_HPVALUE = 2000;
const int TOWER_ATTACK_INTERVAL = 180;


class Tower :public SpriteBase
{
public:
	static Tower* createWithSpriteFrameName(const std::string& filename,int color);


	virtual bool init(int color);

	virtual bool attack();
	virtual float beAttack(const float damage);
	void destroy();

	virtual bool playDestoryAnimation();

	void setHPBar();
	void updateHPBar();

	void initAnimation();

	bool insideAttack(SpriteBase*beAttacker);


private:


};