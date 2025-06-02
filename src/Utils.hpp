#pragma once

std::function<float(float, float)>getFunctionForEasing(int easing);
cocos2d::CCActionEase* getCCEase(cocos2d::CCActionInterval* action, int easing, float exponent);
std::string getEaseString(int easing);