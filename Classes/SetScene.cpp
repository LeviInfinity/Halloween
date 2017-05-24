#include"cocos2d.h"
#include"SetScene.h"
#include"Define.h"
#include"WelcomeScene.h"
#include "SimpleAudioEngine.h"//����������
using namespace CocosDenshion;//�����ദ�ڸ������ռ���
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
	pSprite->setPosition(Vec2(Screen_Width / 2, Screen_Height / 2));	//ͨ���궨�壬��֤ͼƬ����Ļ�м�
	this->addChild(pSprite);
	//��Ӵ����ְ�ť
	auto openMusicItem = MenuItemImage::create("res/openMusic.png", "res/openMusic2.png", CC_CALLBACK_1(SetScene::onOpenMusicCallback, this));
	openMusicItem->setPosition(Vec2(Screen_Width / 3 -20, Screen_Height / 3*2));
	menu = Menu::create(openMusicItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//��ӹر����ְ�ť
	auto closeMusicItem = MenuItemImage::create("res/closeMusic.png", "res/closeMusic2.png", CC_CALLBACK_1(SetScene::onCloseMusicCallback, this));
	closeMusicItem->setPosition(Vec2(Screen_Width /3*2+20 , Screen_Height / 3*2));
	menu = Menu::create(closeMusicItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//����Ч
	auto openEffectItem = MenuItemImage::create("res/openEffect.png", "res/openEffect2.png", CC_CALLBACK_1(SetScene::onOpenEffectCallback, this));
	openEffectItem->setPosition(Vec2(Screen_Width / 3-20 , Screen_Height / 3));
	menu = Menu::create(openEffectItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//�ر���Ч
	auto closeEffectItem = MenuItemImage::create("res/closeEffect.png", "res/closeEffect2.png", CC_CALLBACK_1(SetScene::onCloseEffectCallback, this));
	closeEffectItem->setPosition(Vec2(Screen_Width / 3*2+20, Screen_Height / 3));
	menu = Menu::create(closeEffectItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//���÷��ذ�ť
	auto bakcItem = MenuItemImage::create("res/back1.png", "res/back2.png", CC_CALLBACK_1(SetScene::onBackItemCallback, this));
	bakcItem->setPosition(Vec2(Screen_Width / 2 + 10, Screen_Height / 8 - 30));
	menu = Menu::create(bakcItem, NULL);//�˵���ŵ��˵���
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
	TransitionTurnOffTiles * ptrans = TransitionTurnOffTiles::create(1.0f, scene);//�����л������ķ�ʽ,�鱾 P25
	pDirector->replaceScene(ptrans);	//�л�����
										//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
