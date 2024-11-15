#pragma once
#include <vector>

class DelayLine
{
public:
    // === Lifecycle ==========================================================
    DelayLine(size_t capacity);
    ~DelayLine();

    // === Operations =========================================================
    void addSample(float sample);
    float getDelayed(int delay);
    void clear();

private:
    std::vector<float> samples;
    int leastRecentSample;
};