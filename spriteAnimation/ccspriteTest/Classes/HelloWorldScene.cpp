#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <android/log.h>


using namespace cocos2d;
using namespace CocosDenshion;

#define LOG_TAG "sprite"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

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

//--------------sprite with png list------------
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCArray *aniframe=CCArray::createWithCapacity(4);
	CCSprite *sprite;
	char str[20];
	for(int i=0;i<4;i++){
		
		sprintf(str,"attack/A1_%d.png",i);
		CCSpriteFrame *frame =CCSpriteFrame::create(str,CCRectMake(0,0,64,64));
		if(i==0){
			sprite =CCSprite::createWithSpriteFrame(frame);
			sprite->setPosition(ccp(sprite->getContentSize().width,s.height/2));
			addChild(sprite);
		}
		aniframe->addObject(frame);
	}
	CCAnimation *animation=CCAnimation::createWithSpriteFrames(aniframe,0.2f);
		CCAnimate *animate=CCAnimate::create(animation);
		CCActionInterval* seq=(CCActionInterval*)(CCSequence::create(animate,
			CCFlipX::create(true),
			animate->copy()->autorelease(),
			CCFlipX::create(false),
			NULL
		));

		sprite->runAction(CCRepeatForever::create(seq));
    
//----------------end

//------------------sprite split-----------------//

	CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage("split.png");
    CCArray *splitAniframe=CCArray::createWithCapacity(16);
	CCSprite *splitSprite ;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			{
		 		CCSpriteFrame *frame =CCSpriteFrame::createWithTexture(texture,CCRectMake(32*j,48*i,32,48));
		 		if(i==0){
		 			splitSprite= CCSprite::createWithSpriteFrame(frame);
		 			splitSprite->setPosition(ccp(s.width/2,s.height/2));
		 			addChild(splitSprite);
		 		}
		 		splitAniframe->addObject(frame);
			}
	}
	CCAnimation *splitAnimation=CCAnimation::createWithSpriteFrames(splitAniframe,1.0f);
		CCAnimate *splitAnimate=CCAnimate::create(splitAnimation);
		splitSprite->runAction(CCRepeatForever::create(splitAnimate));
		//------------------end


//------------------sprite use plist--------------------

	CCSpriteFrameCache* cache =CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("attack.plist");
	CCSprite *plistSprite=CCSprite::createWithSpriteFrameName("A1_0.png");
	plistSprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width-plistSprite->getContentSize().width,CCDirector::sharedDirector()->getWinSize().height/2));
	CCSpriteBatchNode* spritebatch =CCSpriteBatchNode::create("attack.png");
	spritebatch->addChild(plistSprite);
	addChild(spritebatch);

	CCArray* plistArray=CCArray::createWithCapacity(4);
	char name[20];
	for(int i=0;i<4;i++){
		sprintf(name,"A1_%d.png",i);
		CCSpriteFrame* frame =cache->spriteFrameByName(name);
		plistArray->addObject(frame);
	}
	
	CCAnimation *plitAnimation=CCAnimation::createWithSpriteFrames(plistArray,0.2f);
		CCAnimate *plitAnimate=CCAnimate::create(plitAnimation);
		CCActionInterval* plitSeq=(CCActionInterval*)(CCSequence::create(plitAnimate,
			CCFlipX::create(true),
			plitAnimate->copy()->autorelease(),
			CCFlipX::create(false),
			NULL
		));

		plistSprite->runAction(CCRepeatForever::create(plitSeq));
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
