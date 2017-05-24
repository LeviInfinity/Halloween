#include"cocos2d.h"
#include"GameScene.h"
#include"WelcomeScene.h"
#include"SpriteShape.h"
#include"Define.h"
#include"SetScene.h"
#include "SimpleAudioEngine.h"//声音管理类
#pragma execution_character_set("utf-8")
using namespace CocosDenshion;//声音类处于该命名空间中
USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/sucai.plist");
	//设置背景图片
	auto pDirector = Director::getInstance();
	auto pSprite = Sprite::create("res/GameBackground.png");
	pSprite->setPosition(Vec2(Screen_Width / 2, Screen_Height / 2));	//通过宏定义，保证图片在屏幕中间
	this->addChild(pSprite);
	//设置返回按钮
	auto setItem = MenuItemImage::create("res/back1.png", "res/back2.png", CC_CALLBACK_1(GameScene::onBackItemCallback, this));
	setItem->setPosition(Vec2(Screen_Width / 2 , Screen_Height / 8-30));
	auto menu = Menu::create(setItem, NULL);//菜单项放到菜单里
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//监听移动触摸事件
	this->scheduleUpdate();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), this);

	isAction = false;
	isFillSprite = false;
	isTouchEnable = true;
	isGameOver = false;
	m_score = 0;
	m_time = 60;
	initMap();//初始化地图
	//创建分数字体
	auto ptextScore = ui::Text::create(StringUtils::format("Score:  %d",0), "res/marry.ttf", 25);
	ptextScore->setTag(10);//因为下面需要在update中访问，所以通过tag访问
	ptextScore->setPosition(Vec2(Screen_Width/4,Screen_Height-20));
	this->addChild(ptextScore);

	auto ptextTime = ui::Text::create(StringUtils::format("Time: %d", 0), "res/marry.ttf", 25);
	ptextTime->setTag(11);//因为下面需要在update中访问，所以通过tag访问
	ptextTime->setPosition(Vec2(Screen_Width / 4*3, Screen_Height - 20));
	this->addChild(ptextTime);

	if (SetScene::isMusicPlay)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("res/GameBackGround.wav", true);

	auto pParticle = ParticleSnow::create();
	this->addChild(pParticle);

	schedule(schedule_selector(GameScene::MyClock),1.0f);//添加控制时间倒计时的定时器
	return true;
}
bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* evt)
{
	startShape = NULL;
	endShape = NULL;
	if (isTouchEnable)
	{
		auto pos = touch->getLocation();
		if (pos.x > 327 || pos.x < 39 || pos.y>519 || pos.y < 69)//如果点击在外面，则不做任何操作
			return false;
		startShape = spriteOfPoint(&pos);
		if (startShape->getImgIndex() == 8)//如果是那只猪，就消除整行
		{
			Vec2 mapPos = PositionOfItem(startShape->getRow(), startShape->getCol());//得到它在地图中的位置
			auto pParticle = ParticleSystemQuad::create("res/leftrow.plist");
			pParticle->setPosition(mapPos);
			this->addChild(pParticle);
			pParticle = ParticleSystemQuad::create("res/rightrow.plist");
			pParticle->setPosition(mapPos);
			this->addChild(pParticle);

			int row = startShape->getRow();
			for (int c = 0; c < COL; c++)
				markRemove(map[row][c]);
			removeSprite();
		}
	}
	return isTouchEnable;
}
//实时判定移动的位置是否可以交换位置
void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* evt)
{
	if (!startShape || !isTouchEnable)
		return;
	int row = startShape->getRow();
	int col = startShape->getCol();
	auto pos = touch->getLocation();
	auto halfSpriteWideth = SpriteWideth / 2;
	auto halfSpriteHeight = SpriteWideth / 2;
	//判定是否向上移动到了上方的那个糖果中
	auto upRect = Rect(startShape->getPositionX() - halfSpriteWideth,
		startShape->getPositionY() + halfSpriteHeight + BorderWideth,
		SpriteWideth, SpriteWideth);
	if (upRect.containsPoint(pos))
	{
		++row;
		if (row < ROW)
		{
			endShape = map[row][col];
		}
		swapSprite();
		return;
	}
	//判定是否向xia移动到了下方的那个糖果中
	auto downRect = Rect(startShape->getPositionX() - halfSpriteWideth, 
		startShape->getPositionY() - halfSpriteHeight*3 - BorderWideth,
		SpriteWideth, SpriteWideth);
	if (downRect.containsPoint(pos))
	{
		--row;
		if (row >=0)
		{
			endShape = map[row][col];
		}
		swapSprite();
		return;
	}
	//判定是否向zuo移动到了上方的那个糖果中
	auto leftRect = Rect(startShape->getPositionX() - halfSpriteWideth*3-BorderWideth,
		startShape->getPositionY() - halfSpriteHeight,
		SpriteWideth, SpriteWideth);
	if (leftRect.containsPoint(pos))
	{
		col--;
		if (col >= 0)
		{
			endShape = map[row][col];
		}
		swapSprite();
		return;
	}
	//判定是否向右移动到了右方的那个糖果中
	auto rightRect = Rect(startShape->getPositionX() + halfSpriteWideth+BorderWideth,
		startShape->getPositionY() - halfSpriteHeight,
		SpriteWideth, SpriteWideth);
	if (rightRect.containsPoint(pos))
	{
		++col;
		if (col < COL)
		{
			endShape = map[row][col];
		}
		swapSprite();
		return;
	}
}
void GameScene::swapSprite()
{
//	isAction = true;
	isFillSprite = false;
	isTouchEnable = false;
	if (!startShape || !endShape)
		return;
	Vec2 posOfScr = startShape->getPosition();
	Vec2 posOfDes = endShape->getPosition();
	float time = 0.2;

	//交换开始位置跟结束位置
	SpriteShape * temp=new SpriteShape();
	/*map[startShape->getRow()][startShape->getCol()] = endShape;
	map[endShape->getRow()][endShape->getCol()] = startShape;*/
	temp = map[startShape->getRow()][startShape->getCol()];
	map[startShape->getRow()][startShape->getCol()] = map[endShape->getRow()][endShape->getCol()];
	map[endShape->getRow()][endShape->getCol()] = temp;

	int tempRow = startShape->getRow();
	int tempCol = startShape->getCol();
	startShape->setRow(endShape->getRow());
	startShape->setCol(endShape->getCol());
	endShape->setRow(tempRow);
	endShape->setCol(tempCol);

	//判断移动后的开始和结束两个精灵是否可以有消除的
	std::list<SpriteShape *>colChainListOfFirst;
	getColChain(startShape, colChainListOfFirst);
	std::list<SpriteShape *>rowChainListOfFirst;
	getRowChain(startShape, rowChainListOfFirst);
	std::list<SpriteShape *>colChainListOfSecond;
	getColChain(endShape, colChainListOfSecond);
	std::list<SpriteShape *>rowChainListOfSecond;
	getRowChain(endShape, rowChainListOfSecond);

	if (colChainListOfFirst.size() >= 3
		|| rowChainListOfFirst.size() >= 3
		|| colChainListOfSecond.size() >= 3
		|| rowChainListOfSecond.size() >= 3
		)
	{
		//可以消除,进行移动
		startShape->runAction(MoveTo::create(time, posOfDes));
		endShape->runAction(MoveTo::create(time, posOfScr));
		return;
	}
	//不可移除的话就还移回来
	else {
		map[startShape->getRow()][startShape->getCol()] = endShape;
		map[endShape->getRow()][endShape->getCol()] = startShape;

		tempRow = startShape->getRow();
		tempCol = startShape->getCol();
		startShape->setRow(endShape->getRow());
		startShape->setCol(endShape->getCol());
		endShape->setRow(tempRow);
		endShape->setCol(tempCol);
		//startShape->runAction(Sequence::create(MoveTo::create(time, posOfDes), MoveTo::create(time, posOfScr),nullptr));
		//endShape->runAction(Sequence::create(MoveTo::create(time, posOfScr), MoveTo::create(time, posOfDes), nullptr));
		return;
	}
}
SpriteShape *GameScene::spriteOfPoint(Vec2 *position)//获得当前位置的糖果
{
	SpriteShape *spr = NULL;
	Rect rect = Rect(0, 0, 0, 0);
	Size sz;
	sz.height = SpriteWideth;
	sz.width = SpriteWideth;
	for (int r = 0; r < ROW; r++)
		for (int c = 0; c < COL; c++)
		{
			spr = map[r][c];
			if (spr)
			{
				rect.origin.x = spr->getPositionX() - (SpriteWideth / 2);
				rect.origin.y = spr->getPositionY() - (SpriteWideth / 2);
				rect.size = sz;
				if (rect.containsPoint(*position))
				{
					return spr;
				}
			}
		}
}
void GameScene::initMap()
{
	for (int r = 0; r < ROW; r++)
		for (int c = 0; c < COL; c++)
			createSprite(r, c);
}
void GameScene::createSprite(int r, int c)
{
	isAction = true;
	isTouchEnable = false;
	SpriteShape * spr = SpriteShape::create(r, c);
	Vec2 endPosition = PositionOfItem(r, c);
	Vec2 startPosition = Vec2(endPosition.x, endPosition.y + Screen_Height / 2);
	spr->setPosition(startPosition);
	float speed = startPosition.y / (1.2*Screen_Height);
	spr->runAction(MoveTo::create(speed, endPosition));//通过一个动画进行下落动作
	this->addChild(spr);
	map[r][c] = spr;
}
//每帧调用是否有可以消除的
void GameScene::update(float dr)
{
	if (isAction)
	{
		isAction = false;
		for (int r = 0; r < ROW; r++)
			for (int c = 0; c < COL; c++)
			{
				SpriteShape *spr = map[r][c];
				//int index = spr->getImgIndex();
			//	if (spr->getImgIndex()==8)
			//	{
				//	auto pParticle = ParticleSystemQuad::create("res/fire.plist");
				//	pParticle->setPosition(PositionOfItem(r, c));
				//	addChild(pParticle);
				//}
				if (spr&&spr->getNumberOfRunningActions()>0)//如果正在执行动作，就不执行消除的判定
				{
					isAction = true;
					isTouchEnable = false;
				}

			}
	}
	if (!isAction)
	{
		isTouchEnable = true;
		if (isFillSprite)
		{
			if(!isGameOver)
				FillSprite();
			isFillSprite = false;
		}
		else
			checkAndRemoveSprite();
	}
	auto pscore = (ui::Text*)this->getChildByTag(10);
	pscore->setString(StringUtils::format("Score:  %d", m_score));
}
void GameScene::MyClock(float dr)
{
	m_time--;
	if (m_time >=0)
	{
		auto ptime = (ui::Text*)this->getChildByTag(11);
		ptime->setString(StringUtils::format("Time:  %d", m_time));
	}
	if (m_time == 10)
		SimpleAudioEngine::getInstance()->playEffect("res/daojishi.wav");
	if (m_time == 0)
	{
		//游戏结束
		SimpleAudioEngine::getInstance()->pauseAllEffects();
		SimpleAudioEngine::getInstance()->playEffect("res/timeover.wav");
		auto overSprite = Sprite::create("res/GameOver.png");
		overSprite->setPosition(Vec2(Screen_Width / 2 + 10, Screen_Height - 20));
		overSprite->runAction(Sequence::create(MoveTo::create(3.0f, Vec2(Screen_Width / 2 + 10, Screen_Height / 2)),
			CallFuncN::create(CC_CALLBACK_1(GameScene::gameOverCallback, this)), nullptr));
		this->addChild(overSprite, 1);
		isTouchEnable = false;

		for (int r = 0; r < ROW; r++)
			for (int c = 0; c < COL; c++)
			{
				markRemove(map[r][c]);
			}
		removeSprite();
		isGameOver = true;
	
	}

}
void GameScene::gameOverCallback(Node *node)
{
	auto pDirector = Director::getInstance();
	auto scene = WelcomeScene::createScene();
	TransitionSplitCols * ptrans = TransitionSplitCols::create(1.0f, scene);//设置切换场景的方式,书本 P25
	pDirector->replaceScene(ptrans);	//切换场景
}
void GameScene::FillSprite()
{
	int colRemoveNumber[10] = { 0 };
	SpriteShape *spr = NULL;
	for (int c = 0; c < COL; c++)
	{
		int removeSpriteOfCol = 0;
		for (int r = 0; r < ROW; r++)//从下往上遍历
		{
			spr = map[r][c];
			if (spr == NULL)
				removeSpriteOfCol++;
			else
			{
				if (removeSpriteOfCol > 0)
				{
					//从上往下落
					int newRow = r - removeSpriteOfCol;//比如有两个空的，原本第5行的就要到3行去
					map[newRow][c] = spr;
					map[r][c] = NULL;
					Vec2 startPostion = spr->getPosition();
					Vec2 endPosition = PositionOfItem(newRow, c);
					float speed = (startPostion.y - endPosition.y) / Screen_Height * 3;
					spr->stopAllActions();
					spr->runAction(MoveTo::create(speed, endPosition));
					spr->setRow(newRow);
				}

			}
		}
		colRemoveNumber[c] = removeSpriteOfCol;//记录下每一列的空行数
	}
	for (int c = 0; c < COL; c++)	//填充顶部的空的部分
	{
		for (int r = ROW - colRemoveNumber[c]; r < ROW; r++)
		{
			createSprite(r, c);
		}
	}
}
void GameScene::checkAndRemoveSprite()//判定是否有可以移除的糖果
{
	SpriteShape *spr = new SpriteShape();
	for (int r = 0; r < ROW; r++)
		for (int c = 0; c < COL; c++)
		{
			spr = map[r][c];
			if (!spr)
				continue;
			//如果糖果已经被标记为需要被移除，则不需要进行判定
			if (spr->getIsNeedRemove())
				continue;
			//先遍历上下相邻的糖果，把相同的糖果放到一个list中
			std::list<SpriteShape*>colChainList;
			getColChain(spr, colChainList);

			if (colChainList.size() >= 3)//如果相同的大于等于3
			{
				std::list<SpriteShape*>::iterator it;
				for (it = colChainList.begin(); it != colChainList.end(); it++)
				{
					spr = (SpriteShape*)*it;
					if (!spr)
						continue;
					markRemove(spr);
				}
			}
		}

	for (int c = 0; c < COL; c++)
		for (int r = 0; r < ROW; r++)
		{
			spr = map[r][c];
			if (!spr)
				continue;
			if (spr->getIsNeedRemove())
				continue;
			std::list<SpriteShape*>RowChainList;
			getRowChain(spr, RowChainList);
			//取更长的那个
		//	std::list<SpriteShape*>&longerList = colChainList.size()>RowChainList.size() ? colChainList : RowChainList;
			//if (longerList.size() < 3)
			//	continue;

			if (RowChainList.size() >= 3)//如果相同的大于等于3
			{
				std::list<SpriteShape*>::iterator rowit;
				for (rowit = RowChainList.begin(); rowit != RowChainList.end(); rowit++)
				{
					spr = (SpriteShape*)*rowit;
					if (!spr)
						continue;
					markRemove(spr);
				}
			}
		}
	removeSprite();
}
void GameScene::getColChain(SpriteShape * spr, std::list<SpriteShape*>&chainList)//判断列
{
	chainList.push_back(spr);
	//向下查找
	int neiborRow = spr->getRow() - 1;
	while (neiborRow >= 0)
	{
		SpriteShape * neiborSprite = map[neiborRow][spr->getCol()];
		if (neiborSprite&&neiborSprite->getImgIndex() == spr->getImgIndex() && !neiborSprite->getIsNeedRemove())
		{
			chainList.push_back(neiborSprite);
			neiborRow--;
		}
		else
			break;
	}
	//向上
	neiborRow = spr->getRow() + 1;
	while (neiborRow <ROW)
	{
		SpriteShape * neiborSprite = map[neiborRow][spr->getCol()];
		if (neiborSprite&&neiborSprite->getImgIndex() == spr->getImgIndex() && !neiborSprite->getIsNeedRemove())
		{
			chainList.push_back(neiborSprite);
			neiborRow++;
		}
		else
			break;
	}

}
void GameScene::getRowChain(SpriteShape * spr, std::list<SpriteShape*>&chainList)//判断行
{
	chainList.push_back(spr);
	//向左
	int neiborCol = spr->getCol() - 1;
	while (neiborCol >= 0)
	{
		SpriteShape * neiborSprite = map[spr->getRow()][neiborCol];
		if (neiborSprite&&neiborSprite->getImgIndex() == spr->getImgIndex() && !neiborSprite->getIsNeedRemove())
		{
			chainList.push_back(neiborSprite);
			neiborCol--;
		}
		else
			break;
	}
	//向右
	neiborCol = spr->getCol() + 1;
	while (neiborCol < COL)
	{
		SpriteShape * neiborSprite = map[spr->getRow()][neiborCol];
		if (neiborSprite&&neiborSprite->getImgIndex() == spr->getImgIndex() && !neiborSprite->getIsNeedRemove())
		{
			chainList.push_back(neiborSprite);
			neiborCol++;
		}
		else
			break;
	}
}

