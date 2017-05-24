#pragma once
#include"cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"Define.h"
#include"SpriteShape.h"
USING_NS_CC;
class GameScene :public cocos2d::Layer
{
public:
	static Scene * createScene();
	virtual bool init();
	virtual void onBackItemCallback(Ref * pSender);

	void initMap();
	void createSprite(int r, int c);
	Vec2 PositionOfItem(int r, int c);
	void update(float dt);
	void checkAndRemoveSprite();
	void getColChain(SpriteShape * spr, std::list<SpriteShape*>&chainList);
	void getRowChain(SpriteShape * spr, std::list<SpriteShape*>&chainList);
	void SpriteRemoveAction(SpriteShape *spr);
	void markRemove(SpriteShape* spr);
	void removeSprite();
	void FillSprite();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* evt);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* evt);
	SpriteShape *spriteOfPoint(Vec2 *position);
	void swapSprite();
	void MyClock(float dr);
	void gameOverCallback(Node *node);
	CREATE_FUNC(GameScene);
private:
	SpriteShape * map[ROW][COL];
	bool isAction;//表示是否正在执行动作
	bool isFillSprite;//表示是否需要进行填充操作
	bool isTouchEnable;
	bool isGameOver;
	SpriteShape *startShape;
	SpriteShape* endShape;
	int m_score;
	int m_time;

};
