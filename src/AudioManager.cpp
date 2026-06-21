#include "AudioManager.hpp"

std::shared_ptr<Util::BGM> AudioManager::m_CurrentBGM = nullptr;
std::string AudioManager::m_CurrentPath = "";

void AudioManager::PlayBGM(const std::string& path, int volume) {
    if (m_CurrentPath != path) {
        m_CurrentPath = path;
        m_CurrentBGM = std::make_shared<Util::BGM>(path);
        if (volume >= 0) {
            m_CurrentBGM->SetVolume(volume);
        }
        m_CurrentBGM->Play();
    } else {
        if (m_CurrentBGM && volume >= 0) {
            m_CurrentBGM->SetVolume(volume);
        }
    }
}

void AudioManager::StopBGM() {
    if (m_CurrentBGM) {
        m_CurrentBGM->Pause();
    }
    m_CurrentPath = "";
}

void AudioManager::PlaySFX(const std::string& path, int volume) {
    auto sfx = std::make_shared<Util::SFX>(path);
    if (volume >= 0) {
        sfx->SetVolume(volume);
    }
    sfx->Play();
}
