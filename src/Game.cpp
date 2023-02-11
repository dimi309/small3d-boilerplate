/*
 * Game.cpp
 *
 *  Created on: 10 May 2022
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include "Game.hpp"
#include "Global.hpp"
#include <small3d/Model.hpp>
#include <small3d/SceneObject.hpp>
#include <small3d/Sound.hpp>
#include <exception>
#include <string>
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include <small3d/Time.hpp>

#if defined(__ANDROID__) || defined(SMALL3D_IOS)

KeyInput latestInput;

void renderControls();

struct android_app *androidGameState;
#endif

void render();

void renderEnv();

void write(std::string text, float elevation = 0.0f);

void initGame(uint32_t level);

small3d::Renderer *r;

#if defined(__ANDROID__) || defined(SMALL3D_IOS)
small3d::Model button;
#endif

void process(KeyInput &keyInput) {

#if defined(__ANDROID__) || defined(SMALL3D_IOS)
  latestInput = keyInput;
#endif

}

void render() {

  // Rendering transparent surfaces like the sea last ensures that transparency works on
  // some platforms, like MacOS.

#if defined(__ANDROID__) || defined(SMALL3D_IOS)
  renderControls();
#endif

  r->swapBuffers();
}

#if defined(__ANDROID__) || defined(SMALL3D_IOS)

void renderControls() {
  // left
  r->render(button, glm::vec3(-0.8f, -0.8f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f),
            latestInput.left > 0 ? "buttonLeftOn" : "buttonLeftOff", false);
  // down
  r->render(button, glm::vec3(-0.6f, -0.8f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f),
            latestInput.down > 0 ? "buttonDownOn" : "buttonDownOff", false);
  // right
  r->render(button, glm::vec3(-0.4f, -0.8f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f),
            latestInput.right > 0 ? "buttonRightOn" : "buttonRightOff", false);
  // up
  r->render(button, glm::vec3(-0.6f, -0.6f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f),
            latestInput.up > 0 ? "buttonUpOn" : "buttonUpOff", false);

  r->render(button, glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec4(0.0f),
            latestInput.space > 0 ? "buttonFireOn" : "buttonFireOff", false);
}

#endif

#if defined(__ANDROID__)

void init(struct android_app *state) {
  androidGameState = state;
#else
  void init() {
#endif

}

#if defined(__ANDROID__)
void toURL(std::string url) {
  JNIEnv *env;
  androidGameState->activity->vm->AttachCurrentThread(&env, nullptr);
  jstring url_string = env->NewStringUTF(url.c_str());
  jclass uri_class = env->FindClass("android/net/Uri");
  jmethodID uri_parse = env->GetStaticMethodID(uri_class, "parse",
                                               "(Ljava/lang/String;)Landroid/net/Uri;");
  jobject uri = env->CallStaticObjectMethod(uri_class, uri_parse, url_string);
  jclass intent_class = env->FindClass("android/content/Intent");
  jfieldID action_view_id = env->GetStaticFieldID(intent_class, "ACTION_VIEW",
                                                  "Ljava/lang/String;");
  jobject action_view = env->GetStaticObjectField(intent_class, action_view_id);
  jmethodID new_intent = env->GetMethodID(intent_class, "<init>",
                                          "(Ljava/lang/String;Landroid/net/Uri;)V");
  jobject intent = env->AllocObject(intent_class);
  env->CallVoidMethod(intent, new_intent, action_view, uri);
  jclass activity_class = env->FindClass("android/app/Activity");
  jmethodID start_activity = env->GetMethodID(activity_class, "startActivity",
                                              "(Landroid/content/Intent;)V");
  env->CallVoidMethod(androidGameState->activity->clazz, start_activity, intent);
  androidGameState->activity->vm->DetachCurrentThread();
}
#endif

void loadTextures() {
#if defined(__ANDROID__) || defined(SMALL3D_IOS)
  r->generateTexture("buttonLeftOn", small3d::Image(resourceDir + "/buttonLeftOn.png"));
  r->generateTexture("buttonLeftOff", small3d::Image(resourceDir + "/buttonLeftOff.png"));
  r->generateTexture("buttonUpOn", small3d::Image(resourceDir + "/buttonUpOn.png"));
  r->generateTexture("buttonUpOff", small3d::Image(resourceDir + "/buttonUpOff.png"));
  r->generateTexture("buttonRightOn", small3d::Image(resourceDir + "/buttonRightOn.png"));
  r->generateTexture("buttonRightOff", small3d::Image(resourceDir + "/buttonRightOff.png"));
  r->generateTexture("buttonDownOn", small3d::Image(resourceDir + "/buttonDownOn.png"));
  r->generateTexture("buttonDownOff", small3d::Image(resourceDir + "/buttonDownOff.png"));
  r->generateTexture("buttonFireOn", small3d::Image(resourceDir + "/buttonFireOn.png"));
  r->generateTexture("buttonFireOff", small3d::Image(resourceDir + "/buttonFireOff.png"));
#endif
}

#if defined(SMALL3D_IOS) && defined(SMALL3D_OPENGLES)
void initGraphics(uint32_t width, uint32_t height, GLint framebuffer, GLint renderbuffer) {
#else
void initGraphics() {
#endif
  
  
#if defined(__ANDROID__) || defined(SMALL3D_IOS)

#if defined(SMALL3D_IOS) && defined(SMALL3D_OPENGLES)
  r = &small3d::Renderer::getInstance(appName, width, height, 0.785f, 1.0f, 24.0f,
                                      resourceDir + "/shaders/", 5000);
  r->origFramebuffer = framebuffer;
  r->origRenderbuffer = renderbuffer;
#else
  r = &small3d::Renderer::getInstance(appName, 854, 480, 0.785f, 1.0f, 24.0f,
                                      resourceDir + "/shaders/", 5000);
#endif
  // May need to resume playing constant sounds here
#else
#if !defined(NDEBUG) && defined(WIN32)
  r = &small3d::Renderer::getInstance(appName, 1024, 768);
#else
  r = &small3d::Renderer::getInstance(appName);
#endif
#endif

  r->setBackgroundColour(skyColour);

#if defined(__ANDROID__) || defined(SMALL3D_IOS)
  r->createRectangle(button, glm::vec3(-0.1f, 0.1f, 0.0f),
                     glm::vec3(0.1f, -0.1f, 0.0f));
#endif

  loadTextures();

}

#if defined(SMALL3D_IOS)

  
void resumeGraphics() {
  GameLoopAndInput::appActive = true;
  if (GameLoopAndInput::instantiated && GameLoopAndInput::deactivatedOnce) {
#ifndef SMALL3D_OPENGL
    r->setupVulkan();
#endif
    // Not recreating OpenGL ES because on iOS its context seems to
    // remain intact on minimised apps
  }
}

#endif
#if defined(__ANDROID__)

void resumeGraphics() {
#ifndef SMALL3D_OPENGL
  r->setupVulkan();
#else
  // The small3d Vulkan Renderer needed to keep track of created visual objects anyway
  // so it recreates them when being reinitialised. The OpenGL (ES) Renderer needs the
  // application to do that when resuming on Android, e.g. :
  /*r->start(appName, 854, 480, 0.785f, 1.0f, 24.0f,
           resourceDir + "/shaders/", 5000, 0);
  loadTextures();

  r->clearBuffers(indicator);
  r->createRectangle(indicator, glm::vec3(-0.4f, -0.4f, 0.1f),
                     glm::vec3(0.4f, -0.50f, 0.1f));
  r->clearBuffers(islet);
  r->clearBuffers(sea);
  particleEngine.clearBuffers();
  radar = std::make_unique<Radar>(r);

  r->clearBuffers(button);
  r->createRectangle(button, glm::vec3(-0.1f, 0.1f, 0.0f),
                     glm::vec3(0.1f, -0.1f, 0.0f));
  */
#endif
  r->setBackgroundColour(skyColour);
  // May need to resume playing constant sounds here too
}

#endif
  
#if defined(__ANDROID__) || defined(SMALL3D_IOS)

void pauseGraphics() {

  GameLoopAndInput::appActive = false;

#ifdef __ANDROID__
  // Only stopping sounds here on Android because iOS seems to be
  // pausing them itself correctly e.g. :
  /*sndEngine->stop();
  sndGun->stop();
  sndExplosion->stop();
  sndSplash->stop();*/
#endif

#if !defined(SMALL3D_OPENGL)
  r->destroyVulkan();
#elif defined(__ANDROID__)
  // Only stopping OpenGL ES on Android because on iOS its context seems to
  // remain intact on minimised apps
  r->stop();
#endif

#ifdef SMALL3D_IOS
  GameLoopAndInput::deactivatedOnce = true;
#endif
}

void getScreenInfo() {
  GameLoopAndInput::screenWidth = r->getScreenWidth();
  GameLoopAndInput::screenHeight = r->getScreenHeight();
}

#endif
