#ifndef MAINGAME_H
#define MAINGAME_H

#include <vector>
#include <mutex>
#include <QString>
#include <QPointer>
#include <QDebug>
#include <QStack>
#include <QTimer>

class MainGame;
class NoteOutput;
class MidiNoteOutput;

#include "../music/score.h"
#include "../music/note.h"
#include "gameclock.h"
#include "evaluator.h"
#include "../sound/noteoutput.h"
#include "../sound/midinoteoutput.h"
#include "../input/midiinput.h"
#include "../input/midiinputmanager.h"
#include "../input/input.h"
#include "../input-android/androidglue.h"
#include "maingamestate.h"
#include "../menu/soundnavigationhandler.h"
#include "../licensing/licensemanager.h"
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroidExtras>
#endif

Q_DECLARE_METATYPE(MainGame*)

class MainGame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool pauseMenuVisible READ pauseMenuVisible NOTIFY pauseMenuToggled)
    Q_PROPERTY(bool resultScreenVisible READ resultScreenVisible NOTIFY resultScreenToggled)
    Q_PROPERTY(qreal loadingProgress READ loadingProgress NOTIFY loadingProgressChanged)
    Q_PROPERTY(QStringList availableInputs READ getAvailableInputs NOTIFY availableInputsChanged)
    Q_PROPERTY(bool disconnectHandler READ getDisconnect WRITE setDisconnect NOTIFY disconnectHandlerChanged)

public:
    MainGame();
    //~MainGame();

private:
    MidiInputManager midiInputManager;
    QList<QPointer<Input>> inputs;
    QList<QPointer<Score>> availableScores;
    QPointer<Score> score;
    QPointer<Track> playerTrack; // what the player played
    QPointer<Input> playerInput; // the players hardware
    QPointer<Evaluator> evaluator; // the players judge
    GameClock gameClock;
    int midiInputCount;
    int selectedInput;
    int selectedTrack;
    int selectedSection;
    QPointer<NoteOutput> noteOutput;
    MidiNoteOutput *midiOutput = NULL;
    QPointer<SoundNavigationHandler> uiCommandHandler;
    QString selectedFilename;
    State state = MAIN_MENU;
    State oldState = MAIN_MENU;

    std::mutex inputRefreshLock;
    bool showAudioInputs = true;

    LicenseState licenseState = UNLICENSED;
    QPointer<LicenseManager> licenseManager;

    std::mutex stateMutex;
    GameMode mode = NORMAL;
    bool showPauseMenu = false;
    bool showResultScreen = false;
    QThread *checker = NULL;
    bool checkerRunning = true;

    void connectPlayer(Input *input, int trackIndex);
    void connectToUI(Input *input);
    void disconnectFromUI(Input *input);
    bool isInputInList(QPointer<Input> input);
    LicenseState getLicenseState();

    int awaitedNoteTime;
    int playedNoteTime;

    int scoreInReplay = 0;

    bool getDisconnect();
    void setDisconnect(bool disconnect);
    bool deviceDisconnected = false;

    qreal loadingProgress();
    qreal progress = 0.0;
    bool isPitchingDone = false;


public: // functions accesable from QML
    // state navigation
    Q_INVOKABLE void mainMenu();
    Q_INVOKABLE void songSelection();
    Q_INVOKABLE void startSong();
    Q_INVOKABLE void startAgain();
    Q_INVOKABLE void startReplay();

    // game mode selection
    Q_INVOKABLE void setGameMode(int mode);

    // song selection
    Q_INVOKABLE QString selectSong(QString filename);
    Q_INVOKABLE void selectInput(int index);
    Q_INVOKABLE void selectTrack(int index);
    Q_INVOKABLE void selectSection(int index);
    Q_INVOKABLE void setTempo(float factor);

    Q_INVOKABLE QString getSelectedScore();
    Q_INVOKABLE int getSelectedTrackIndex();
    Q_INVOKABLE int getSelectedSectionIndex();

    Q_INVOKABLE QString getScoresPath();
    Q_INVOKABLE QStringList getAvailableTracks();
    Q_INVOKABLE QStringList getAvailableInputs();
    Q_INVOKABLE QStringList getAvailableModes();
    Q_INVOKABLE QList<QObject*> getAvailableSections();
    Q_INVOKABLE QList<QObject*> getAvailableScores();
    Q_INVOKABLE QList<QObject*> getAvailableScoresFiltered(QString filterText);
    Q_INVOKABLE int getLengthOfSelectedSong();

    // ingame
    Q_INVOKABLE int getGameTime();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void clearAudioBuffer();
    Q_INVOKABLE void togglePause();

    Q_INVOKABLE int getScorePercentage();
    Q_INVOKABLE void increaseVolume();
    Q_INVOKABLE void decreaseVolume();
    Q_INVOKABLE void setVolume(int volume);

    Q_INVOKABLE bool isRunning();
    Q_INVOKABLE bool isDesktop();

    Q_INVOKABLE bool isReplayMode();
    Q_INVOKABLE bool isResultScreen();

    // related to options
    Q_INVOKABLE void toggleAudioInputs(bool start);
    Q_INVOKABLE void setLatency(int value);

    // other
    Q_INVOKABLE void changeFullScreen();

    const bool pauseMenuVisible();
    const bool resultScreenVisible();

    float tempoFactor = 1;

signals:
    void stateChanged(State);
    void pauseMenuToggled();
    void resultScreenToggled();
    void loadingProgressChanged();
    void availableInputsChanged();
    void disconnectHandlerChanged();

public slots:
    void cleanup();

private slots:
    void pauseMenuHandler(State state);
    void resultScreenHandler(State state);
    void progressHandler();
    void deviceDisconnectedHandler(); // Currently Android only

    void licenseValidityChanged();

public: // C++-only functions
    void start();
    void setGameTime(int time);
    QPointer<Input> getCurrentInput();
    void pauseGameClock();
    void resumeGameClock();
    QPointer<Score> getScore();
    QPointer<Track> getTargetTrack();
    QPointer<Track> getPlayerTrack();
    void setState(State state);
    State getState();
    GameMode getGameMode();
    int getStartIndex(int trackId);

    int timingOnThreshold = 15;
    int timingOffThreshold = 200;

    #ifdef Q_OS_ANDROID
    void setAndroidGlue(AndroidGlue *glue);
    AndroidGlue *androidGlue;
    #endif

    void awaitNote(int time);
    void nextNote(int time);

    void setLicenseManager(QPointer<LicenseManager> licenseManager);

    QPointer<SoundNavigationHandler> getUiCommandHandler() const;
    void setUiCommandHandler(const QPointer<SoundNavigationHandler> &value);


    QTimer *refreshTimer;

private slots:
    void refreshInputList();
};

#endif // MAINGAME_H
