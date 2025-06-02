#include <Geode/Geode.hpp>
#include "EasingSelectPopup.hpp"
#include "EasingButton.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

bool EasingButton::init(int type, SetupTriggerPopup* triggerPopup, int steps, int previewType, float exponent, float thickness, ccColor4F col) {
    auto sprite = CCScale9Sprite::create("square02_001.png");
    sprite->setContentSize(CCSize(60.0f, 40.0f));
    sprite->setOpacity(100);
    if (!CCMenuItemSpriteExtra::init(sprite, nullptr, this, menu_selector(EasingButton::onSelectEase))) return false;
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    m_type = type;
    m_easingFunction = getFunctionForEasing(type);
    m_triggerPopup = triggerPopup;
    m_defaultPreviewType = previewType;
    m_closeOnSelect = Mod::get()->getSettingValue<bool>("close-on-select");

    auto drawNode = CCDrawNode::create();
    drawNode->setPosition(15.0f, 5.0f);
    drawNode->setContentSize(CCSize(60.0f, 40.0f));
    this->addChild(drawNode);
    m_drawNode = drawNode;

    auto previewSprite = CCSprite::createWithSpriteFrameName("square_01_001.png");
    previewSprite->setScale(0.5f);
    this->addChild(previewSprite);
    m_previewSprite = previewSprite;

    update(steps, previewType, exponent, thickness, col);
    return true;
}

void EasingButton::onSelectEase(CCObject* sender) {
    if (m_triggerPopup->m_easingLabel) {
        if (auto obj = m_triggerPopup->m_gameObject) {
            obj->m_easingType = static_cast<EasingType>(m_type);
        } else {
            for (auto obj : CCArrayExt<EffectGameObject*>(m_triggerPopup->m_gameObjects)) obj->m_easingType = static_cast<EasingType>(m_type);
        }
        m_triggerPopup->m_easingType = static_cast<EasingType>(m_type);
        m_triggerPopup->m_easingLabel->setString(getEaseString(m_type).c_str());
    }
    if (m_closeOnSelect) static_cast<EasingSelectPopup*>(this->getParent()->getParent()->getParent())->onClose(nullptr);
}

EasingButton* EasingButton::create(int type, SetupTriggerPopup* triggerPopup, int steps,  int previewType, float exponent, float thickness, ccColor4F col) {
    auto ret = new EasingButton();
    if (ret->init(type, triggerPopup, steps, previewType, exponent, thickness, col)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void EasingButton::update(int steps, int previewType, float exponent, float thickness, ccColor4F col) {
    auto drawNode = m_drawNode;
    m_drawNode->clear();
    auto sprite = m_previewSprite;
    sprite->setVisible(false);
    sprite->stopAllActions();
    if (previewType == 0) {
        auto easingFunction = m_easingFunction;
        auto lastPoint = ccp(0.0f, 0.0f);
        for (int i = 0; i < steps; i++) {
            auto x = (float)i / steps;
            auto y = easingFunction(x, exponent);
            auto point = ccp(x * 30.0f, y * 30.0f);
            drawNode->drawSegment(lastPoint, point, thickness, col);
            lastPoint = point;
        }
    } else {
        sprite->setVisible(true);

        CCActionInterval* toAction = nullptr;
        CCActionInterval* returnAction = nullptr;

        if (previewType == 1) {
            sprite->setPosition(ccp(10.0f, 20.0f));
            toAction = CCMoveBy::create(2.0f, ccp(40.0f, 0.0f));
            returnAction = CCMoveTo::create(0.0f, ccp(10.0f, 20.0f));
        } else if (previewType == 2) {
            sprite->setPosition(ccp(30.0f, 20.0f));
            toAction = CCScaleTo::create(2.0f, 1.0f);
            returnAction = CCScaleTo::create(0.0f, 0.5f);
        } else {
            sprite->setPosition(ccp(30.0f, 20.0f));
            toAction = CCRotateBy::create(2.0f, 360.0f);
            returnAction = CCRotateTo::create(0.0f, 0.0f);
        }
        
        if (m_type == 0) {
            sprite->runAction(CCRepeatForever::create(CCSequence::create(
            toAction, CCFadeOut::create(0.25f), returnAction, CCFadeIn::create(0.25f), nullptr)));
        } else {
            sprite->runAction(CCRepeatForever::create(CCSequence::create(getCCEase(toAction, m_type, exponent),
            CCFadeOut::create(0.25f), returnAction, CCFadeIn::create(0.25f), nullptr)));
        }
    }
}