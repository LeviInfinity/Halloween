#include"cocos2d.h"
#include"WelcomeScene.h"
#include"GameScene.h"
#include"Define.h"
#include "SimpleAudioEngine.h"//����������
#include"SetScene.h"
using namespace CocosDenshion;//�����ദ�ڸ������ռ���
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
	pSprite->setPosition(Vec2(Screen_Width/2,Screen_Height/2));	//ͨ���궨�壬��֤ͼƬ����Ļ�м�
	this->addChild(pSprite);

	//��ӿ�ʼ��ť
	auto pNormalSprite = Sprite::create("res/start.png");	//��������ͼƬ��Ϊ���飬����ť������ѡ��������
	auto pSelectedSprite = Sprite::create("res/start2.png");
	pNormalSprite->setOpacity(220);//͸��������0-255
	pNormalSprite->setScale(0.8f);//ͼƬ̫���ˣ�����һ��
	pSelectedSprite->setScale(0.8f);
	auto StartItem = MenuItemSprite::create(pNormalSprite, pSelectedSprite, nullptr,
		CC_CALLBACK_1(WelcomeScene::onStartItemCallback, this));
	StartItem->setPosition(Vec2(Screen_Width/2 + 15,Screen_Height/6));
	menu = Menu::create(StartItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	//����˳���ť
	auto exitItem = MenuItemImage::create("res/EXIT1.png", "res/EXIT2.png", CC_CALLBACK_1(WelcomeScene::onExitItemCallback, this));
	exitItem->setPosition(Vec2(Screen_Width / 6*5+25, Screen_Height / 8));
	menu = Menu::create(exitItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//������ð�ť
	auto setItem = MenuItemImage::create("res/SET1.png", "res/SET2.png", CC_CALLBACK_1(WelcomeScene::onSetItemCallback, this));
	setItem->setPosition(Vec2(Screen_Width / 8 -20, Screen_Height /8));
	menu = Menu::create(setItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//��ӱ�������
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
	TransitionPageTurn * ptrans = TransitionPageTurn::create(1.0f, scene, true);//�����л������ķ�ʽ,�鱾 P25
	pDirector->replaceScene(ptrans);	//�л�����
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playEffect("res/WelcomeToGame.wav");//Ĭ��Ϊ���ظ�����
}
void WelcomeScene::onSetItemCallback(Ref * pSender)
{
	auto pDirector = Director::getInstance();
	auto scene =SetScene::createScene();
	TransitionPageTurn * ptrans = TransitionPageTurn::create(1.0f, scene, true);//�����л������ķ�ʽ,�鱾 P25
	pDirector->replaceScene(ptrans);	//�л�����
}
void WelcomeScene::onExitItemCallback(Ref * pSender)
{
	Director::getInstance()->end();
}