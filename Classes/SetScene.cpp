#include"cocos2d.h"
#include"SetScene.h"
#include"Define.h"
#include"WelcomeScene.h"
#include "SimpleAudioEngine.h"//声音管理类
using namespace CocosDenshion;//声音类处于该命名空间中
USING_NS_CC;
bool SetScene::isEffectPlay = true;
bool SetScene::isMusicPlay = true;
Scene* SetScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SetScene::create();
	scene->addChild(layer);
	return scene;
}
bool SetScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	cocos2d::Menu *menu;
	auto pDirector = Director::getInstance();
	auto pSprite = Sprite::create("res/Set.png");
	pSprite->setPosition(Vec2(Screen_Width / 2, Screen_Height / 2));	//通过宏定义，保证图片在屏幕中间
	this->addChild(pSprite);
	//添加打开音乐按钮
	auto openMusicItem = MenuItemImage::create("res/openMusic.png", "res/openMusic2.png", CC_CALLBACK_1(SetScene::onOpenMusicCallback, this));
	openMusicItem->setPosition(Vec2(Screen_Width / 3 -20, Screen_Height / 3*2));
	menu = Menu::create(openMusicItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//添加关闭音乐按钮
	auto closeMusicItem = MenuItemImage::create("res/closeMusic.png", "res/closeMusic2.png", CC_CALLBACK_1(SetScene::onCloseMusicCallback, this));
	closeMusicItem->setPosition(Vec2(Screen_Width /3*2+20 , Screen_Height / 3*2));
	menu = Menu::create(closeMusicItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//打开音效
	auto openEffectItem = MenuItemImage::create("res/openEffect.png", "res/openEffect2.png", CC_CALLBACK_1(SetScene::onOpenEffectCallback, this));
	openEffectItem->setPosition(Vec2(Screen_Width / 3-20 , Screen_Height / 3));
	menu = Menu::create(openEffectItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//关闭音效
	auto closeEffectItem = MenuItemImage::create("res/closeEffect.png", "res/closeEffect2.png", CC_CALLBACK_1(SetScene::onCloseEffectCallback, this));
	closeEffectItem->setPosition(Vec2(Screen_Width / 3*2+20, Screen_Height / 3));
	menu = Menu::create(closeEffectItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//设置返回按钮
	auto bakcItem = MenuItemImage::create("res/back1.png", "res/back2.png", CC_CALLBACK_1(SetScene::onBackItemCallback, this));
	bakcItem->setPosition(Vec2(Screen_Width / 2 + 10, Screen_Height / 8 - 30));
	menu = Menu::create(bakcItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}
void SetScene::onOpenMusicCallback(Ref * pSender)
{
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	isMusicPlay = true;
}
void SetScene::onCloseMusicCallback(Ref * pSender)
{	
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	isMusicPlay = false;
}
void SetScene::onOpenEffectCallback(Ref * pSender)
{
	isEffectPlay = true;
}
void SetScene::onCloseEffectCallback(Ref * pSender)
{
	isEffectPlay = false;
}
void SetScene::onBackItemCallback(Ref * pSender)
{
	auto pDirector = Director::getInstance();
	auto scene = WelcomeScene::createScene();
	TransitionTurnOffTiles * ptrans = TransitionTurnOffTiles::create(1.0f, scene);//设置切换场景的方式,书本 P25
	pDirector->replaceScene(ptrans);	//切换场景
										//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
