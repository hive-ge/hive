#pragma once

#include <algorithm>
#include <chrono>
#include <stdio.h>

namespace hive {
namespace interface {
namespace keyboard {

using milliseconds = std::chrono::duration<unsigned, std::ratio<1, 1000>>;

/**
 * Key names and their respective
 * KEY_STATUS index.
 */
enum KeyName : unsigned char {
    ZERO = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    A = 10,
    B = 11,
    C = 12,
    D = 13,
    E = 14,
    F = 15,
    G = 16,
    H = 17,
    I = 18,
    J = 19,
    K = 20,
    L = 21,
    M = 22,
    N = 23,
    O = 24,
    P = 25,
    Q = 26,
    R = 27,
    S = 28,
    T = 29,
    U = 30,
    V = 31,
    W = 32,
    X = 33,
    Y = 34,
    Z = 35,
    F1 = 36,
    F2 = 37,
    F3 = 38,
    F4 = 39,
    F5 = 40,
    F6 = 41,
    F7 = 42,
    F8 = 43,
    F9 = 44,
    ESC = 45,
    CAPS_LOCK = 46,
    LEFT_SHIFT = 47,
    RIGHT_SHIFT = 48,
    LEFT_CTRL = 49,
    RIGHT_CTR = 50,
    SPACE = 51,
    LEFT_ALT = 52,
    RIGHT_ALT = 53,
    ENTER = 54,
    UP_ARROW = 55,
    DOWN_ARROW = 56,
    LEFT_ARROW = 57,
    RIGHT_ARROW = 58,
    INSERT = 59,
    HOME = 60,
    DELETE = 61,
    END = 62,
    PAGE_UP = 63,
    PAGE_DN = 64,
    COMMA = 65,
    PERIOD = 66,
    OPEN_BRACKET = 67,
    CLOSE_BRACKET = 68,
    FORWARD_SLASH = 69,
    BACK_SLASH = 70,
    MINUS = 71,
    EQUAL = 72,
    NUM_LOCK = 73,
    BACKSPACE = 74,
    RETURN = 75,
    META = 76,
    TAB = 77
};

class Keyboard {
    /**
     * A global object that can be updated
     * to indicate a certain key has been
     * pressed. It supports time information
     * through a last hit time stamp if
     * the value is not zero.
     */
    unsigned KEY_STATUS[128]{0};

    unsigned last_evaluated_time = 0;

    std::chrono::time_point<std::chrono::system_clock> start =
        std::chrono::system_clock::now();

  public:
    void setKey(KeyName key) {
        const unsigned key_state = KEY_STATUS[key];

        if (key_state > 0)
            return;

        KEY_STATUS[key] = last_evaluated_time;
    }

    void unsetKey(KeyName key) { KEY_STATUS[key] = 0; }

    unsigned isKEYSET(KeyName key) {

        const unsigned key_state = KEY_STATUS[key];

        return std::max(last_evaluated_time - key_state, 0u);
    }

    void update() {
        auto time = (start - std::chrono::system_clock::now());
        last_evaluated_time = time.count();
    };

}; // namespace keyboard
} // namespace keyboard
} // namespace interface
} // namespace hive