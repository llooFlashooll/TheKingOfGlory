#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "StartScene.h"
#include "../Model/GameMap.h"
#include "../Controller/GameController.h"
USING_NS_CC;
using namespace CocosDenshion;



void GameScene::onEnter()
{
	Layer::onEnter();
	//GameAudio::getInstance()->playBgm("Sounds/Game.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/GameBgm.mp3", true);
}


void GameScene::createMenuButton()
{
	//���Ӳ˵���ť
	auto menuItem = MenuItemImage::create(
		"Pictures/UI/SettingNormal.png",
		"Pictures/UI/SettingSelected.png",
		CC_CALLBACK_0(GameScene::createMenu,this));
	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Vec2(visible_Size.width*0.95, visible_Size.height*0.95));
	this->addChild(menu);
	menuRect= Rect(menu->getPosition().x - menuItem->getContentSize().width / 2,
		menu->getPosition().y - menuItem->getContentSize().height / 2,
		menuItem->getContentSize().width + menuItem->getContentSize().width / 2,
		menuItem->getContentSize().height + menuItem->getContentSize().height / 2);
}

void GameScene::updateMenu()
{
	//���²˵������յ�UpdateMenu�¼�����Ӧ
	if (hasMenu)
		removeMenu();
	else
		createMenu();
}

void GameScene::createMenu()
{
	if (hasMenu)
		return;

	menu = Sprite::create("Pictures/UI/Menu1.png");
	menu->setPosition(Vec2(visible_Size.width / 2, visible_Size.height / 2));
	this->addChild(menu);

	Color4B text_Color= Color4B(255,255,255,255);
	std::string text_Font = "fonts/UnifrakturCook-Bold.ttf";
	float text_Size = 28;
	//���Ӳ˵�ͼƬ
	Size menuSize = menu->getContentSize();
	float menuBottom = visible_Size.height / 2 - menuSize.height / 2;

	//���Ӽ�����Ϸ����
	continueLabel = Label::createWithTTF("Continue", text_Font, text_Size);
	continueLabel->setTextColor(text_Color);
	continueLabel->setPosition(Vec2(visible_Size.width / 2, menuBottom + menuSize.height*0.8));
	this->addChild(continueLabel);

	//������Ϸ��������
	settingLabel = Label::createWithTTF("Setting", text_Font, text_Size);
	settingLabel->setTextColor(text_Color);
	settingLabel->setPosition(Vec2(visible_Size.width / 2, menuBottom + menuSize.height*0.6));
	this->addChild(settingLabel);

	//�������¿�ʼ����
	restartLabel = Label::createWithTTF("Restart", text_Font, text_Size);
	restartLabel->setTextColor(text_Color);
	restartLabel->setPosition(Vec2(visible_Size.width / 2, menuBottom + menuSize.height*0.4));
	this->addChild(restartLabel);

	//���ӷ������˵�����
	returnLabel = Label::createWithTTF("Main Menu", text_Font, text_Size);
	returnLabel->setTextColor(text_Color);
	returnLabel->setPosition(Vec2(visible_Size.width / 2, menuBottom + menuSize.height*0.2));
	this->addChild(returnLabel);

	menuListener->setEnabled(true);
	hasMenu = true;

}

void GameScene::removeMenu()
{
	if (!hasMenu)
		return;
	//ͣ�ò˵��¼���������ɾ���˵�
	menuListener->setEnabled(false);
	this->removeChild(menu, true);
	this->removeChild(continueLabel, true);
	this->removeChild(settingLabel, true);
	this->removeChild(restartLabel, true);
	this->removeChild(returnLabel, true);
	hasMenu = false;
}

void GameScene::createResultBox(bool isWin)
{
}

void GameScene::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto touchLocation = touch->getLocation();
	auto nodeLocation = this->convertToNodeSpace(touchLocation);

	if (this->rectOfLabel(continueLabel).containsPoint(nodeLocation)||
		menuRect.containsPoint(nodeLocation))
	{
		removeMenu();
	}
	else if (this->rectOfLabel(settingLabel).containsPoint(nodeLocation))
	{
		//ͣ�ò˵��¼���������ɾ���˵�,������Ϸ���ý���
		//removeMenu();
		//Director::getInstance()->pushScene(TransitionFade::create(1, SettingScene::createScene()));
	}
	else if (this->rectOfLabel(restartLabel).containsPoint(nodeLocation))
	{
		//ͣ�ò˵��¼������������¿�ʼ��Ϸ
		menuListener->setEnabled(false);
		hasMenu = false;
		Director::getInstance()->replaceScene(TransitionFade::create(1, GameScene::createScene()));
	}
	else if (this->rectOfLabel(returnLabel).containsPoint(nodeLocation))
	{
		//ͣ�ò˵��¼����������������˵�
		menuListener->setEnabled(false);
		hasMenu = false;
		Director::getInstance()->replaceScene(TransitionFade::create(1,	StartScene::createScene()));
	}
}

cocos2d::Rect GameScene::rectOfLabel(cocos2d::Label * label)
{
	return Rect(label->getPosition().x - label->getContentSize().width / 2,
		label->getPosition().y - label->getContentSize().height / 2,
		label->getContentSize().width,
		label->getContentSize().height);
}

cocos2d::Scene * GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
		return false;
	visible_Size = Director::getInstance()->getVisibleSize();

	//�����˵��¼����������Ȳ�����
	menuListener = EventListenerTouchOneByOne::create();
	menuListener->onTouchBegan = [](Touch* touch, Event* event) {return true; };
	menuListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	menuListener->setSwallowTouches(true);
	menuListener->setEnabled(false);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(menuListener, -1);

	//�����Զ����¼������������ڴ򿪹رղ˵�
	auto updateMenuListener = EventListenerCustom::create("UpdateMenu", CC_CALLBACK_0(GameScene::updateMenu, this));
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateMenuListener,1);

	//�����Զ����¼�����������Ϸ����ʱ�����Ի���
	auto gameOverListener = EventListenerCustom::create("GameOver", CC_CALLBACK_1(GameScene::createResultBox, this));
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(gameOverListener, 1);

	//���ӵ�ͼ
	auto map = GameMap::create();
	map->setMap("1v1");
	map->setPosition(Vec2::ZERO);
	map->setScale(0.5f);
	//map->setPosition(Vec2(visible_Size.width / 2-512, visible_Size.height / 2-3052));
	this->addChild(map, -1);

	auto gameController = GameController::create();
	gameController->setMap(map);
	this->addChild(gameController, -1);

	createMenuButton();

	return true;
}