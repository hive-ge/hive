

#define HIVE_DEBUG
#define HIVE_USE_OPENGL
#define HIVE_USE_GLFW

#include <hive.h>

#include <nodejs/src/node_api.h>

#include <thread>
#include <vector>


#include "./interfaces.hpp"

using namespace hive::all;

BigBadBoss boss;

GLFWBoss interface_boss(512, 512);

// Sentinal against multiple setup calls if there
// are more than one NodeJS instances running.
bool INITIALIZED = false, CLOSING = false;

// Deferred object to inform NodeJS/Electron that the app should close.
napi_threadsafe_function close_function;

std::vector<napi_env> closing_environments;

void shouldClose()
{
    napi_status status;

    napi_value undefined;

    std::atomic_thread_fence(std::memory_order_release);

    if (INITIALIZED && !CLOSING) {
        CLOSING = true;
        napi_call_threadsafe_function(close_function, NULL,
                                      napi_threadsafe_function_call_mode::napi_tsfn_nonblocking);
    }
}


// Run from a detached thread.
void mainHiveThread()
{

    while (boss.update())
        ;

    shouldClose();
}


napi_value HiveInitializeFunction(napi_env env, napi_callback_info info)
{

    std::cout << "PRE SETUP ## Bosses length " << Boss::bosses.size() << std::endl;

    size_t argc = 1;
    napi_value argv[1];

    napi_status status;

    napi_value init_result;

    status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    if (status != napi_ok) napi_throw_error(env, NULL, "Failed to parse arguments");

    if (!INITIALIZED) {

        INITIALIZED = true;

        boss.setup();

        // Setup gl system


        // Setup Main Hive thread
        std::thread main_thread(mainHiveThread);

        main_thread.detach();

        status = napi_create_int32(env, 1, &init_result);

        if (status != napi_ok) napi_throw_error(env, NULL, "Unable to create return value");

    } else {
        napi_throw_error(env, NULL, "Hive has already been initialized");
    }

    return init_result;
}

void informShouldClose(napi_env env, napi_value js_callback, void * context, void * data)
{
    napi_value global, arg, ret;

    napi_value * argv = &arg;

    napi_status status = napi_get_global(env, &global);
    if (status != napi_ok) return;

    status = napi_call_function(env, global, js_callback, 0, argv, &ret);
    if (status != napi_ok) return;
}

napi_value HiveShouldClose(napi_env env, napi_callback_info info)
{

    size_t argc = 1;

    napi_value argv[1];

    napi_status status;

    napi_value init_result;

    status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    if (status != napi_ok) napi_throw_error(env, NULL, "Failed to parse arguments");

    napi_value cb_fn = argv[0], name;

    napi_create_string_utf8(env, "Hive Should Close Callback Handler", 5, &name);


    status = napi_create_threadsafe_function(env, cb_fn, NULL, name, 0, 2, NULL, NULL, NULL,
                                             informShouldClose, &close_function);

    if (status != napi_ok) napi_throw_error(env, NULL, "Failed to create threadsafe function");

    closing_environments.push_back(env);

    return NULL;
}

NAPI_MODULE_INIT()
{
    napi_status status;

    napi_value init_fn, close_event_fn;
    //*
    status = napi_create_function(env, "init", 0, HiveInitializeFunction, NULL, &init_fn);
    if (status != napi_ok) return NULL;

    status = napi_set_named_property(env, exports, "init", init_fn);
    if (status != napi_ok) return NULL;

    // Promise that is used to signal the app should be closed.
    status = napi_create_function(env, "listenForClose", 0, HiveShouldClose, NULL, &close_event_fn);
    if (status != napi_ok) return NULL;

    status = napi_set_named_property(env, exports, "addShouldCloseCB", close_event_fn);
    if (status != napi_ok) return NULL;

    //*/
    hive::js::RegisterInterfaces(env, exports);

    return exports;
}
