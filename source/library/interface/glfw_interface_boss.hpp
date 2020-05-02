#pragma once

#include "interface/interface_boss.hpp"

//#ifdef HIVE_USE_OPENGL

#include <GL/gl.h>

#include <GL/glew.h>

#include "gpu/opengl/gl.hpp"

#define GLFW_INCLUDE_NONE

//#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

//#ifdef HIVE_USE_OPENGLFW

#define makePressCase(KEY_NAME)                                                                    \
    case GLFW_KEY_##KEY_NAME:                                                                      \
        glfwKeyboard.setKey(KeyName::KEY_NAME);                                                    \
        break;

#define makeRenamedPressCase(KEY_NAME, GLFW_NAME)                                                  \
    case GLFW_KEY_##GLFW_NAME:                                                                     \
        glfwKeyboard.setKey(KeyName::KEY_NAME);                                                    \
        break;

#define makeReleaseCase(KEY_NAME)                                                                  \
    case GLFW_KEY_##KEY_NAME:                                                                      \
        glfwKeyboard.unsetKey(KeyName::KEY_NAME);                                                  \
        break;

#define makeRenamedReleaseCase(KEY_NAME, GLFW_NAME)                                                \
    case GLFW_KEY_##GLFW_NAME:                                                                     \
        glfwKeyboard.unsetKey(KeyName::KEY_NAME);                                                  \
        break;

#define makeRenamedReleaseCaseMouse(KEY_NAME, GLFW_NAME)                                           \
    case GLFW_MOUSE_BUTTON_##GLFW_NAME:                                                            \
        glfwMouse.unsetButton(mouse::ButtonName::KEY_NAME);                                        \
        break;

#define makeRenamedPressCaseMouse(KEY_NAME, GLFW_NAME)                                             \
    case GLFW_MOUSE_BUTTON_##GLFW_NAME:                                                            \
        glfwMouse.setButton(mouse::ButtonName::KEY_NAME);                                          \
        break;


/* Printable keys */
namespace hive
{
    namespace interface
    {
        using namespace keyboard;
        using namespace controller;
        using namespace mouse;

        static keyboard::Keyboard glfwKeyboard;
        static mouse::Mouse glfwMouse;

