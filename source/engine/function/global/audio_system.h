#pragma once

#include <string>
#include <memory>

namespace Lacrima
{
    class AudioSystem
    {
    public:
        AudioSystem();
        ~AudioSystem();

        void init();
        void destroy();

        void playSound(const std::string& filepath, bool loop = false);

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
