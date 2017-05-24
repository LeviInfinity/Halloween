#include"cocos2d.h"
#include"WelcomeScene.h"
#include"GameScene.h"
#include"Define.h"
#include "SimpleAudioEngine.h"//声音管理类
#include"SetScene.h"
using namespace CocosDenshion;//声音类处于该命名空间中
USING_NS_CC;

Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}
bool WelcomeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	cocos2d::Menu *menu;
	auto pDirector = Director::getInstance();
	auto pSprite = Sprite::create("res/Welcome.png");
	pSprite->setPosition(Vec2(Screen_Width/2,Screen_Height/2));	//通过宏定义，保证图片在屏幕中间
	this->addChild(pSprite);

	//添加开始按钮
	auto pNormalSprite = Sprite::create("res/start.png");	//导入两张图片作为精灵，代表按钮正常和选定的样子
	auto pSelectedSprite = Sprite::create("res/start2.png");
	pNormalSprite->setOpacity(220);//透明度设置0-255
	pNormalSprite->setScale(0.8f);//图片太大了，缩放一半
	pSelectedSprite->setScale(0.8f);
	auto StartItem = MenuItemSprite::create(pNormalSprite, pSelectedSprite, nullptr,
		CC_CALLBACK_1(WelcomeScene::onStartItemCallback, this));
	StartItem->setPosition(Vec2(Screen_Width/2 + 15,Screen_Height/6));
	menu = Menu::create(StartItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	//添加退出按钮
	auto exitItem = MenuItemImage::create("res/EXIT1.png", "res/EXIT2.png", CC_CALLBACK_1(WelcomeScene::onExitItemCallback, this));
	exitItem->setPosition(Vec2(Screen_Width / 6*5+25, Screen_Height / 8));
	menu = Menu::create(exitItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//添加设置按钮
	auto setItem = MenuItemImage::create("res/SET1.png", "res/SET2.png", CC_CALLBACK_1(WelcomeScene::onSetItemCallback, this));
	setItem->setPosition(Vec2(Screen_Width / 8 -20, Screen_Height /8));
	menu = Menu::create(setItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//添加背景音乐
	if(SetScene::isMusicPlay)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("WelcomeBackground.mp3", true); 

	auto pParticle = ParticleSystemQuad::create("res/fire.plist");
	pParticle->setPosition(Vec2(Screen_Width / 2, 0));
	this->addChild(pParticle);

	return true;
}

void WelcomeScene::onStartItemCallback(Ref * pSender)
{
	auto pDirector = Director::getInstance();
	auto scene = GameScene::createScene();
	TransitionPageTurn * ptrans = TransitionPageTurn::create(1.0f, scene, true);//设置切换场景的方式,书本 P25
	pDirector->replaceScene(ptrans);	//切换场景
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playEffect("res/WelcomeToGame.wav");//默认为不重复播放
}
void WelcomeScene::onSetItemCallback(Ref * pSender)
{
	auto pDirector = Director::getInstance();
	auto scene =SetScene::createScene();
	TransitionPageTurn * ptrans = TransitionPageTurn::create(1.0f, scene, true);//设置切换场景的方式,书本 P25
	pDirector->replaceScene(ptrans);	//切换场景
}
void WelcomeScene::onExitItemCallback(Ref * pSender)
{
	Director::getInstance()->end();
}