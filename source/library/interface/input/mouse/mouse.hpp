#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdio.h>

namespace hive
{
    namespace interface
    {
        namespace mouse
        {

            using milliseconds = std::chrono::duration<unsigned, std::ratio<1, 1000>>;

            /**
             * Key names and their respective
             * KEY_STATUS index.
             */
            enum ButtonName : char {
                ONE    = 0,
                LEFT   = 0,
                TWO    = 1,
                RIGHT  = 1,
                THREE  = 2,
                MIDDLE = 2,
                FOUR,
                FIVE,
                SIX,
                SEVEN,
                EIGHT,
                NINE,
                TEN,
            };

            class Mouse
            {
                /**
                 * A global object that can be updated
                 * to indicate a certain button has been
                 * pressed. It supports time information
                 * through a last hit time stamp if
                 * the value is not zero.
                 */
                int BUTTON_STATUS[10]{0};

                double x = 0.0;
                double y = 0.0;

                unsigned BUTTON_STATE_CHANGED = 0;

                /**
                 * true if the mouse is active / connected.
                 */
                bool IS_ALIVE = false;

              public:
                /**
                 * Class for reading the state of a mouse.
                 *
                 * A mouse is defined as set of buttons and
                 * input axes.
                 *
                 * A mouse can have up to 20 buttons and 10 axes.
                 * The mouse may not actually have this number of buttons
                 * or axes.
                 */
                class MouseStateReader
                {
                  private:
                    Mouse & mouse;

                  public:
                    MouseStateReader(Mouse & kb) : mouse(kb) {}

                    /**
                     * Returns true if the button is currently pressed.
                     */
                    bool isButtonPressed(ButtonName button) const
                    {
                        return mouse.BUTTON_STATUS[button] > 0;
                    }

                    /**
                     * Get length of time a button has been pressed in milliseconds.
                     *
                     * This value always returns the last button press duration,
                     * regardless of whether the button is currently being pressed or
                     * not.
                     *
                     * Use Keyboard~isKeySet to determine if the button is actually
                     * being pressed or not.
                     *
                     * This will also reset the buttonpress duration to zero if the
                     * button is not being pressed.
                     */
                    unsigned getButtonPressDuration(ButtonName button) const
                    {
                        const int button_state_timestamp = mouse.BUTTON_STATUS[button];

                        // Keypress duration already consumed, or button has never been pressed.
                        if (button_state_timestamp == 0) return 0;

                        if (button_state_timestamp < 0) {
                            // Key is not pressed but has been pressed since last check.
                            mouse.BUTTON_STATUS[button] = 0;
                            return -button_state_timestamp;
                        } else {
                            // We need to do some chronology to handle a button that
                            // is currently being pressed.

                            // Currently button_state represents an integer timestamp
                            // since the beginning of program execution. We need to get
                            // an updated value for the current timestamp, convert
                            // that to an integer, and subtract button_state from the current
                            // time stamp to get the actual duration.

                            const int new_time_stamp = mouse.getTimeStamp();

                            return new_time_stamp - button_state_timestamp;
                        }
                    }

                    double getX() const { return mouse.x; }
                    double getY() const { return mouse.y; }

                    /**
                     * Has any button changed since the last call to poll;
                     */
                    bool hasChanged() const { return mouse.BUTTON_STATE_CHANGED > 0; }

                    /**
                     * Has the button changed since the last call to poll;
                     */
                    bool hasButtonStateChanged(ButtonName button)
                    {
                        return ((1 << (unsigned)button) & mouse.BUTTON_STATE_CHANGED) > 0;
                    };


#ifdef HIVE_DEBUG //################### DEBUG
                    friend std::ostream & operator<<(std::ostream & os, const MouseStateReader & r)
                    {
                        os << "{ \"x\": " << r.getX();
                        os << ", \"y\": " << r.getY();
                        os << ", \"buttons\": [";
                        os << " ], \"buttons:[ ";
                        for (int i = 0; i < 10; i++) {
                            os << ":{"
                               << " \"press\": " << r.isButtonPressed((ButtonName)i)
                               << " \"dur\": " << r.getButtonPressDuration((ButtonName)i) << " }";
                            if (i < 9) os << ", ";
                        }
                        os << " ]";
                        return os;
                    }


#endif //############################## END DEBUG

#ifndef HIVE_DEBUG //################## RELEASE

                    friend std::ostream & operator<<(std::ostream & os,
                                                     const ControllerStateReader & reader)
                    {
                        return os;
                    }
#endif //############################## END RELEASE
                };

              private:
                MouseStateReader reader;

                std::chrono::time_point<std::chrono::system_clock> start =
                    std::chrono::system_clock::now();

                /**
                 * Return an integer timestamp since program start.
                 */
                inline int getTimeStamp()
                {
                    const auto mil = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now() - start);

                    return mil.count();
                }

              public:
                Mouse() : reader(*this){};

                void setX(double X) { x = X; };
                void setY(double Y) { y = Y; };

                void setButton(ButtonName button, unsigned char preassure = 0)
                {
                    const int button_timestamp = BUTTON_STATUS[button];

                    if (button_timestamp > 0) return;

                    BUTTON_STATE_CHANGED |= 1 << button;

                    BUTTON_STATUS[button] = getTimeStamp();
                }

                void unsetButton(ButtonName button)
                {
                    const int button_timestamp = BUTTON_STATUS[button];

                    if (button_timestamp > 0) {
                        BUTTON_STATE_CHANGED |= 1 << button;

                        BUTTON_STATUS[button] = -(getTimeStamp() - button_timestamp);
                    }
                }

                MouseStateReader & getMouseStateReader() { return reader; }


            }; // namespace buttonboard
        }      // namespace mouse
    }          // namespace interface
} // namespace hive