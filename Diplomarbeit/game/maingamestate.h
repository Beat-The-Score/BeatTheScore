#ifndef MAINGAMESTATE_H
#define MAINGAMESTATE_H

enum State {
    LICENSE_MENU,
    MAIN_MENU,
    SONG_SELECTION,
    PLAYING,
    WAITING, // waiting for the user to input the next note
    PAUSED,
    RESULT,
    SCROLLING,
    GAME_STARTED
};

enum GameMode {
    NORMAL = 0,
    TRAINING = 1,
    REPLAY = 2
};

enum SelectedView {
    PIANOROLL = 0,
    SCORE = 1,
    TABULATURE = 2
};

enum LicenseState {
    LICENSED,
    UNLICENSED
};

#endif // MAINGAMESTATE_H
