#ifndef NOTEUTILS_H
#define NOTEUTILS_H

#define NOTE_0_FREQUENCY            8.17579891564370733368  // 2^(-69/12)*440
#define NOTE_SEMITONE_FACTOR        1.05946309435929526456  // 2^(1/12)
#define NOTE_LOG_SEMITONE_FACTOR    0.05776226504666210912  // log_e(2^(1/12))

float getFrequencyOfPitch(float pitch);
float getPitchOfFrequency(float frequency);

#endif // NOTEUTILS_H
