#pragma once

class EasingSelectPopup : public geode::Popup<> {
private:
    cocos2d::CCArray* m_easingButtonArray;
    SetupTriggerPopup* m_triggerPopup;
    float m_exponent;
    int m_steps;
    int m_type;
    float m_thickness;
    cocos2d::ccColor4F m_col;

    void onTogglePreviewMode(cocos2d::CCObject* sender);

public:
    static EasingSelectPopup* create(SetupTriggerPopup* triggerPopup, float exponent);
    bool setup() override;

    void onClose(CCObject* sender) override;

};
