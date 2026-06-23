#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <string>
#include <memory>
#include "Util/BGM.hpp"
#include "Util/SFX.hpp"

class AudioManager {
private:
    static std::shared_ptr<Util::BGM> m_CurrentBGM;
    static std::string m_CurrentPath;

public:
    // Plays the BGM at the specified path. 
    // If the BGM is already playing, it won't restart.
    // volume is 0~128, -1 means default
    static void PlayBGM(const std::string& path, int volume = -1);

    // Stops (pauses) the current BGM.
    static void StopBGM();

    // Plays a sound effect
    // volume is 0~128, -1 means default
    static void PlaySFX(const std::string& path, int volume = -1);
};

#endif // AUDIO_MANAGER_HPP
