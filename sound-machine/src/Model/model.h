//Silas Schaprian
//Matrikelnummer: 3520242

#ifndef MCT_MODEL_H
#define MCT_MODEL_H

#include <cstdint>
#include "mp3Mappings.h"
#include "pcm_player.h"

/*
 * Class to interact between UI and PCM_Player
 */
class Model {
private:
    bool matrix[16][4]{};
    Sample samples[4];
    PCM_Player pcmPlayer;
    bool isPlaying;
    uint8_t currentColumn;

public:

    Model() : matrix{}, samples{}, pcmPlayer(), isPlaying(), currentColumn() {}

    ~Model() = default;

    [[nodiscard]] bool at(const uint8_t column, const uint8_t row) const {
        return matrix[column][row];
    }

    bool &at(const uint8_t column, const uint8_t row) {
        if(!isPlaying) {
            return matrix[column][row];
        }
    }

    [[nodiscard]] Sample *getSamples() {
        return samples;
    }

    /**
     *
     * @return Column which the player is currently plaing
     */
    [[nodiscard]] uint8_t getCurrentColumn(){
        return currentColumn;
    }

    /*
     * actual playing of Sounds
     */
    void play() {
        isPlaying = true;
        pcmPlayer.timer_reset();
        int left, right, count;
        pcm_value_t pcm;
        Sample sample{};
        int row;
        uint32_t sampleIndex;
        //loop over columns
        for (currentColumn = 0; currentColumn < 16; currentColumn++) {
            bool* column = matrix[currentColumn];
            for (sampleIndex = 0; sampleIndex < MCT_SAMPLE_RATE / 2; sampleIndex += 2) {
                // Reset values
                count = 0;
                left = 0;
                right = 0;

                //Add PCM samples to left/right
                for (row = 0; row < 4; row++) {
                    sample = samples[row];
                    if (column[row] && sampleIndex < sample.getLength()) {
                        left += (int16_t) sample.getMusicSample()[sampleIndex];
                        right += (int16_t) sample.getMusicSample()[sampleIndex + 1];
                        count++;
                    }
                }

                //scaling left and right
                left /= count;
                right /= count;
                //wait for pcm_driver
                while (!pcmPlayer.canPut())
                    task::sleep(10);
                pcm.left = (int16_t) left;
                pcm.right = (int16_t) right;
                pcmPlayer.put(pcm);
            }
        }
        isPlaying = false;
    }

};


#endif //MCT_MODEL_H
