#include "LoadingScene.h"
#include "SimpleAudioEngine.h"
#include "LoginScene.h"

USING_NS_CC;
using namespace CocosDenshion;


cocos2d::Scene * LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingScene::init()
{
	if (!Layer::init())
		return false;
	visible_Size = Director::getInstance()->getVisibleSize();
	
	createBackground();
	createProgressBar();

	return true;
}

void LoadingScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	

	//����Ϸ��Դ�ļ�������ValueVector��
	ValueVector spriteSheets, effects, musics;

//	spriteSheets.push_back(Value("animation.plist"));


	musics.push_back(Value("Sounds/StartBgm.mp3"));
	musics.push_back(Value("Sounds/GameBgm.mp3"));
	musics.push_back(Value("Sounds/LoginBgm.mp3"));

	effects.push_back(Value("Sounds/LoginClick.wav"));
	effects.push_back(Value("Sounds/ButtonClick.wav"));
	effects.push_back(Value("Sounds/Win.wav"));
	effects.push_back(Value("Sounds/Lose.wav"));

	sourceCount = spriteSheets.size() + effects.size() + musics.size();

	progress_Interval = 100 / sourceCount;
	
	loadMusic(musics);
	loadEffect(effects);
//	loadSpriteSheets(spriteSheets);
	

}

void LoadingScene::createBackground()
{
	//���ӱ���ͼ
	auto background = Sprite::create("Pictures/Background/LoadingBackground.png");
	background->setPosition(Vec2(visible_Size.width / 2, visible_Size.height / 2));
	this->addChild(background, -1);
}

void LoadingScene::createProgressBar()
{
	//����������
	auto barSprite = Sprite::create("Pictures/UI/Bar.png");
	progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setMidpoint(Point(0.0f, 0.5f));
	progress->setBarChangeRate(Point(1.0f, 0.0f));
	progress->setType(ProgressTimer::Type::BAR);
	progress->setPosition(Vec2(visible_Size.width / 2, visible_Size.height / 2));
	this->addChild(progress);
}

void LoadingScene::loadMusic(ValueVector musicFiles)
{
	//��������
	for (Value& v : musicFiles)
	{
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(v.asString().c_str());
		progressUpdate();
	}
}

void LoadingScene::loadEffect(ValueVector effectFiles)
{
	//������Ч
	for (Value &v : effectFiles) {
		SimpleAudioEngine::getInstance()->preloadEffect(v.asString().c_str());
		progressUpdate();
	}
}

void LoadingScene::loadSpriteSheets(ValueVector spriteFiles)
{
	//���ؾ������
	for (Value &v : spriteFiles) {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
		progressUpdate();
	}
}

void LoadingScene::progressUpdate()
{
	if (--sourceCount) {
		//����ʣ�����Դ�����ý���������
		progress->setPercentage(100.0f - (progress_Interval * sourceCount));
	}
	else {
		//��Ϸ��Դ������ϣ��л�����
		auto pft = ProgressFromTo::create(0.5f, progress->getPercentage(), 100);

		auto callFunc = CallFunc::create([=] {

			auto delay = DelayTime::create(2.0f);
			const auto transition = TransitionFade::create(1, LoginScene::createScene());
			auto action = Sequence::create(delay, transition, NULL);
			Director::getInstance()->replaceScene(transition);
		});
		auto action = Sequence::create(pft, callFunc, NULL);

		progress->runAction(action);
	}
}