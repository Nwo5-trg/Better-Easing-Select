#include <Geode/Geode.hpp>
#include <Geode/modify/SetupTriggerPopup.hpp>
#include "EasingSelectPopup.hpp"

using namespace geode::prelude;


class $modify(TriggerPopup, SetupTriggerPopup) { // ill add area trigger support later i cant be fucked rn
    void createEasingControls(CCPoint p0, float p1, int p2, int p3) {
        SetupTriggerPopup::createEasingControls(p0, p1, p2, p3);
        if (m_easingRateButton) {
            auto easeSelectButton = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_plus2Btn_001.png"), this, menu_selector(TriggerPopup::onEasingPopup));
            easeSelectButton->setPosition(m_easingRateButton->getPositionX() - 93, m_easingRateButton->getPositionY());
            easeSelectButton->m_baseScale = 0.65f;
            easeSelectButton->setScale(0.65f);
            m_buttonMenu->addChild(easeSelectButton);
        }
    }

    void onEasingPopup(CCObject* sender) {
        EasingSelectPopup::create(this, m_easingRate)->show();
    }
};
