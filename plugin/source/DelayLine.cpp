#include "DelayLine.h"
#include <algorithm>

DelayLine::DelayLine(size_t capacity)
{
    samples = std::vector<float>(capacity);
    leastRecentSample = 0;
    clear();
}

DelayLine::~DelayLine() {}

void DelayLine::addSample(float sample)
{
    samples[leastRecentSample++] = sample;
    leastRecentSample %= samples.size();
}

float DelayLine::getDelayed(int delay)
{
    if (delay > samples.size())
        return 0;
    int index = leastRecentSample - 1 - delay;
    if (index < 0)
        index += samples.size();
    return samples[index];
}

void DelayLine::clear()
{
    std::fill(samples.begin(), samples.end(), 0);
}