        inline void GLFWKeyboardCallback(GLFWwindow * window, int key, int scancode, int action,
                                         int mods)
        {
            if (action == GLFW_PRESS) {
                switch (key) {
                    makePressCase(SPACE);
                    makePressCase(APOSTROPHE);
                    makePressCase(COMMA);
                    makePressCase(MINUS);
                    makePressCase(PERIOD);
                    makeRenamedPressCase(FORWARDSLASH, SLASH);
                    makeRenamedPressCase(ZERO, 0);
                    makeRenamedPressCase(ONE, 1);
                    makeRenamedPressCase(TWO, 2);
                    makeRenamedPressCase(THREE, 3);
                    makeRenamedPressCase(FOUR, 4);
                    makeRenamedPressCase(FIVE, 5);
                    makeRenamedPressCase(SIX, 6);
                    makeRenamedPressCase(SEVEN, 7);
                    makeRenamedPressCase(EIGHT, 8);
                    makeRenamedPressCase(NINE, 9);
                    makePressCase(SEMICOLON);
                    makePressCase(EQUAL);
                    makePressCase(A);
                    makePressCase(B);
                    makePressCase(C);
                    makePressCase(D);
                    makePressCase(E);
                    makePressCase(F);
                    makePressCase(G);
                    makePressCase(H);
                    makePressCase(I);
                    makePressCase(J);
                    makePressCase(K);
                    makePressCase(L);
                    makePressCase(M);
                    makePressCase(N);
                    makePressCase(O);
                    makePressCase(P);
                    makePressCase(Q);
                    makePressCase(R);
                    makePressCase(S);
                    makePressCase(T);
                    makePressCase(U);
                    makePressCase(V);
                    makePressCase(W);
                    makePressCase(X);
                    makePressCase(Y);
                    makePressCase(Z);
                    makeRenamedPressCase(OPEN_BRACKET, LEFT_BRACKET);
                    makePressCase(BACKSLASH);
                    makeRenamedPressCase(CLOSE_BRACKET, RIGHT_BRACKET);
                    makeRenamedPressCase(TILDE, GRAVE_ACCENT);
                    makePressCase(WORLD_1);
                    makePressCase(WORLD_2);
                    makePressCase(ESCAPE);
                    makePressCase(ENTER);
                    makePressCase(TAB);
                    makePressCase(BACKSPACE);
                    makePressCase(INSERT);
                    makePressCase(DELETE);
                    makeRenamedPressCase(RIGHT_ARROW, RIGHT);
                    makeRenamedPressCase(LEFT_ARROW, LEFT);
                    makeRenamedPressCase(DOWN_ARROW, DOWN);
                    makeRenamedPressCase(UP_ARROW, UP);
                    makePressCase(PAGE_UP);
                    makePressCase(PAGE_DOWN);
                    makePressCase(HOME);
                    makePressCase(END);
                    makePressCase(CAPS_LOCK);
                    makePressCase(SCROLL_LOCK);
                    makePressCase(NUM_LOCK);
                    makePressCase(PRINT_SCREEN);
                    makePressCase(PAUSE);
                    makePressCase(F1);
                    makePressCase(F2);
                    makePressCase(F3);
                    makePressCase(F4);
                    makePressCase(F5);
                    makePressCase(F6);
                    makePressCase(F7);
                    makePressCase(F8);
                    makePressCase(F9);
                    makePressCase(F10);
                    makePressCase(F11);
                    makePressCase(F12);
                    makePressCase(F13);
                    makePressCase(F14);
                    makePressCase(F15);
                    makePressCase(F16);
                    makePressCase(F17);
                    makePressCase(F18);
                    makePressCase(F19);
                    makePressCase(F20);
                    makePressCase(F21);
                    makePressCase(F22);
                    makePressCase(F23);
                    makePressCase(F24);
                    makePressCase(F25);
                    makeRenamedPressCase(KEYPAD_ZERO, KP_0);
                    makeRenamedPressCase(KEYPAD_ONE, KP_1);
                    makeRenamedPressCase(KEYPAD_TWO, KP_2);
                    makeRenamedPressCase(KEYPAD_THREE, KP_3);
                    makeRenamedPressCase(KEYPAD_FOUR, KP_4);
                    makeRenamedPressCase(KEYPAD_FIVE, KP_5);
                    makeRenamedPressCase(KEYPAD_SIX, KP_6);
                    makeRenamedPressCase(KEYPAD_SEVEN, KP_7);
                    makeRenamedPressCase(KEYPAD_EIGHT, KP_8);
                    makeRenamedPressCase(KEYPAD_NINE, KP_9);
                    makeRenamedPressCase(KEYPAD_PERIOD, KP_DECIMAL);
                    makeRenamedPressCase(KEYPAD_DIVIDE, KP_DIVIDE);
                    makeRenamedPressCase(KEYPAD_MULTIPLY, KP_MULTIPLY);
                    makeRenamedPressCase(KEYPAD_SUBTRACT, KP_SUBTRACT);
                    makeRenamedPressCase(KEYPAD_ADD, KP_ADD);
                    makeRenamedPressCase(KEYPAD_ENTER, KP_ENTER);
                    makeRenamedPressCase(KEYPAD_EQUAL, KP_EQUAL);
                    makePressCase(LEFT_SHIFT);
                    makePressCase(LEFT_CONTROL);
                    makePressCase(LEFT_ALT);
                    makeRenamedPressCase(LEFT_META, LEFT_SUPER);
                    makePressCase(RIGHT_SHIFT);
                    makePressCase(RIGHT_CONTROL);
                    makePressCase(RIGHT_ALT);
                    makeRenamedPressCase(RIGHT_META, RIGHT_SUPER);
                    makePressCase(MENU);
                }
            } else if (action == GLFW_RELEASE) {
                switch (key) {
                    makeReleaseCase(SPACE);
                    makeReleaseCase(APOSTROPHE);
                    makeReleaseCase(COMMA);
                    makeReleaseCase(MINUS);
                    makeReleaseCase(PERIOD);
                    makeRenamedReleaseCase(FORWARDSLASH, SLASH);
                    makeRenamedReleaseCase(ZERO, 0);
                    makeRenamedReleaseCase(ONE, 1);
                    makeRenamedReleaseCase(TWO, 2);
                    makeRenamedReleaseCase(THREE, 3);
                    makeRenamedReleaseCase(FOUR, 4);
                    makeRenamedReleaseCase(FIVE, 5);
                    makeRenamedReleaseCase(SIX, 6);
                    makeRenamedReleaseCase(SEVEN, 7);
                    makeRenamedReleaseCase(EIGHT, 8);
                    makeRenamedReleaseCase(NINE, 9);
                    makeReleaseCase(SEMICOLON);
                    makeReleaseCase(EQUAL);
                    makeReleaseCase(A);
                    makeReleaseCase(B);
                    makeReleaseCase(C);
                    makeReleaseCase(D);
                    makeReleaseCase(E);
                    makeReleaseCase(F);
                    makeReleaseCase(G);
                    makeReleaseCase(H);
                    makeReleaseCase(I);
                    makeReleaseCase(J);
                    makeReleaseCase(K);
                    makeReleaseCase(L);
                    makeReleaseCase(M);
                    makeReleaseCase(N);
                    makeReleaseCase(O);
                    makeReleaseCase(P);
                    makeReleaseCase(Q);
                    makeReleaseCase(R);
                    makeReleaseCase(S);
                    makeReleaseCase(T);
                    makeReleaseCase(U);
                    makeReleaseCase(V);
                    makeReleaseCase(W);
                    makeReleaseCase(X);
                    makeReleaseCase(Y);
                    makeReleaseCase(Z);
                    makeRenamedReleaseCase(OPEN_BRACKET, LEFT_BRACKET);
                    makeReleaseCase(BACKSLASH);
                    makeRenamedReleaseCase(CLOSE_BRACKET, RIGHT_BRACKET);
                    makeRenamedReleaseCase(TILDE, GRAVE_ACCENT);
                    makeReleaseCase(WORLD_1);
                    makeReleaseCase(WORLD_2);
                    makeReleaseCase(ESCAPE);
                    makeReleaseCase(ENTER);
                    makeReleaseCase(TAB);
                    makeReleaseCase(BACKSPACE);
                    makeReleaseCase(INSERT);
                    makeReleaseCase(DELETE);
                    makeRenamedReleaseCase(RIGHT_ARROW, RIGHT);
                    makeRenamedReleaseCase(LEFT_ARROW, LEFT);
                    makeRenamedReleaseCase(DOWN_ARROW, DOWN);
                    makeRenamedReleaseCase(UP_ARROW, UP);
                    makeReleaseCase(PAGE_UP);
                    makeReleaseCase(PAGE_DOWN);
                    makeReleaseCase(HOME);
                    makeReleaseCase(END);
                    makeReleaseCase(CAPS_LOCK);
                    makeReleaseCase(SCROLL_LOCK);
                    makeReleaseCase(NUM_LOCK);
                    makeReleaseCase(PRINT_SCREEN);
                    makeReleaseCase(PAUSE);
                    makeReleaseCase(F1);
                    makeReleaseCase(F2);
                    makeReleaseCase(F3);
                    makeReleaseCase(F4);
                    makeReleaseCase(F5);
                    makeReleaseCase(F6);
                    makeReleaseCase(F7);
                    makeReleaseCase(F8);
                    makeReleaseCase(F9);
                    makeReleaseCase(F10);
                    makeReleaseCase(F11);
                    makeReleaseCase(F12);
                    makeReleaseCase(F13);
                    makeReleaseCase(F14);
                    makeReleaseCase(F15);
                    makeReleaseCase(F16);
                    makeReleaseCase(F17);
                    makeReleaseCase(F18);
                    makeReleaseCase(F19);
                    makeReleaseCase(F20);
                    makeReleaseCase(F21);
                    makeReleaseCase(F22);
                    makeReleaseCase(F23);
                    makeReleaseCase(F24);
                    makeReleaseCase(F25);
                    makeRenamedReleaseCase(KEYPAD_ZERO, KP_0);
                    makeRenamedReleaseCase(KEYPAD_ONE, KP_1);
                    makeRenamedReleaseCase(KEYPAD_TWO, KP_2);
                    makeRenamedReleaseCase(KEYPAD_THREE, KP_3);
                    makeRenamedReleaseCase(KEYPAD_FOUR, KP_4);
                    makeRenamedReleaseCase(KEYPAD_FIVE, KP_5);
                    makeRenamedReleaseCase(KEYPAD_SIX, KP_6);
                    makeRenamedReleaseCase(KEYPAD_SEVEN, KP_7);
                    makeRenamedReleaseCase(KEYPAD_EIGHT, KP_8);
                    makeRenamedReleaseCase(KEYPAD_NINE, KP_9);
                    makeRenamedReleaseCase(KEYPAD_PERIOD, KP_DECIMAL);
                    makeRenamedReleaseCase(KEYPAD_DIVIDE, KP_DIVIDE);
                    makeRenamedReleaseCase(KEYPAD_MULTIPLY, KP_MULTIPLY);
                    makeRenamedReleaseCase(KEYPAD_SUBTRACT, KP_SUBTRACT);
                    makeRenamedReleaseCase(KEYPAD_ADD, KP_ADD);
                    makeRenamedReleaseCase(KEYPAD_ENTER, KP_ENTER);
                    makeRenamedReleaseCase(KEYPAD_EQUAL, KP_EQUAL);
                    makeReleaseCase(LEFT_SHIFT);
                    makeReleaseCase(LEFT_CONTROL);
                    makeReleaseCase(LEFT_ALT);
                    makeRenamedReleaseCase(LEFT_META, LEFT_SUPER);
                    makeReleaseCase(RIGHT_SHIFT);
                    makeReleaseCase(RIGHT_CONTROL);
                    makeReleaseCase(RIGHT_ALT);
                    makeRenamedReleaseCase(RIGHT_META, RIGHT_SUPER);
                    makeReleaseCase(MENU);
                }
            }
        };

