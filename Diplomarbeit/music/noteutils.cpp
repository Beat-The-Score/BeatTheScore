#include "noteutils.h"
#include <math.h>

float getFrequencyOfPitch(float pitch)
{
    return powf(NOTE_SEMITONE_FACTOR, pitch) * NOTE_0_FREQUENCY;
}

float getPitchOfFrequency(float frequency)
{
    return logf(frequency / NOTE_0_FREQUENCY) / NOTE_LOG_SEMITONE_FACTOR;
}
