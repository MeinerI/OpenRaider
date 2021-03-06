/*!
 * \file include/system/SoundAL.h
 * \brief OpenAL Sound Implementation
 *
 * \author xythobuz
 */

#ifndef _SOUND_AL_H_
#define _SOUND_AL_H_

#include <string>
#include <vector>

class SoundAL {
  public:
    static int initialize();
    static void shutdown();
    static void clear();

    static int numBuffers();
    static int loadBuffer(unsigned char* buffer, unsigned int length);

    static int numSources(bool atListener);
    static int addSource(int buffer, float vol, bool atListener, bool loop);

    static int sourceAt(int source, glm::vec3 pos);
    static void listenAt(glm::vec3 pos, glm::vec3 at, glm::vec3 up);

    static void play(int source, bool atListener);
    static void stop(int source);
    static void stopAll();

    static void setEnabled(bool on);
    static bool getEnabled();

    static void setVolume(float vol);
    static float getVolume();

    static std::string getVersion(bool linked);

  private:
    static bool init;
    static bool enabled;
    static float volume;

    static std::vector<unsigned int> buffers;
    static std::vector<unsigned int> sources;
    static std::vector<unsigned int> listenerSources;

    static glm::vec3 lastPosition;
};

#endif