        inline void GLFWMousePositionCallback(GLFWwindow * window, double xpos, double ypos)
        {
            glfwMouse.setX(xpos);
            glfwMouse.setY(ypos);
        };

        inline void GLFWMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
        {
            if (action == GLFW_PRESS) {
                switch (button) {
                    makeRenamedPressCaseMouse(LEFT, LEFT);
                    makeRenamedPressCaseMouse(RIGHT, RIGHT);
                    makeRenamedPressCaseMouse(MIDDLE, MIDDLE);
                    makeRenamedPressCaseMouse(FOUR, 4);
                    makeRenamedPressCaseMouse(FIVE, 5);
                    makeRenamedPressCaseMouse(SIX, 6);
                    makeRenamedPressCaseMouse(SEVEN, 7);
                    makeRenamedPressCaseMouse(EIGHT, 8);
                }
            } else {
                switch (button) {
                    makeRenamedReleaseCaseMouse(LEFT, LEFT);
                    makeRenamedReleaseCaseMouse(RIGHT, RIGHT);
                    makeRenamedReleaseCaseMouse(MIDDLE, MIDDLE);
                    makeRenamedReleaseCaseMouse(FOUR, 4);
                    makeRenamedReleaseCaseMouse(FIVE, 5);
                    makeRenamedReleaseCaseMouse(SIX, 6);
                    makeRenamedReleaseCaseMouse(SEVEN, 7);
                    makeRenamedReleaseCaseMouse(EIGHT, 8);
                }
            }
        }

