# TimerTask Class for Arduino

## Overview
The `TimerTask` class is designed for Arduino projects to facilitate easy timing and execution of tasks at set intervals. It offers functionality to execute a specific task repeatedly at a defined interval, with additional control for pausing, resuming, and rearming the timer.

## Features
- Execute a task at a specified interval.
- Control automatic retriggering of the task after each interval.
- Pause and resume timer functionality.
- Rearm the timer to reset and optionally restart it.
- Option to use incremental time updates to reduce cumulative timing errors.

## Usage

### Including the Class
To use the `TimerTask` class in your Arduino project, include the `TimerTask.h` file at the beginning of your sketch.

```cpp
#include "TimerTask.h"
```

### Constructor
The constructor initializes the timer with specific settings.

```cpp
TimerTask(void (*func)() = NULL, unsigned long newInterval = 1000, bool startPaused = false, bool autoRetrigger = true)
```

- `func`: Pointer to the function to be called when the interval elapses.
- `newInterval`: The interval in milliseconds for the task execution.
- `startPaused`: If `true`, the timer starts in a paused state.
- `autoRetrigger`: Determines if the task should automatically retrigger.

### checkInterval Method
The `checkInterval` method checks if the interval has elapsed and executes the task if so. It now includes an `incrementalTime` parameter to reduce timing errors.

```cpp
bool checkInterval(bool incrementalTime = false)
```

- `incrementalTime`: When set to `true`, the method uses incremental time updates to avoid cumulative timing errors. It ensures the task is not executed multiple times in quick succession if the timer is checked after a long delay.

### rearm Method
Resets and optionally restarts the timer. This method is particularly useful when `autoRetrigger` is set to `false`.

```cpp
void rearm(bool resetTime = true, bool startTimer = true)
```

- `resetTime`: If true, resets the timer's `lastTime` to the current time (`millis()`). This is useful for restarting the timer from the current moment.
- `startTimer`: If true, resumes the timer if it is paused. This allows immediate continuation of the task execution. If false, the timer remains paused, and manual resumption (`resume()`) is needed.

### Creating an Instance
Create an instance of the `TimerTask` class by specifying the task, interval, and other settings.

```cpp
TimerTask myTimer(myTaskFunction, 1000, false, true);
```

### Other Methods
- `setTask(void (*func)())`: Set or change the task function.
- `setInterval(unsigned long newInterval)`: Set or change the interval.
- `pause()`: Pause the timer.
- `resume()`: Resume the timer.
- `isTriggered()`: Check if the timer has been triggered (useful when autoRetrigger is `false`).

### Example
```cpp
#include "TimerTask.h"

void toggleLed() {
    // Toggle LED logic
}

TimerTask ledTimer(toggleLed, 1000);

void setup() {
    // Setup code
}

void loop() {
    ledTimer.checkInterval(true);
}
```

### Example
```cpp
#include "TimerTask.h"

// Define the pin numbers
const int ledPin = 13;
const int buttonPin = 2;

// Flag to store LED state
bool ledState = false;

// Function to toggle the LED
void toggleLed() {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
}

// Create a TimerTask instance. The task is to toggle the LED every 1000ms (1 second).
// Start with auto-retrigger disabled.
TimerTask timerTask(toggleLed, 1000, false, false);

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP); // Set the button pin to INPUT_PULLUP mode

    // Start with the LED off
    digitalWrite(ledPin, LOW);
}

void loop() {
    // Check if the interval has elapsed and the task needs to be executed
    timerTask.checkInterval();

    // Check if the button is pressed (will read LOW due to INPUT_PULLUP mode)
    if (digitalRead(buttonPin) == LOW) {
        // Rearm the timer - resets the timer and resumes it
        timerTask.rearm();

        // Debounce delay
        delay(50);
        while(digitalRead(buttonPin) == LOW); // Wait for the button to be released
        delay(50);
    }
}
```


## Contributing
Contributions to the `TimerTask` class are welcome. Please feel free to submit pull requests or open issues to discuss potential improvements or report bugs.

## License
This project is released under the [MIT License](https://opensource.org/licenses/MIT).
