//Silas Schaprian
//Matrikelnummer: 3520242

#ifndef MCT_MP3MAPPINGS_H
#define MCT_MP3MAPPINGS_H

#ifndef MCT_SAMPLE_RATE
#define MCT_SAMPLE_RATE 44100
#endif

#include <cstdint>
#include "../../mp3/achievement-win-drum.h"
#include "../../mp3/angelic-drum.h"
#include "../../mp3/bad-joke-drum.h"
#include "../../mp3/bass-futuristic.h"
#include "../../mp3/bass-mystirious.h"
#include "../../mp3/bass-pulsating.h"
#include "../../mp3/bass-rumble-hum.h"
#include "../../mp3/cinematic-synth-riser.h"
#include "../../mp3/deep-dark-horror-drum.h"
#include "../../mp3/electric-guitar.h"
#include "../../mp3/guitar-stroke-down-slow.h"
#include "../../mp3/guitar-stroke-up-slow.h"
#include "../../mp3/melodic-guitar.h"
#include "../../mp3/synth-intense-rhythm.h"
#include "../../mp3/synth-mechanical-notification.h"
#include "../../mp3/synth-pass.h"

/**
 * Enum to select Samples
 */
enum Song {
    ACHIEVEMENT_WIN_DRUM,
    ANGELIC_DRUM,
    BAD_JOKE_DRUM,
    BASS_FUTURISTIC,
    BASS_MYSTERIOUS,
    BASS_PULSATING,
    BASS_RUMBLE_HUM,
    CINEMATIC_SYNTH_RISER,
    DEEP_DARK_HORROR_DRUM,
    ELECTRIC_GUITAR,
    GUITAR_STROKE_DOWN_SLOW,
    GUITAR_STROKE_UP_SLOW,
    MELODIC_GUITAR,
    SYNTH_INTENSE_RHYTHM,
    SYNTH_MECHANICAL_NOTIFICATION,
    SYNTH_PASS,
};

// Array to get Labels for each Sample --> UI
const char* LABELS[] {"ACH_DRUM", "ANG_DRUM", "BAD_JOKE", "BASS_FUT",
                      "BASS_MYS", "BASS_PULS", "BASS_RUM", "CINC_SY_RIS",
                      "HORROR_DRUM","E_GUITAR",
                      "GUITAR_UP", "GUITAR_UP", "MELO_GUITAR",
                      "SYNTH_INT", "SYNTH_MECH", "SYNTH_PASS"};

/**
 * Class to save Samples and its length inside the Model-Class
 */
class Sample {
private:
    uint64_t length;
    const uint16_t *musicSample;
    Song selectedSong;

    /**
     * Initializes Class
     * @param song Song to be saved
     */
    void init(Song song) {
        switch (song) {
            case ACHIEVEMENT_WIN_DRUM:
                length = sizeof(achievement_win_drum) / sizeof(uint16_t);
                musicSample = achievement_win_drum;
                break;
            case ANGELIC_DRUM:
                length = sizeof(angelic_drum) / sizeof(uint16_t);
                musicSample = angelic_drum;
                break;
            case BAD_JOKE_DRUM:
                length = sizeof(bad_joke_drum) / sizeof(uint16_t);
                musicSample = bad_joke_drum;
                break;
            case BASS_FUTURISTIC:
                length = sizeof(bass_futuristic) / sizeof(uint16_t);
                musicSample = bass_futuristic;
                break;
            case BASS_MYSTERIOUS:
                length = sizeof(bass_mystirious) / sizeof(uint16_t);
                musicSample = bass_mystirious;
                break;
            case BASS_PULSATING:
                length = sizeof(bass_pulsating) / sizeof(uint16_t);
                musicSample = bass_pulsating;
                break;
            case BASS_RUMBLE_HUM:
                length = sizeof(bass_rumble_hum) / sizeof(uint16_t);
                musicSample = bass_rumble_hum;
                break;
            case CINEMATIC_SYNTH_RISER:
                length = sizeof(cinematic_synth_riser) / sizeof(uint16_t);
                musicSample = cinematic_synth_riser;
                break;
            case DEEP_DARK_HORROR_DRUM:
                length = sizeof(deep_dark_horror_drum) / sizeof(uint16_t);
                musicSample = deep_dark_horror_drum;
                break;
            case ELECTRIC_GUITAR:
                length = sizeof(electric_guitar) / sizeof(uint16_t);
                musicSample = electric_guitar;
                break;
            case GUITAR_STROKE_DOWN_SLOW:
                length = sizeof(guitar_stroke_down_slow) / sizeof(uint16_t);
                musicSample = guitar_stroke_down_slow;
                break;
            case GUITAR_STROKE_UP_SLOW:
                length = sizeof(guitar_stroke_up_slow) / sizeof(uint16_t);
                musicSample = guitar_stroke_up_slow;
                break;
            case MELODIC_GUITAR:
                length = sizeof(melodic_guitar) / sizeof(uint16_t);
                musicSample = melodic_guitar;
                break;
            case SYNTH_INTENSE_RHYTHM:
                length = sizeof(synth_intense_rhythm) / sizeof(uint16_t);
                musicSample = synth_intense_rhythm;
                break;
            case SYNTH_MECHANICAL_NOTIFICATION:
                length = sizeof(synth_mechanical_notification) / sizeof(uint16_t);
                musicSample = synth_mechanical_notification;
                break;
            case SYNTH_PASS:
                length = sizeof(synth_pass) / sizeof(uint16_t);
                musicSample = synth_pass;
                break;
            default:
                length = 0;
        }
    }

public:
    Sample() = default;
    ~Sample() = default;

    explicit Sample(Song song) : length(), musicSample(), selectedSong(song) {
        init(song);
    }

    [[nodiscard]] unsigned long long int getLength() const {
        return length;
    }

    [[nodiscard]] uint16_t *getMusicSample() const {
        return const_cast<uint16_t *>(musicSample);
    }

    [[nodiscard]] Song getSelectedSong() const {
        return selectedSong;
    }

    [[nodiscard]] const char* getName(){
        return LABELS[selectedSong];
    }
};


#endif //MCT_MP3MAPPINGS_H
