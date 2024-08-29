#pragma once
#include "../common.h"

class Timeline {
 public:
  float iTime = 0;
  bool rendered = false;

  inline bool IsPlaying() { return state == PLAYING; }
  inline bool IsPaused() { return state == PAUSED; }
  inline bool IsStopped() { return state == STOPPED; }

  void Play();
  void Pause();
  void Stop();
  void Update();

 private:
  enum State { PLAYING, PAUSED, STOPPED };
  State state = STOPPED;
  float timeStarted = 0;
};
