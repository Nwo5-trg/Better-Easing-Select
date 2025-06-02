#include <Geode/Geode.hpp>
#include "EasingButton.hpp"
#include "EasingSelectPopup.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

EasingSelectPopup* EasingSelectPopup::create(SetupTriggerPopup* triggerPopup, float exponent) { 
    auto ret = new EasingSelectPopup();
    ret->m_exponent = exponent;
    ret->m_triggerPopup = triggerPopup;
    if (ret->initAnchored(400.0f, 290.0f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool EasingSelectPopup::setup() {
    this->setTitle("Setup Easing");

    auto easingButtonArray = CCArray::create();
    m_easingButtonArray = easingButtonArray;

    auto mod = Mod::get();

    m_steps = mod->getSettingValue<int64_t>("steps");
    m_thickness = mod->getSettingValue<double>("thickness");
    m_col = ccc4FFromccc3B(mod->getSettingValue<ccColor3B>("col"));

    int type = 0;
    GameObject* obj = nullptr;
    if (auto effectObj = m_triggerPopup->m_gameObject) obj = effectObj;
    else obj = static_cast<GameObject*>(m_triggerPopup->m_gameObjects->firstObject());
    if (obj) {
        auto id = obj->m_objectID;
        if (id == 901 /*|| id == 3006 || id == 3011 || id == 3017*/) type = 1;
        if (id == 2067 /*|| id == 3008 || id == 3013 || id == 3019*/) type = 2;
        if (id == 1346 /*|| id == 3007 || id == 3012 || id == 3018*/) type = 3;
    }
    m_type = type;

    for (int i = 0; i < 19; i++) {
        auto button = EasingButton::create(i, m_triggerPopup, m_steps, type, m_exponent, m_thickness, m_col);
        m_buttonMenu->addChild(button);
        easingButtonArray->addObject(button);
    }

    auto spacing = ccp(80.0f, 60.0f);
    auto start = ccp((m_size.width / 2) - ((spacing.x * (5 - 1)) / 2), m_size.height - 60);
    for (int i = 0; i < 20; i++) {
        if (i == 19) {
            if (type != 0) {
                auto previewModeToggle = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
                CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(EasingSelectPopup::onTogglePreviewMode));
                previewModeToggle->setPosition(start.x + ((i % 5) * spacing.x), start.y - ((i / 5) * spacing.y));
                m_buttonMenu->addChild(previewModeToggle);
            }
            continue;
        }
        static_cast<CCNode*>(easingButtonArray->objectAtIndex(i))->setPosition(
        start.x + ((i % 5) * spacing.x), start.y - ((i / 5) * spacing.y));
        auto label = CCLabelBMFont::create(getEaseString(i).c_str(), "bigFont.fnt");
        label->setPosition(start.x + ((i % 5) * spacing.x), (start.y - ((i / 5) * spacing.y)) - 30.0f);
        label->setScale(0.45f - 0.01786f * (std::string(label->getString()).length() - 4)); // ur telling me there was a fucking label lenght limit thing this whole tiem
        m_mainLayer->addChild(label);
    }

    return true;
}

void EasingSelectPopup::onTogglePreviewMode(CCObject* sender) {
    for (auto node : CCArrayExt<CCNode*>(m_buttonMenu->getChildren())) {
        if (auto easingButton = typeinfo_cast<EasingButton*>(node)) easingButton->update(
        m_steps, static_cast<CCMenuItemToggler*>(sender)->m_toggled ? m_type : 0, m_exponent, m_thickness, m_col);
    }
}

void EasingSelectPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
}