#pragma once

#include <JuceHeader.h>
#include <vector>

class Arpeggiator
{
public:
    Arpeggiator();
    ~Arpeggiator();

    void init(double sampleRate);
    void process(float& left, float& right);

    enum class Mode
    {
        Up,
        Down,
        UpDown,
        Random,
        AsPlayed
    };

    void setMode(Mode mode);
    void setRate(float rate);
    void setOctaveRange(int range);
    void setGateTime(float gateTime);

    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);

    void start();
    void stop();

private:
    double sampleRate;
    Mode currentMode;
    float rate;
    int octaveRange;
    float gateTime;
    bool isRunning;

    std::vector<int> heldNotes;
    std::vector<int> sequence;
    size_t currentStep;
    double stepCounter;

    void generateSequence();
    void advanceStep();
    int getCurrentNote();
};