        inline void error_callback(int error, const char * description)
        {
            fprintf(stderr, "Error: %s\n", description);
        }


        class GLFWBoss : public hive::interface::InterfaceBoss
        {
          public:
            static const char MAX_AVAILABLE_CONTROLLERS = 16;

          private:
            GLFWwindow * window = nullptr;

            Controller controllers[MAX_AVAILABLE_CONTROLLERS];

          private:
            void pollControllers()
            {
                auto root = GLFW_JOYSTICK_1;

                for (int i = 0; i < MAX_AVAILABLE_CONTROLLERS; i++) {

                    const auto id = i + root;

                    int count;
                    auto & controller = controllers[i];

                    if (glfwJoystickPresent(id)) {
                        controller.setAliveStatus(true);
                        controller.setAxisArrayPointer(glfwGetJoystickAxes(i, &count), count);
                        controller.setButtonArrayPointer(glfwGetJoystickButtons(i, &count), count);

                    } else if (!glfwJoystickPresent(id) && controller.isALIVE()) {
                        controller.setAliveStatus(false);
                        controller.setAxisArrayPointer(nullptr, 0);
                        controller.setButtonArrayPointer(nullptr, 0);
                    }
                }
            }

          public:
            GLFWBoss(unsigned int w = 1024, unsigned int h = 720) : InterfaceBoss(w, h)
            {
                std::cout << "GLFW ## Bosses length " << Boss::bosses.size() << std::endl;
            };

