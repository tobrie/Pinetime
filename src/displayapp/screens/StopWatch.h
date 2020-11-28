#pragma once

#include <chrono>
#include <libs/lvgl/lvgl.h>
#include "Screen.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class StopWatch : public Screen {
        public:
          StopWatch(DisplayApp *app, Controllers::DateTime& dateTimeController);
          ~StopWatch() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

          bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;
        private:
          void start();
          void stop();
          void reset();
          float getCurrentTime();

          float currentTime = 0.0f;
          bool stopWatchRunning = false;

          Controllers::DateTime& dateTimeController;
          std::chrono::system_clock::time_point startTime;

          lv_obj_t *label;
          lv_obj_t *start_stop_label;

          bool running = true;
      };
    }
  }
}
