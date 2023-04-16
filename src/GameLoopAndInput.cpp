/*
 * GameLoopAndInput.cpp
 *
 *  Created on: 9 Oct 2021
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include "GameLoopAndInput.hpp"
#include <small3d/Logger.hpp>
#include <stdexcept>
#include <small3d/Time.hpp>

KeyInput GameLoopAndInput::input;

void (*GameLoopAndInput::process)(KeyInput& keyInput);
void (*GameLoopAndInput::render)();
bool GameLoopAndInput::stop_;

#if !defined(__ANDROID__) && !defined(SMALL3D_IOS)

GLFWwindow* GameLoopAndInput::window;

void GameLoopAndInput::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                                   int mods) {
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    input.down = true;
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    input.up = true;
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    input.left = true;
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    input.right = true;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    input.esc = true;
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    input.enter = true;
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    input.space = true;
  if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    input.lshift = true;
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    input.down = false;
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    input.up = false;
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    input.left = false;
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    input.right = false;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    input.esc = false;
  if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    input.enter = false;
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    input.space = false;
  if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    input.lshift = false;
}

void GameLoopAndInput::init(GLFWwindow* window, void (*process_function)(KeyInput&), void (*render_function)()) {
  if (!process_function) {
    throw std::runtime_error("process_function must be defined.");
  }
  if (!render_function) {
    throw std::runtime_error("render_function must be defined.");
  }
  process = process_function;
  render = render_function;
  GameLoopAndInput::window = window;
  stop_ = false;
  glfwSetKeyCallback(GameLoopAndInput::window, keyCallback);
}

void GameLoopAndInput::run(const int fps) {
  
  try {
    auto seconds = getTimeInSeconds();
    auto prevSeconds = seconds;
    auto secondsInterval = 1.0 / fps;
    auto zpos = -15.1f, zrot = 0.0f, xpos = 0.5f;
    
    while (!glfwWindowShouldClose(window) && !stop_) {
      glfwPollEvents();
      
      seconds = getTimeInSeconds();
      if (seconds - prevSeconds > secondsInterval) {
        prevSeconds = seconds;
        process(input);
        render();
      }
    }
  }
  catch (std::runtime_error& e) {
    LOGERROR(e.what());
  }
}
#else
float GameLoopAndInput::screenWidth;
float GameLoopAndInput::screenHeight;
KeyInput GameLoopAndInput::keyInput;
bool GameLoopAndInput::appActive;
bool GameLoopAndInput::instantiated;

void (*GameLoopAndInput::init_graphics)();
void (*GameLoopAndInput::pause_graphics)();
void (*GameLoopAndInput::resume_graphics)();
void (*GameLoopAndInput::get_screen_info)();

void GameLoopAndInput::interpret_input(float xval, float yval, bool keyValue, bool reset) {
  keyInput.enter = keyValue;
  if (yval > 0.4f && yval < 0.6f && xval > 0.8f) {
    keyInput.space = keyValue;
  } else if (reset && yval > 0.3f && yval < 0.7f && xval > 0.7f) {
    keyInput.space = false;
  }

  if (yval > 0.85f) {
    if (xval < 0.15f) {
      keyInput.left = keyValue;
      if (reset) {
        keyInput.right = false;
        keyInput.down = false;
        keyInput.up = false;
      }
    } else if (xval < 0.25f) {
      keyInput.down = keyValue;

      if (reset) {
        keyInput.left = false;
        keyInput.right = false;
        keyInput.up = false;
      }
    } else if (xval < 0.37f) {
      keyInput.right = keyValue;

      if (reset) {
        keyInput.left = false;
        keyInput.down = false;
        keyInput.up = false;
      }

    } else {
      keyInput.right = false;
      keyInput.left = false;
      keyInput.down = false;
      keyInput.up = false;
    }
  } else if (yval > 0.6f) {

    keyInput.left = false;
    keyInput.right = false;
    keyInput.down = false;

    if (xval > 0.15f && xval < 0.3f) {
      keyInput.up = keyValue;
    } else {
      keyInput.up = false;
    }
    if (yval < 0.75) {
      keyInput.up = false;
    }
  }
}

#if defined(SMALL3D_IOS)

void GameLoopAndInput::init(void (*process_function)(KeyInput&), void (*render_function)(),
                            void (*init_graphics_function)(),
                            void (*pause_graphics_function)(),
                            void (*resume_graphics_function)(),
                            void (*get_screen_info_function)()) {
  if (!process_function) {
    throw std::runtime_error("process_function must be defined.");
  }
  if (!render_function) {
    throw std::runtime_error("render_function must be defined.");
  }
  if (!init_graphics_function) {
    throw std::runtime_error("init_graphics_function must be defined.");
  }
  if (!pause_graphics_function) {
    throw std::runtime_error("init_graphics_function must be defined.");
  }
  if (!pause_graphics_function) {
    throw std::runtime_error("resume_graphics_function must be defined.");
  }
  if (!get_screen_info_function) {
    throw std::runtime_error("get_screen_inf_function must be defined.");
  }
  
  process = process_function;
  render = render_function;
  init_graphics = init_graphics_function;
  pause_graphics = pause_graphics_function;
  resume_graphics = resume_graphics_function;
  get_screen_info = get_screen_info_function;
}

bool GameLoopAndInput::deactivatedOnce;

void GameLoopAndInput::handle_input(int x, int y, bool isTouched, bool reset) {
  if (appActive) {
    
    int keyValue = isTouched;
    
    if (screenWidth != 0 && screenHeight != 0) {
      float xval = static_cast<float>(x) / screenWidth;
      float yval = static_cast<float>(y) / screenHeight;
      
      interpret_input(xval, yval, keyValue, reset);
    }
  }
}
#endif
#if defined(__ANDROID__)
void GameLoopAndInput::init(struct android_app *state, void (*process_function)(KeyInput&), void (*render_function)(),
                            void (*init_graphics_function)(),
                            void (*pause_graphics_function)(),
                            void (*resume_graphics_function)(),
                            void (*get_screen_info_function)()) {
  small3d_android_app = state;
  state->onAppCmd = handle_cmd;
  state->onInputEvent = handle_input;
  
  
  if (!process_function) {
    throw std::runtime_error("process_function must be defined.");
  }
  if (!render_function) {
    throw std::runtime_error("render_function must be defined.");
  }
  if (!init_graphics_function) {
    throw std::runtime_error("init_graphics_function must be defined.");
  }
  if (!pause_graphics_function) {
    throw std::runtime_error("init_graphics_function must be defined.");
  }
  if (!resume_graphics_function) {
    throw std::runtime_error("resume_graphics_function must be defined.");
  }
  if (!get_screen_info_function) {
    throw std::runtime_error("get_screen_inf_function must be defined.");
  }

  process = process_function;
  render = render_function;
  init_graphics = init_graphics_function;
  pause_graphics = pause_graphics_function;
  resume_graphics = resume_graphics_function;
  get_screen_info = get_screen_info_function;
  GameLoopAndInput::state = state;
  initVolumeControl();
}
struct android_app *GameLoopAndInput::state;
JNIEnv *GameLoopAndInput::env;
jobject GameLoopAndInput::tm;
jmethodID GameLoopAndInput::adjustVolume;
android_poll_source *GameLoopAndInput::pSource;
int GameLoopAndInput::events;

void GameLoopAndInput::initVolumeControl() {
  
  small3d_android_app->activity->vm->AttachCurrentThread(&env, NULL);
  
  jclass context = env->FindClass("android/content/Context");
  
  if (!context) {
    LOGERROR("Failed to get context class!");
  }
  
  jfieldID audioServiceField = env->GetStaticFieldID(context, "AUDIO_SERVICE",
                                                     "Ljava/lang/String;");
  
  jstring jstr = (jstring) env->GetStaticObjectField(context, audioServiceField);
  
  jmethodID getSystemServiceID = env->GetMethodID(context, "getSystemService",
                                                  "(Ljava/lang/String;)Ljava/lang/Object;");
  
  tm = env->CallObjectMethod(small3d_android_app->activity->clazz, getSystemServiceID, jstr);
  
  jclass AudioManager = env->FindClass("android/media/AudioManager");
  
  if (!AudioManager) {
    throw std::runtime_error("Could not get AudioManager.");
  }
  
  adjustVolume = env->GetMethodID(AudioManager, "adjustVolume", "(II)V");
  
  if (!adjustVolume) {
    throw std::runtime_error("Could not get adjustVolume method id.");
  }
  
}

void GameLoopAndInput::handle_cmd(android_app *pApp, int32_t cmd) {
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
    case APP_CMD_GAINED_FOCUS:
      if (!GameLoopAndInput::appActive) {
        if (!GameLoopAndInput::instantiated) {
          init_graphics();
          GameLoopAndInput::instantiated = true;
          
        } else {

          resume_graphics();

        }

        // Needed for input coordinates to be interpreted
        // correctly on Android
        get_screen_info();
        appActive = true;
      }
      break;
      
    case APP_CMD_TERM_WINDOW:
    case APP_CMD_LOST_FOCUS:
    case APP_CMD_SAVE_STATE:
    case APP_CMD_STOP:
      if (GameLoopAndInput::appActive) {
        pause_graphics();
      }
      break;
      
    default:
      LOGDEBUG("event not handled: " + std::to_string(cmd));
  }
}


int32_t GameLoopAndInput::handle_input(android_app *app, AInputEvent *event) {
  
  if (appActive) {
    bool isTouched = false;
    bool reset = false;


    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

      auto evtAction = AMotionEvent_getAction(event);

      if (evtAction == AMOTION_EVENT_ACTION_DOWN || evtAction == AMOTION_EVENT_ACTION_POINTER_DOWN
      || evtAction == 261) { // 261: Deprecated ACTION_POINTER_2_DOWN
        isTouched = true;
      }
      else if (evtAction == AMOTION_EVENT_ACTION_MOVE) {
        isTouched = true;
        reset = true;
      } else if (evtAction != AMOTION_EVENT_ACTION_UP && evtAction != AMOTION_EVENT_ACTION_POINTER_UP
      && evtAction != 262) { // 262: Deprecated ACTION_POINTER_2_UP
        return 1;
      }

      auto skipFirstPointer = evtAction == 262;

      keyInput.enter = isTouched;

      int32_t pointerCount = AMotionEvent_getPointerCount(event);
      if (screenWidth != 0 && screenHeight != 0) {
        for (uint32_t idx = 0; idx < pointerCount; ++idx) {

          if (idx == 0 && skipFirstPointer) {
            continue;
          }

          float xval = AMotionEvent_getX(event, idx) / screenWidth;
          float yval = AMotionEvent_getY(event, idx) / screenHeight;

          keyInput.lshift = (yval > 0.75f && yval < 0.95f && xval > 0.2f && xval < 0.8f);

          interpret_input(xval, yval, isTouched, reset);
        }
      }
      
    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
      switch (AKeyEvent_getKeyCode(event)) {
        case AKEYCODE_VOLUME_DOWN:
          env->CallVoidMethod(tm, adjustVolume, -1, 1);
          break;
        case AKEYCODE_VOLUME_UP:
          env->CallVoidMethod(tm, adjustVolume, 1, 1);
          break;
        case AKEYCODE_BACK:
          return 0;
          break;
        default:
          // do nothing
          break;
      }
    }
  }
  return 1;
}


void GameLoopAndInput::run(const int fps) {
  
  const uint32_t frameRate = 60;
  double seconds = getTimeInSeconds();
  double prevSeconds = seconds;
  double secondsInterval = 1.0 / frameRate;
  
  do {
    
    if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
      if (pSource != NULL) {
        pSource->process(state, pSource);
      }
    }
    
    if (GameLoopAndInput::appActive) {
      seconds = getTimeInSeconds();
      
      if (seconds - prevSeconds > secondsInterval) {
        process(GameLoopAndInput::keyInput);
        prevSeconds = seconds;
        render();
      }
    }
    
  } while (!state->destroyRequested);
  LOGDEBUG("Closing...");
}

#endif
#endif

void GameLoopAndInput::stop() {
  stop_ = true;
}
