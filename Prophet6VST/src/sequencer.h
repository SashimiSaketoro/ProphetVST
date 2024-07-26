#pragma once

#include <JuceHeader.h>
#include <vector>

class Sequencer
{
public:
    Sequencer();
    ~Sequencer();

    void init(double sampleRate);
    void process(float& left, float& right);

    struct Step
    {
        int note;
        float velocity;
        bool rest;
        bool tie;
    };

    void setSteps(const std::vector<Step>& steps);
    void setRate(float rate);
    void setGateTime(float gateTime);

    void start();
    void stop();
    void reset();

    bool isRunning() const { return running; }

private:
    double sampleRate;
    std::vector<Step> sequence;
    float rate;
    float gateTime;
    bool running;

    size_t currentStep;
    double stepCounter;
    double gateCounter;

    void advanceStep();
    void triggerNote(const Step& step);
    void releaseNote();
};