#pragma once


#include "cocos2d.h"
#include "Soldier.h"
#include "Player.h"

USING_NS_CC;

//炮车的初始属性值
const float GUNCAR_ATTACK_RADIUS = 350;
const float GUNCAR_DAMAGE = 300;
const float GUNCAR_HPVALUE = 2500.0;
const int GUNCAR_ATTACK_INTERVAL = 100;
const float GUNCAR_MOVE_SPEED = 5;
const float GUNCAR_DEFEND = 0.15;
//击杀经验、金钱
const int GUNCAR_KILL_EXPRIENCE =80;
const int GUNCAR_KILL_MONEY = 80;

class GunCar :public Soldier
{
public:
	virtual bool init(int color);
	static GunCar* createWithSpriteFrameName(const std::string& filename, int color);

	virtual void setHPBar();
	virtual void updateHPBar();

	virtual void startMove();
	virtual void stopMove();

	virtual float beAttack(const float damage);

	virtual bool attack();//返回是否进行了攻击
	virtual void stopAttack();

	virtual void initAnimation();
	virtual void move();

	bool insideAttack(SpriteBase*beAttacker);

private:




};