#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdio.h>

namespace hive
{
    namespace interface
    {
        namespace controller
        {

            using milliseconds = std::chrono::duration<unsigned, std::ratio<1, 1000>>;

            /**
             * Key names and their respective
             * KEY_STATUS index.
             */
            enum ButtonName : char {
                ONE,
                TWO,
                THREE,
                FOUR,
                FIVE,
                SIX,
                SEVEN,
                EIGHT,
                NINE,
                TEN,
                ELEVEN,
                TWELVE,
                THIRTEEN,
                FOURTEEN,
                FIFTEEN,
                SIXTEEN,
                SEVENTEEN,
                EIGHTEEN,
                NINETEEN,
                TWENTY
            };

            enum class AxisName : char {
                ONE,
                TWO,
                THREE,
                FOUR,
                FIVE,
                SIX,
                SEVEN,
                EIGHT,
                NINE,
                TEN,
                ELEVEN,
                TWELVE,
                THIRTEEN,
                FOURTEEN,
                FIFTEEN,
                SIXTEEN,
                SEVENTEEN,
                EIGHTEEN,
                NINETEEN,
                TWENTY
            };

            class Controller
            {
                /**
                 * A global object that can be updated
                 * to indicate a certain button has been
                 * pressed. It supports time information
                 * through a last hit time stamp if
                 * the value is not zero.
                 */
                int BUTTON_STATUS[20]{0};
                unsigned BUTTON_PREASSURE_STATUS[20]{0};

                const float * axes            = nullptr;
                const unsigned char * buttons = nullptr;

                char MAX_AXES    = 0;
                char MAX_BUTTONS = 0;

                unsigned BUTTON_STATE_CHANGED = 0;

                /**
                 * true if the controller is active / connected.
                 */
                bool IS_ALIVE = false;

              public:
                /**
                 * Class for reading the state of a controller.
                 *
                 * A controller is defined as set of buttons and
                 * input axes.
                 *
                 * A controller can have up to 20 buttons and 10 axes.
                 * The controller may not actually have this number of buttons
                 * or axes.
                 */
                class ControllerStateReader
                {
                  private:
                    Controller & controller;

                    char getMaxAxis() const { return controller.MAX_AXES; }
                    char getMaxButtons() const { return controller.MAX_BUTTONS; }

                  public:
                    ControllerStateReader(Controller & kb) : controller(kb) {}

                    /**
                     * Returns true if the button is currently pressed.
                     */
                    bool isButtonPressed(ButtonName button) const
                    {
                        return controller.BUTTON_STATUS[button] > 0;
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
                        const int button_state_timestamp = controller.BUTTON_STATUS[button];

                        // Keypress duration already consumed, or button has never been pressed.
                        if (button_state_timestamp == 0) return 0;

                        if (button_state_timestamp < 0) {
                            // Key is not pressed but has been pressed since last check.
                            controller.BUTTON_STATUS[button] = 0;
                            return -button_state_timestamp;
                        } else {
                            // We need to do some chronology to handle a button that
                            // is currently being pressed.

                            // Currently button_state represents an integer timestamp
                            // since the beginning of program execution. We need to get
                            // an updated value for the current timestamp, convert
                            // that to an integer, and subtract button_state from the current
                            // time stamp to get the actual duration.

                            const int new_time_stamp = controller.getTimeStamp();

                            return new_time_stamp - button_state_timestamp;
                        }
                    }

                    /**
                     * Get the preassure a button was pressed with. This value is always
                     * returned as the value set when button was last pressed, regardless
                     * of whether the button is currently pressed.
                     */
                    unsigned getButtonPreassure(ButtonName button) const
                    {
                        return controller.BUTTON_PREASSURE_STATUS[button];
                    }

                    float getAxis(AxisName axis) const
                    {
                        if (controller.IS_ALIVE && (unsigned char)axis < controller.MAX_AXES)
                            return controller.axes[(unsigned char)axis];

                        return 0.;
                    }

                    /**
                     * Has any button changed since the last call to poll;
                     */
                    bool hasChanged() const { return controller.BUTTON_STATE_CHANGED > 0; }

                    /**
                     * Has the button changed since the last call to poll;
                     */
                    bool hasButtonStateChanged(ButtonName button) const
                    {
                        return ((1 << (unsigned)button) & controller.BUTTON_STATE_CHANGED) > 0;
                    };

                    void poll() const { controller.poll(); }

                    bool isALIVE() const { return controller.IS_ALIVE; }

#ifdef HIVE_DEBUG //################### DEBUG
                    friend std::ostream & operator<<(std::ostream & os,
                                                     const ControllerStateReader & r)
                    {
                        const auto & ctrl = r.controller;
                        os << "{ \"axes\":[ ";
                        for (int i = 0; i < r.getMaxAxis(); i++) {
                            os << r.getAxis((AxisName)i);
                            if (i < r.getMaxAxis() - 1) os << " , ";
                        }
                        os << " ], \"buttons:[ ";
                        for (int i = 0; i < r.getMaxButtons(); i++) {
                            os << ":{"
                               << " \"press\": " << r.isButtonPressed((ButtonName)i)
                               << " \"dur\": " << r.getButtonPressDuration((ButtonName)i) << " }";
                            if (i < r.getMaxButtons() - 1) os << ", ";
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
                ControllerStateReader reader;


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
                Controller() : reader(*this){};

                bool isALIVE() { return IS_ALIVE; }

                void setAliveStatus(bool is_alive = false) { IS_ALIVE = is_alive; }

                void setButton(ButtonName button, unsigned char preassure = 0)
                {
                    const int button_timestamp = BUTTON_STATUS[button];

                    if (button_timestamp > 0) return;

                    BUTTON_STATE_CHANGED |= 1 << button;

                    BUTTON_PREASSURE_STATUS[button] = preassure;
                    BUTTON_STATUS[button]           = getTimeStamp();
                }

                void unsetButton(ButtonName button)
                {
                    const int button_timestamp = BUTTON_STATUS[button];

                    if (button_timestamp > 0) {
                        BUTTON_STATE_CHANGED |= 1 << button;

                        BUTTON_STATUS[button] = -(getTimeStamp() - button_timestamp);
                    }
                }

                void setAxisArrayPointer(const float * array, size_t length)
                {
                    axes     = array;
                    MAX_AXES = length;
                }

                void setButtonArrayPointer(const unsigned char * array, size_t length)
                {
                    buttons     = array;
                    MAX_BUTTONS = length;
                }

                void poll()
                {
                    if (IS_ALIVE) {

                        for (int i = 0; i < MAX_AXES; i++) {
                        }

                        BUTTON_STATE_CHANGED = 0;

                        for (unsigned char i = 0; i < MAX_BUTTONS; i++) {
                            if (buttons[i] == 0)
                                unsetButton((ButtonName)i);
                            else
                                setButton((ButtonName)i);
                        }
                    }
                }

                ControllerStateReader & getControllerStateReader() { return reader; }


            }; // namespace buttonboard
        }      // namespace controller
    }          // namespace interface
} // namespace hive