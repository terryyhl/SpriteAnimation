#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <android/log.h> 



using namespace cocos2d;
using namespace CocosDenshion;

#define  LOG_TAG    "main"                                                                                                              ||-   function
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)  

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	this->setTouchEnabled(true);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCAnimationCache::purgeSharedAnimationCache();
	CCAnimationCache *animCache = CCAnimationCache::sharedAnimationCache();

    animCache->addAnimationsWithFile("sg.plist");
	


	cache=CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("sg.plist");

	sprite=CCSprite::createWithSpriteFrameName("A1_6.png");
	sprite->setPosition(ccp(size.width-sprite->getContentSize().width,size.height/2));
	spriteBatchNode=CCSpriteBatchNode::create("sg.png");
	spriteBatchNode->addChild(sprite);
	 addChild(spriteBatchNode);

	cache->addSpriteFramesWithFile("hero.plist");
	hero=CCSprite::createWithSpriteFrameName("Hero02_0.png");
	hero->setPosition(ccp(hero->getContentSize().width,size.height/2));
	heroBatchNode=CCSpriteBatchNode::create("hero.png");
	heroBatchNode ->addChild(hero);
	hero->setFlipX(true);
	
    addChild(heroBatchNode);
	


	attackArray =CCArray::createWithCapacity(4);
	char attackName[20];
	for(int i=0;i<4;i++){
		sprintf(attackName,"A1_%d.png",i);
		CCSpriteFrame* frame =cache->spriteFrameByName(attackName);
		attackArray->addObject(frame);
	}
	CCAnimation *attackAnimation =CCAnimation::createWithSpriteFrames(attackArray,0.2f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(attackAnimation, "attack");
	
	attackArray->removeAllObjects();

	

	standArray= CCArray::createWithCapacity(1);
	char standName[20];
		for(int i=6;i<7;i++){
			sprintf(standName,"A1_%d.png",i);
			CCSpriteFrame* frame =cache->spriteFrameByName(standName);
			standArray->addObject(frame);
		}
		CCAnimation *standAnimation =CCAnimation::createWithSpriteFrames(standArray,0.2f);
		CCAnimationCache::sharedAnimationCache()->addAnimation(standAnimation, "stand");
		standArray->removeAllObjects();
	
	
    return true;
}

void HelloWorld::moveDone(){
	//
	sprite->stopAllActions();
	CCAnimationCache *animCache = CCAnimationCache::sharedAnimationCache();


	CCAnimation *standAnimation = animCache->animationByName("stand");
		standAnimation->setRestoreOriginalFrame(true);
		CCAnimate *standAni=CCAnimate::create(standAnimation);
		CCActionInterval* s=(CCActionInterval*)(CCSequence::create(standAni,
			standAni->copy()->autorelease(),
			NULL
		));
		CCAction *frameAction=CCRepeatForever::create(s);

	
	sprite->runAction(frameAction);
}

CCAction* HelloWorld::createAction(int begin,int end,char* cacheActionName,CCPoint point){
	CCAnimationCache *animCache = CCAnimationCache::sharedAnimationCache();
	CCArray *array = CCArray::createWithCapacity(end-begin);

	char name[20];
	for(int i = begin ;i<end;i++){
		sprintf(name,"A1_%d.png",i);
		CCSpriteFrame* frame =cache->spriteFrameByName(name);
		array->addObject(frame);
	}

	CCAnimation *plistAnimation = CCAnimation::createWithSpriteFrames(array,0.2f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(plistAnimation, cacheActionName);

	array->removeAllObjects();

	CCAnimation *animation = animCache->animationByName(cacheActionName);
	animation->setRestoreOriginalFrame(true);
	CCAnimate *ani=CCAnimate::create(animation);
	CCActionInterval* plistSeq=(CCActionInterval*)(CCSequence::create(ani,
		CCFlipX::create(point.x>0? true:false),
		ani->copy()->autorelease(),
		NULL
	));
	return CCRepeatForever::create(plistSeq);
}


void HelloWorld::registerWithTouchDispatcher(){                                                                                    
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
void HelloWorld::ccTouchesEnded(cocos2d::CCSet * touches,cocos2d::CCEvent * event){

	sprite->stopAllActions();
	
	
	CCTouch* touch=(CCTouch*)(touches->anyObject());  
	 CCPoint location = touch ->getLocation();
	 float offX=location.x-sprite->getPosition().x; 
	 float offY=location.y-sprite->getPosition().y; 
	 
	 walkAction=createAction(4,6,"move",ccp(offX,offY));
	 
	 sprite->setFlipX(offX>0?true:false);

	
	 float realX=offY/offX;
	  CCPoint realDeast =ccp(location.x,location.y);
	  CCActionInterval *actionTo=CCMoveTo::create(2.2f,realDeast);
	CCAction *moveToAction=CCSequence::create(
		actionTo,
		CCCallFunc::create(this,callfunc_selector(HelloWorld::moveDone)),
		NULL
	);

  sprite->runAction(walkAction);
  sprite->runAction(moveToAction);
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
