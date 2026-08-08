#include "audio_system.h"
#include "engine/core/base/macro.h"

// Prevent miniaudio from defining its implementation here again to speed up compile times.
// The implementation is compiled once in external/miniaudio/miniaudio.c
#include "miniaudio.h"
#include <vector>

namespace Lacrima
{
    struct AudioSystem::Impl
    {
        ma_engine engine;
        bool initialized = false;
        std::vector<ma_sound> active_sounds;
    };

    AudioSystem::AudioSystem() : m_impl(std::make_unique<Impl>())
    {
    }

    AudioSystem::~AudioSystem()
    {
        destroy();
    }

    void AudioSystem::init()
    {
        if (m_impl->initialized)
        {
            return;
        }

        ma_result result = ma_engine_init(NULL, &m_impl->engine);
        if (result != MA_SUCCESS)
        {
            LOG_ERROR("Failed to initialize miniaudio engine");
            return;
        }

        m_impl->initialized = true;
        LOG_INFO("AudioSystem initialized successfully");
    }

    void AudioSystem::destroy()
    {
        if (!m_impl->initialized)
        {
            return;
        }

        // Clean up any active looping/custom sounds
        for (auto& sound : m_impl->active_sounds)
        {
            ma_sound_uninit(&sound);
        }
        m_impl->active_sounds.clear();

        ma_engine_uninit(&m_impl->engine);
        m_impl->initialized = false;
        LOG_INFO("AudioSystem destroyed");
    }

    void AudioSystem::playSound(const std::string& filepath, bool loop)
    {
        if (!m_impl->initialized)
        {
            LOG_ERROR("AudioSystem not initialized, cannot play sound: {}", filepath);
            return;
        }

        if (loop)
        {
            ma_sound sound;
            ma_result result = ma_sound_init_from_file(&m_impl->engine, filepath.c_str(), 0, NULL, NULL, &sound);
            if (result == MA_SUCCESS)
            {
                ma_sound_set_looping(&sound, MA_TRUE);
                ma_sound_start(&sound);
                m_impl->active_sounds.push_back(sound);
            }
            else
            {
                LOG_ERROR("Failed to load looping sound: {}", filepath);
            }
        }
        else
        {
            ma_engine_play_sound(&m_impl->engine, filepath.c_str(), NULL);
        }
    }
}
