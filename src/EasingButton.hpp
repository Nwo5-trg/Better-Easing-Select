#pragma once

class EasingButton : public CCMenuItemSpriteExtra {
    private:
        int m_type;
        int m_defaultPreviewType;
        bool m_closeOnSelect;
        std::function<float(float, float)> m_easingFunction;
        SetupTriggerPopup*  m_triggerPopup;
        cocos2d::CCDrawNode* m_drawNode;
        cocos2d::CCSprite* m_previewSprite;

        void onSelectEase(CCObject* sender);
        
    public:
        static EasingButton* create(int type, SetupTriggerPopup* triggerPopup, int steps, int previewType, float exponent, float thickness, cocos2d::ccColor4F col);
        bool init(int type, SetupTriggerPopup* triggerPopup, int steps, int previewType, float exponent, float thickness, cocos2d::ccColor4F col);

        void update(int steps, int previewType, float exponent, float thickness, cocos2d::ccColor4F col);
        
};