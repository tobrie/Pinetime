#include "../DisplayApp.h"
#include "components/datetime/DateTimeController.h"
#include "StopWatch.h"
#include "Symbols.h"

using namespace Pinetime::Applications::Screens;

using std::chrono::duration;
using std::chrono::duration_cast;

extern lv_font_t jetbrains_mono_extrabold_compressed;
extern lv_font_t jetbrains_mono_bold_20;

StopWatch::StopWatch(DisplayApp* app,
                     Controllers::DateTime& dateTimeController) : Screen(app),
                     dateTimeController{dateTimeController} {

  label = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align(label, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
  lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);

  start_stop_label = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align(start_stop_label, label, LV_ALIGN_CENTER, 0, 10);
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
      if (!stopWatchRunning) {
        reset();
      }
      return true;
    default:
      return false;
  }
}

void StopWatch::start() {
  startTime = dateTimeController.CurrentDateTime();
  lv_label_set_text(start_stop_label, Symbols::play);
  stopWatchRunning = true;
}

void StopWatch::stop() {
  lv_label_set_text(start_stop_label, Symbols::pause);
  stopWatchRunning = false;
}

void StopWatch::reset() {
  currentTime = 0.0f;
}

float StopWatch::getCurrentTime() {
  //TickType_t delta = xTaskGetTickCount() - startTime; // <- alternative
  duration<float> delta =
      duration_cast<duration<float>>(dateTimeController.CurrentDateTime() - startTime);
  return (float) delta.count();
}