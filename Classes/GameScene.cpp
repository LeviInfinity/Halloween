#include"cocos2d.h"
#include"GameScene.h"
#include"WelcomeScene.h"
#include"SpriteShape.h"
#include"Define.h"
#include"SetScene.h"
#include "SimpleAudioEngine.h"//����������
#pragma execution_character_set("utf-8")
using namespace CocosDenshion;//�����ദ�ڸ������ռ���
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
	//���ñ���ͼƬ
	auto pDirector = Director::getInstance();
	auto pSprite = Sprite::create("res/GameBackground.png");
	pSprite->setPosition(Vec2(Screen_Width / 2, Screen_Height / 2));	//ͨ���궨�壬��֤ͼƬ����Ļ�м�
	this->addChild(pSprite);
	//���÷��ذ�ť
	auto setItem = MenuItemImage::create("res/back1.png", "res/back2.png", CC_CALLBACK_1(GameScene::onBackItemCallback, this));
	setItem->setPosition(Vec2(Screen_Width / 2 , Screen_Height / 8-30));
	auto menu = Menu::create(setItem, NULL);//�˵���ŵ��˵���
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//�����ƶ������¼�
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
	initMap();//��ʼ����ͼ
	//������������
	auto ptextScore = ui::Text::create(StringUtils::format("Score:  %d",0), "res/marry.ttf", 25);
	ptextScore->setTag(10);//��Ϊ������Ҫ��update�з��ʣ�����ͨ��tag����
	ptextScore->setPosition(Vec2(Screen_Width/4,Screen_Height-20));
	this->addChild(ptextScore);

	auto ptextTime = ui::Text::create(StringUtils::format("Time: %d", 0), "res/marry.ttf", 25);
	ptextTime->setTag(11);//��Ϊ������Ҫ��update�з��ʣ�����ͨ��tag����
	ptextTime->setPosition(Vec2(Screen_Width / 4*3, Screen_Height - 20));
	this->addChild(ptextTime);

	if (SetScene::isMusicPlay)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("res/GameBackGround.wav", true);

	auto pParticle = ParticleSnow::create();
	this->addChild(pParticle);

	schedule(schedule_selector(GameScene::MyClock),1.0f);//��ӿ���ʱ�䵹��ʱ�Ķ�ʱ��
	return true;
}
bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* evt)
{
	startShape = NULL;
	endShape = NULL;
	if (isTouchEnable)
	{
		auto pos = touch->getLocation();
		if (pos.x > 327 || pos.x < 39 || pos.y>519 || pos.y < 69)//�����������棬�����κβ���
			return false;
		startShape = spriteOfPoint(&pos);
		if (startShape->getImgIndex() == 8)//�������ֻ������������
		{
			Vec2 mapPos = PositionOfItem(startShape->getRow(), startShape->getCol());//�õ����ڵ�ͼ�е�λ��
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
//ʵʱ�ж��ƶ���λ���Ƿ���Խ���λ��
void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* evt)
{
	if (!startShape || !isTouchEnable)
		return;
	int row = startShape->getRow();
	int col = startShape->getCol();
	auto pos = touch->getLocation();
	auto halfSpriteWideth = SpriteWideth / 2;
	auto halfSpriteHeight = SpriteWideth / 2;
	//�ж��Ƿ������ƶ������Ϸ����Ǹ��ǹ���
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
	//�ж��Ƿ���xia�ƶ������·����Ǹ��ǹ���
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
	//�ж��Ƿ���zuo�ƶ������Ϸ����Ǹ��ǹ���
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
	//�ж��Ƿ������ƶ������ҷ����Ǹ��ǹ���
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

	//������ʼλ�ø�����λ��
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

	//�ж��ƶ���Ŀ�ʼ�ͽ������������Ƿ������������
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
		//��������,�����ƶ�
		startShape->runAction(MoveTo::create(time, posOfDes));
		endShape->runAction(MoveTo::create(time, posOfScr));
		return;
	}
	//�����Ƴ��Ļ��ͻ��ƻ���
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
SpriteShape *GameScene::spriteOfPoint(Vec2 *position)//��õ�ǰλ�õ��ǹ�
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
	spr->runAction(MoveTo::create(speed, endPosition));//ͨ��һ�������������䶯��
	this->addChild(spr);
	map[r][c] = spr;
}
//ÿ֡�����Ƿ��п���������
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
				if (spr&&spr->getNumberOfRunningActions()>0)//�������ִ�ж������Ͳ�ִ���������ж�
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
		//��Ϸ����
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
	TransitionSplitCols * ptrans = TransitionSplitCols::create(1.0f, scene);//�����л������ķ�ʽ,�鱾 P25
	pDirector->replaceScene(ptrans);	//�л�����
}
void GameScene::FillSprite()
{
	int colRemoveNumber[10] = { 0 };
	SpriteShape *spr = NULL;
	for (int c = 0; c < COL; c++)
	{
		int removeSpriteOfCol = 0;
		for (int r = 0; r < ROW; r++)//�������ϱ���
		{
			spr = map[r][c];
			if (spr == NULL)
				removeSpriteOfCol++;
			else
			{
				if (removeSpriteOfCol > 0)
				{
					//����������
					int newRow = r - removeSpriteOfCol;//�����������յģ�ԭ����5�еľ�Ҫ��3��ȥ
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
		colRemoveNumber[c] = removeSpriteOfCol;//��¼��ÿһ�еĿ�����
	}
	for (int c = 0; c < COL; c++)	//��䶥���ĿյĲ���
	{
		for (int r = ROW - colRemoveNumber[c]; r < ROW; r++)
		{
			createSprite(r, c);
		}
	}
}
void GameScene::checkAndRemoveSprite()//�ж��Ƿ��п����Ƴ����ǹ�
{
	SpriteShape *spr = new SpriteShape();
	for (int r = 0; r < ROW; r++)
		for (int c = 0; c < COL; c++)
		{
			spr = map[r][c];
			if (!spr)
				continue;
			//����ǹ��Ѿ������Ϊ��Ҫ���Ƴ�������Ҫ�����ж�
			if (spr->getIsNeedRemove())
				continue;
			//�ȱ����������ڵ��ǹ�������ͬ���ǹ��ŵ�һ��list��
			std::list<SpriteShape*>colChainList;
			getColChain(spr, colChainList);

			if (colChainList.size() >= 3)//�����ͬ�Ĵ��ڵ���3
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
			//ȡ�������Ǹ�
		//	std::list<SpriteShape*>&longerList = colChainList.size()>RowChainList.size() ? colChainList : RowChainList;
			//if (longerList.size() < 3)
			//	continue;

			if (RowChainList.size() >= 3)//�����ͬ�Ĵ��ڵ���3
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
void GameScene::getColChain(SpriteShape * spr, std::list<SpriteShape*>&chainList)//�ж���
{
	chainList.push_back(spr);
	//���²���
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
	//����
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
void GameScene::getRowChain(SpriteShape * spr, std::list<SpriteShape*>&chainList)//�ж���
{
	chainList.push_back(spr);
	//����
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
	//����
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
void GameScene::SpriteRemoveAction(SpriteShape *spr)//������ʧ�Ķ���
{
	if(SetScene::isEffectPlay)
		SimpleAudioEngine::getInstance()->playEffect("res/xiaochu.mp3");
	m_score += 30;
	isAction = true;
	isTouchEnable = false;
	auto pFadeIn = FadeOut::create(0.6);
	auto pRotateBy = RotateBy::create(0.6f, 180);//˳ʱ����ת30�㣬ͬ��Ҳ��RotateTo�汾
	auto pScaleBy = ScaleBy::create(0.6f, 0.5f);
	spr->runAction(Spawn::create(pFadeIn, pRotateBy, pScaleBy, nullptr));
	if (!isAction)
		spr->removeFromParent();
	isFillSprite = true;
}


Vec2 GameScene::PositionOfItem(int r,int c)	//һ��С�ľ����ڵ�ͼ����ʾ��λ��
{
	float x = (c + 1) * SpriteWideth + BorderWideth+10;
	float y = (r + 1) * SpriteWideth + BorderWideth+40;
	return  Vec2(x, y);
}
//���ص������水ť�Ļص�����

void GameScene::onBackItemCallback(Ref * pSender)
{
	auto pDirector = Director::getInstance();
	auto scene = WelcomeScene::createScene();
	TransitionSplitCols * ptrans = TransitionSplitCols::create(1.0f, scene);//�����л������ķ�ʽ,�鱾 P25
	pDirector->replaceScene(ptrans);	//�л�����
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
