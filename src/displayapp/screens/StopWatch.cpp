#include "../DisplayApp.h"

#include "StopWatch.h"

using namespace Pinetime::Applications::Screens;

using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

extern lv_font_t jetbrains_mono_extrabold_compressed;
extern lv_font_t jetbrains_mono_bold_20;
extern lv_style_t* LabelBigStyle;

StopWatch::StopWatch(DisplayApp* app) : Screen(app) {
  label = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align(label, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 0);
  lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
}

StopWatch::~StopWatch() {
  lv_obj_clean(lv_scr_act());
}

bool StopWatch::Refresh() {
  if (stopWatchRunning) {
    currentTime = getCurrentTime();
  }

  // TODO: is this necessary when not changing?
  lv_label_set_text_fmt(label, "%2.2f", currentTime);

  return running;
}

bool StopWatch::OnButtonPushed() {
  running = false;
  return true;
}

bool StopWatch::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  switch(event) {
    case TouchEvents::Tap:
      if (stopWatchRunning) {
        stop();
      } else {
        start();
      }
      return true;
    case TouchEvents::SwipeDown:
      reset();
      return true;
    default:
      return false;
  }
}

void StopWatch::start() {
  startTime = system_clock::now();
  stopWatchRunning = true;
}

void StopWatch::stop() {
  stopWatchRunning = false;
}

void StopWatch::reset() {
  currentTime = 0.0f;
}

float StopWatch::getCurrentTime() {
  duration<float> delta = duration_cast<duration<float>>(system_clock::now() - startTime);
  return delta.count();
}