            virtual ~GLFWBoss() { teardown(); };

          protected:
            virtual int priority() { return 0; }


            virtual void teardown()
            {
                if (window != nullptr) {
                    glfwDestroyWindow(window);

#ifdef HIVE_USE_OPENGL
                    glfwTerminate();
#endif
#ifdef HIVE_USE_VULKAN
                    vulkanDestroy();
#endif
                    window = nullptr;
                }
            }

          protected:
            virtual void setup()
            {

                glfwSetErrorCallback(error_callback);

                if (glfwInit() == GLFW_TRUE) {
                    std::cout << "Pending Created GL Context" << std::endl;

#ifdef HIVE_USE_VULKAN
                    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif


#ifdef HIVE_USE_OPENGL
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//############################## DEBUG
#ifdef HIVE_DEBUG

                    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
//############################## END DEBUG
#else
                    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
#endif

                    window = glfwCreateWindow(width, height, "HIVE RELEASE", NULL, NULL);

                    if (window == nullptr) {
                        glfwTerminate();
                        fatalError();
                        return;
                    }

                    /*make this the current context */
                    glfwMakeContextCurrent(window);

                    // Initialize glew
                    std::cout << "Intializing GLEW" << std::endl;

                    GLenum err = glewInit();

                    if (GLEW_OK != err) {
                        /* Problem: glewInit failed, something is seriously wrong. */
                        std::cout << glewGetErrorString(err) << std::endl;
                        glfwTerminate();
                        fatalError();
                        return;
                    }

                    std::cout << "Created GL Context" << std::endl;
#endif


#ifdef HIVE_USE_VULKAN

                    auto extensionFunction = [&](unsigned * extension_count,
                                                 const char *** glfw_extensions) {
                        (*glfw_extensions) = glfwGetRequiredInstanceExtensions(extension_count);
                    };

                    auto surfaceFunction = [&](VkInstance instance,
                                               VkSurfaceKHR * vksurface) -> bool {
                        if (glfwCreateWindowSurface(instance, window, nullptr, vksurface) !=
                            VK_SUCCESS) {
                            __ERROR("Failed to create VK Surface");
                            return false;
                        }

                        return true;
                    };

                    initVulkanInstance(extensionFunction);

                    initVulkanSurface(surfaceFunction);

                    initVulkanDevice();
#endif

                    // Setup inputs
                    glfwSetKeyCallback(window, GLFWKeyboardCallback);

                    glfwSetCursorPosCallback(window, GLFWMousePositionCallback);

                    glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);

                    // Get all connected controllers
                    pollControllers();

                    return;
                }

                fatalError();

                return;
            };


            virtual void update(float delta_t)
            {
                if (!glfwWindowShouldClose(window)) {

                    glfwSwapBuffers(window);

                    /* Get keyboard events and update the keyboard object state */
                    glfwPollEvents();

                    // update controllers
                    pollControllers();
                } else {
                    prepareExit();
                }
            };

            virtual bool resize(unsigned int w = 0, unsigned int h = 0,
                                hive::interface::SCREEN_MODE mode = hive::interface::WINDOWED)
            {
                return true;
            };

          public:
            virtual hive::interface::keyboard::Keyboard::KeyStateReader & getKeyboard()
            {
                return glfwKeyboard.getKeyStateReader();
            }

            virtual hive::interface::controller::Controller::ControllerStateReader &
            getController(unsigned index)
            {
                return controllers[index % MAX_AVAILABLE_CONTROLLERS].getControllerStateReader();
            }

            virtual hive::interface::mouse::Mouse::MouseStateReader & getMouse()
            {
                return glfwMouse.getMouseStateReader();
            };
        }; // namespace interface
    }      // namespace interface
} // namespace hive

//#endif