void GameScene::markRemove(SpriteShape* spr)
{
	if (spr->getIsNeedRemove())
		return;
	spr->setIsNeedRemove(true);
}

void GameScene::removeSprite()
{
	SpriteShape *spr = new SpriteShape();
	for (int r = 0; r < ROW; r++)
		for (int c = 0; c < COL; c++)
		{
			spr = map[r][c];
			if (!spr)
				continue;
			if (spr->getIsNeedRemove())
			{
				SpriteRemoveAction(spr);
				map[r][c] = NULL;
			}
		}

}
void GameScene::SpriteRemoveAction(SpriteShape *spr)//精灵消失的动画
{
	if(SetScene::isEffectPlay)
		SimpleAudioEngine::getInstance()->playEffect("res/xiaochu.mp3");
	m_score += 30;
	isAction = true;
	isTouchEnable = false;
	auto pFadeIn = FadeOut::create(0.6);
	auto pRotateBy = RotateBy::create(0.6f, 180);//顺时针旋转30°，同样也有RotateTo版本
	auto pScaleBy = ScaleBy::create(0.6f, 0.5f);
	spr->runAction(Spawn::create(pFadeIn, pRotateBy, pScaleBy, nullptr));
	if (!isAction)
		spr->removeFromParent();
	isFillSprite = true;
}


Vec2 GameScene::PositionOfItem(int r,int c)	//一个小的精灵在地图中显示的位置
{
	float x = (c + 1) * SpriteWideth + BorderWideth+10;
	float y = (r + 1) * SpriteWideth + BorderWideth+40;
	return  Vec2(x, y);
}
//返回到主界面按钮的回调函数

void GameScene::onBackItemCallback(Ref * pSender)
{
	auto pDirector = Director::getInstance();
	auto scene = WelcomeScene::createScene();
	TransitionSplitCols * ptrans = TransitionSplitCols::create(1.0f, scene);//设置切换场景的方式,书本 P25
	pDirector->replaceScene(ptrans);	//切换场景
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
