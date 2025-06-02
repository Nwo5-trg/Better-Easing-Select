#include "Utils.hpp"

using namespace geode::prelude;

const float pi = 3.14159;
const float cc1 = 1.70158;
const float cc2 = 2.59491;
const float cc3 = 2.70158;
const float cc4 = 2.09440;
const float cc5 = 1.39626;

// hopefully these are right half ts is just slight modified rips from random places (namely https://easings.net/#)
const std::unordered_map<int, std::function<float(float, float)>> easingFunctionMap {
    {0, [] (float x, float exponent) { // linear (none)
        return x;
    }},
    {1, [] (float x, float exponent) { // ease
        if (x < 0.5f) return 0.5f * powf(2 * x, exponent);
        else return 1 - (0.5f * powf(2 * (1 - x), exponent));
    }},
    {2, [] (float x, float exponent) {
        return powf(x, exponent);
    }},
    {3, [] (float x, float exponent) {
        return 1 - powf(1 - x, exponent);
    }},
    {4, [] (float x, float exponent) { // elastic
        if (x == 0 || x == 1) return x;
        exponent = 10.0f * exponent;
        if (x < 0.5f) return -0.5f * powf(2, exponent * 2 * x - exponent) * sinf(((exponent * 2 * x) - exponent + 0.25f) * cc5);
        else return 0.5f * powf(2, -exponent * 2 * (x - 0.5f)) * sinf(((exponent * 2 * (x - 0.5f)) - 0.75f) * cc5) + 1;
    }},
    {5, [] (float x, float exponent) {
        if (x == 0 || x == 1) return x;
        exponent = 10.0f * exponent;
        return -powf(2, exponent * x - exponent) * sinf(((x * exponent) - exponent + 0.25f) * cc4);
    }},
    {6, [] (float x, float exponent) {
        if (x == 0 || x == 1) return x;
        exponent = 10.0f * exponent;
        return powf(2, -exponent * x) * sinf(((x * exponent) - 0.75f) * cc4) + 1;
    }},
    {7, [] (float x, float exponent) { // bounce
        if(x < 0.5f) return 8 * powf(2, 8 * (x - 1)) * abs(sinf((x * pi) * 7));
        else return 1 - (8 * powf( 2, -8 * x) * abs(sinf((x * pi) * 7)));
    }},
    {8, [] (float x, float exponent) {
        return powf(2, 6 * (x - 1)) * abs(sinf((x * pi) * 3.5f));
    }},
    {9, [] (float x, float exponent) {
        return 1 - (powf(2, -6 * x) * abs(cosf((x * pi) * 3.5f)));
    }},
    {10, [] (float x, float exponent) { // exponential
        if (x == 0 || x == 1) return x;
        if (x < 0.5f) return powf(2, (20 * x) - 10) / 2;
        else return (2 - powf(2, (-20 * x) + 10)) / 2;
    }},
    {11, [] (float x, float exponent) {
        if (x == 0) return x; 
        else return powf(2, 10 * (x - 1));
    }},
    {12, [] (float x, float exponent) {
        if (x == 1) return x;
        else return 1 - powf(2, -10 * x);
    }},
    {13, [] (float x, float exponent) { // sine
        return -(cosf(pi * x) - 1) / 2;
    }},
    {14, [] (float x, float exponent) {
        return 1 - cosf((x * pi) / 2);
    }},
    {15, [] (float x, float exponent) {
        return sinf((x * pi) / 2);
    }},
    {16, [] (float x, float exponent) { // back
        if (x < 0.5f) return (powf(((2 * x)), 2)) * ((((cc2 + 1) * ((2 * x))) - cc2)) / 2;
        else return (((powf((((2 * x) - 2)), 2)) * (((cc2 + 1) * (((2 * x) - 2))) + cc2)) + 2) / 2;
    }},
    {17, [] (float x, float exponent) {
        return (cc3 * powf(x, 3)) - (cc1 * powf(x, 2));
    }},
    {18, [] (float x, float exponent) {
        return 1 + ((cc3 * powf(x - 1, 3)) + (cc1 * powf(x - 1, 2)));
    }},
};

const std::unordered_map<int, std::string> easingStringMap {
    {0, "None"}, {1, "Ease In Out"}, {2, "Ease In"}, {3, "Ease Out"},
    {4, "Elastic In Out"}, {5, "Elastic In"}, {6, "Elastic Out"},
    {7, "Bounce In Out"}, {8, "Bounce In"}, {9, "Bounce Out"},
    {10, "Exponential In Out"}, {11, "Exponential In"}, {12, "Exponential Out"},
    {13, "Sine In Out"}, {14, "Sine In"}, {15, "Sine Out"},
    {16, "Back In Out"}, {17, "Back In"}, {18, "Back Out"}
};


std::function<float(float, float)>getFunctionForEasing(int easing) {
    if (easing >= easingFunctionMap.size()) easing = easing % easingFunctionMap.size();
    return easingFunctionMap.at(easing);
}

cocos2d::CCActionEase* getCCEase(cocos2d::CCActionInterval* action, int easing, float exponent) {
    if (easing >= easingFunctionMap.size()) easing = easing % easingFunctionMap.size();
    switch (easing) {
        case 1: return CCEaseInOut::create(action, exponent); // ease in is broken so we just making some fake shit up so it looks good :3
        case 2: return CCEaseInOut::create(action, exponent * 1.5);
        case 3: return CCEaseOut::create(action, exponent);
        case 4: return CCEaseElasticInOut::create(action);
        case 5: return CCEaseElasticIn::create(action);
        case 6: return CCEaseElasticOut::create(action);
        case 7: return CCEaseBounceInOut::create(action);
        case 8: return CCEaseBounceIn::create(action);
        case 9: return CCEaseBounceOut::create(action);
        case 10: return CCEaseExponentialInOut::create(action);
        case 11: return CCEaseExponentialIn::create(action);
        case 12: return CCEaseExponentialOut::create(action);
        case 13: return CCEaseSineInOut::create(action);
        case 14: return CCEaseSineIn::create(action);
        case 15: return CCEaseSineOut::create(action);
        case 16: return CCEaseBackInOut::create(action);
        case 17: return CCEaseBackIn::create(action);
        case 18: return CCEaseBackOut::create(action);
    }
    return nullptr;
}

std::string getEaseString(int easing) {
    if (easing >= easingFunctionMap.size()) easing = easing % easingFunctionMap.size();
    return easingStringMap.at(easing);
};