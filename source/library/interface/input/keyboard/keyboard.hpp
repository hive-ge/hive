#pragma once

#include <algorithm>
#include <chrono>
#include <stdio.h>

namespace hive
{
    namespace interface
    {
        namespace keyboard
        {

            using milliseconds = std::chrono::duration<unsigned, std::ratio<1, 1000>>;

            /**
             * Key names and their respective
             * KEY_STATUS index.
             */
            enum KeyName : unsigned char {
                ZERO = 0,
                ONE,
                TWO,
                THREE,
                FOUR,
                FIVE,
                SIX,
                SEVEN,
                EIGHT,
                NINE,
                A,
                B,
                C,
                D,
                E,
                F,
                G,
                H,
                I,
                J,
                K,
                L,
                M,
                N,
                O,
                P,
                Q,
                R,
                S,
                T,
                U,
                V,
                W,
                X,
                Y,
                Z,
                F1,
                F2,
                F3,
                F4,
                F5,
                F6,
                F7,
                F8,
                F9,
                F10,
                F11,
                F12,
                F13,
                F14,
                F15,
                F16,
                F17,
                F18,
                F19,
                F20,
                F21,
                F22,
                F23,
                F24,
                F25,
                KEYPAD_ZERO,
                KEYPAD_ONE,
                KEYPAD_TWO,
                KEYPAD_THREE,
                KEYPAD_FOUR,
                KEYPAD_FIVE,
                KEYPAD_SIX,
                KEYPAD_SEVEN,
                KEYPAD_EIGHT,
                KEYPAD_NINE,
                KEYPAD_PERIOD,
                KEYPAD_DIVIDE,
                KEYPAD_MULTIPLY,
                KEYPAD_SUBTRACT,
                KEYPAD_ADD,
                KEYPAD_ENTER,
                KEYPAD_EQUAL,
                ESCAPE,
                CAPS_LOCK,
                LEFT_SHIFT,
                RIGHT_SHIFT,
                LEFT_CONTROL,
                RIGHT_CONTROL,
                SPACE,
                LEFT_ALT,
                RIGHT_ALT,
                ENTER,
                UP_ARROW,
                DOWN_ARROW,
                LEFT_ARROW,
                RIGHT_ARROW,
                INSERT,
                HOME,
                DELETE,
                END,
                PAGE_UP,
                PAGE_DN,
                COMMA,
                PERIOD,
                OPEN_BRACKET,
                CLOSE_BRACKET,
                FORWARDSLASH,
                BACKSLASH,
                MINUS,
                EQUAL,
                NUM_LOCK,
                BACKSPACE,
                RETURN,
                LEFT_META,
                RIGHT_META,
                TAB,
                APOSTROPHE,
                SEMICOLON,
                PAGE_DOWN,
                SCROLL_LOCK,
                PRINT_SCREEN,
                PAUSE,
                TILDE,
                MENU,
                WORLD_1,
                WORLD_2
            };

            class Keyboard
            {
                /**
                 * A global object that can be updated
                 * to indicate a certain key has been
                 * pressed. It supports time information
                 * through a last hit time stamp if
                 * the value is not zero.
                 */
                int KEY_STATUS[128]{0};

              public:
                class KeyStateReader
                {
                  private:
                    Keyboard & keyboard;

                  public:
                    KeyStateReader(Keyboard & kb) : keyboard(kb) {}

                    /**
                     * Returns true if the key is currently pressed.
                     */
                    bool isKeyPressed(KeyName key) { return keyboard.KEY_STATUS[key] > 0; }

                    /**
                     * Get length of time a key has been pressed in milliseconds.
                     *
                     * This value always returns the last key press duration,
                     * regardless of whether the key is currently being pressed or
                     * not.
                     *
                     * Use Keyboard~isKeySet to determine if the key is actually
                     * being pressed or not.
                     *
                     * This will also reset the keypress duration to zero if the
                     * key is not being pressed.
                     */
                    unsigned getKeyPressDuration(KeyName key)
                    {
                        const int key_state_timestamp = keyboard.KEY_STATUS[key];

                        // Keypress duration already consumed, or key has never been pressed.
                        if (key_state_timestamp == 0) return 0;

                        if (key_state_timestamp < 0) {
                            // Key is not pressed but has been pressed since last check.
                            keyboard.KEY_STATUS[key] = 0;
                            return -key_state_timestamp;
                        } else {
                            // We need to do some chronology to handle a key that
                            // is currently being pressed.

                            // Currently key_state represents an integer timestamp
                            // since the beginning of program execution. We need to get
                            // an updated value for the current timestamp, convert
                            // that to an integer, and subtract key_state from the current
                            // time stamp to get the actual duration.

                            const int new_time_stamp = keyboard.getTimeStamp();

                            return new_time_stamp - key_state_timestamp;
                        }
                    }
                };

              private:
                KeyStateReader reader;


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
                Keyboard() : reader(*this){};

                void setKey(KeyName key)
                {
                    const int key_timestamp = KEY_STATUS[key];

                    if (key_timestamp > 0) return;

                    KEY_STATUS[key] = getTimeStamp();
                }

                void unsetKey(KeyName key)
                {
                    const int key_timestamp = KEY_STATUS[key];

                    if (key_timestamp > 0) {
                        KEY_STATUS[key] = -(getTimeStamp() - key_timestamp);
                    }
                }

                KeyStateReader & getKeyStateReader() { return reader; }


            }; // namespace keyboard
        }      // namespace keyboard
    }          // namespace interface
} // namespace hive