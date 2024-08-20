#include "Timeline.h"

void Timeline::Play() {
  timeStarted = glfwGetTime();
  state = State::PLAYING;
}

void Timeline::Pause() {
  state = State::PAUSED;  //
}

void Timeline::Stop() {
  iTime = 0;
  state = State::STOPPED;
}

void Timeline::Update() {
  float now = glfwGetTime();
  float timeDelta = now - timeStarted;
  timeStarted = now;
  iTime += timeDelta;
};
