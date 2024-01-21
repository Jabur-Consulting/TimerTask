// TimerTask.h
#ifndef TIMER_TASK_H
#define TIMER_TASK_H

#include <Arduino.h>

class TimerTask {
public:
  // Constructor: Initializes the timer task with a function, interval, initial paused state, and auto-retrigger behavior.
  TimerTask(void (*func)() = NULL, unsigned long newInterval = 1000, bool startPaused = false, bool autoRetrigger = true) {
    task = func;
    interval = newInterval;
    isPaused = startPaused;
    this->autoRetrigger = autoRetrigger;
    triggered = false;
    lastTime = millis();
    pauseTime = 0;
  }

  // Set the task function to be called on interval completion.
  void setTask(void (*func)()) {
    task = func;
  }

  // Set the time interval for the task execution.
  void setInterval(unsigned long newInterval) {
    interval = newInterval;
  }

  // Check if the interval has elapsed and execute the task if so. Returns true if the interval has elapsed.
  bool checkInterval(bool incrementalTime = false) {
    if (!isPaused && millis() - lastTime >= interval) {

      if (incrementalTime) {
        unsigned long currentTime = millis();
        while (lastTime + interval <= currentTime) {
          lastTime += interval;
        }
      } else {
        lastTime = millis();
      }

      if (!autoRetrigger) {
        triggered = true;
        pause();
      }
      if (task != NULL) {
        task();
      }
      return true;
    }
    return false;
  }

  // Pause the timer. Saves the current time to calculate the paused duration later.
  void pause() {
    if (!isPaused) {
      isPaused = true;
      pauseTime = millis();
    }
  }

  // Resume the timer. Adjusts the lastTime to account for the paused duration.
  void resume() {
    if (isPaused) {
      isPaused = false;
      lastTime += millis() - pauseTime;
    }
  }

  // Check if the timer has been triggered (used when autoRetrigger is false).
  bool isTriggered() const {
    return triggered;
  }

  // Rearm the timer. Resets the timer's state and optionally starts the timer based on the parameters.
  void rearm(bool resetTime = true, bool startTimer = true) {
    triggered = false;

    if (resetTime) {
      lastTime = millis();  // Reset lastTime to current time
      if (isPaused) {
        pauseTime = millis();  // Also reset pauseTime if the timer is paused
      }
    }

    if (startTimer) {
      resume();  // Resume the timer if startTimer is true
    } else {
      pause();  // Keep the timer paused if startTimer is false
    }
  }

private:
  unsigned long lastTime;   // Tracks the last time the task was executed or the timer was started/resumed.
  unsigned long interval;   // The time interval in milliseconds for the task execution.
  void (*task)();           // The task function to be executed.
  bool isPaused;            // Indicates whether the timer is paused.
  unsigned long pauseTime;  // Stores the time at which the timer was paused.
  bool autoRetrigger;       // Indicates whether the timer should automatically restart after each interval.
  bool triggered;           // Flag to indicate whether the timer has been triggered (used in non-autoRetrigger mode).
};

#endif  // TIMER_TASK_H
