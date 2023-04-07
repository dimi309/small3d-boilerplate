//
//  ViewController.m
//
//  Created by me on 07/01/2022.
//

#import "ViewController.h"
#include "interop.h"
#include <small3d/Renderer.hpp>
#include "Game.hpp"
#include <small3d/Time.hpp>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

double startSeconds = getTimeInSeconds();
double seconds = getTimeInSeconds();
double prevSeconds = seconds;
const uint32_t framerate = 60;
constexpr double secondsInterval = 1.0 / framerate;

void dummy() {
  
}

@implementation ViewController {
  CADisplayLink* _displayLink;
}

- (void)viewDidLoad {
  [super viewDidLoad];
  
  small3d::initLogger();
  
  self.app_eaglLayer = (CAEAGLLayer*) self.view.layer;
  
  self.app_eaglLayer.opaque = YES;
  self.app_eaglLayer.drawableProperties =
  [NSDictionary dictionaryWithObjectsAndKeys:
                                      [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
  
  self.app_view = [GLKView alloc];//(GLKView *)self.view;
  self.app_view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  
  if (!self.app_view.context) {
    LOGERROR("Failed to create OpenGL ES context.");
  }
  if(![EAGLContext setCurrentContext:self.app_view.context]) {
    LOGERROR("Failed to set current OpenGL ES context.");
  }
  
  GLuint tmp = 0;
  
  glGenFramebuffers(1, &tmp);
  self.app_framebuffer = tmp;
  glBindFramebuffer(GL_FRAMEBUFFER, self.app_framebuffer);
  
  tmp = 0;
  glGenRenderbuffers(1, &tmp);
  self.app_colour_render_buffer = tmp;
  glBindRenderbuffer(GL_RENDERBUFFER, self.app_colour_render_buffer);
  
  [[self.app_view context] renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.app_eaglLayer];
  
  GLint framebufferWidth = 0, framebufferHeight = 0;
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, self.app_colour_render_buffer);
  
  tmp = 0;
  glGenRenderbuffers(1, &tmp);
  self.app_depth_buffer = tmp;
  glBindRenderbuffer(GL_RENDERBUFFER, self.app_depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, self.app_depth_buffer);
  
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
          LOGERROR("Failed to make complete framebuffer object");
  }
  else {
    LOGDEBUG("Framebuffer object is complete.");
  }

  try {
    
    GameLoopAndInput::init(process, render, dummy, pauseGraphics, resumeGraphics, getScreenInfo);
    
    init();
    initGraphics(framebufferWidth, framebufferHeight, self.app_framebuffer, self.app_colour_render_buffer);
    
    GameLoopAndInput::instantiated = true;
    GameLoopAndInput::appActive = true;
    
  }
  catch (std::exception& e) {
    LOGERROR(std::string(e.what()));
  }
  
  GameLoopAndInput::screenWidth = get_app_width();
  GameLoopAndInput::screenHeight = get_app_height();
    
  _displayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(renderLoop)];
  // This does not work on old iOSes
  // _displayLink.preferredFramesPerSecond = 60;
  [_displayLink addToRunLoop: NSRunLoop.currentRunLoop forMode: NSDefaultRunLoopMode];
    
}

- (void) dealloc {
  GLuint tmp = 0;
  
  if (self.app_framebuffer) {
    tmp = self.app_framebuffer;
    glDeleteFramebuffers(1, &tmp);
    self.app_framebuffer = 0;
  }
  
  if (self.app_depth_buffer) {
    tmp = self.app_depth_buffer;
    glDeleteRenderbuffers(1, &tmp);
    self.app_depth_buffer = 0;
  }
  
  if (self.app_colour_render_buffer) {
    tmp = self.app_colour_render_buffer;
    glDeleteRenderbuffers(1, &tmp);
    self.app_colour_render_buffer = 0;
  }
  
  if ([EAGLContext currentContext] == [self.app_view context]) {
    [EAGLContext setCurrentContext:nil];
  }
}

- (void) viewDidAppear:(BOOL)animated {
    
}

- (void) viewWillDisappear:(BOOL)animated {
    
}

-(void) renderLoop {
  
  seconds = getTimeInSeconds();
  
  if (GameLoopAndInput::appActive && (seconds - prevSeconds > secondsInterval)) {
    prevSeconds = seconds;
    glBindFramebuffer(GL_FRAMEBUFFER, self.app_framebuffer);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    process(GameLoopAndInput::keyInput);
    render();
    
    const GLenum discards[]  = {GL_DEPTH_ATTACHMENT};
    glDiscardFramebufferEXT(GL_FRAMEBUFFER,1,discards);
    glBindRenderbuffer(GL_RENDERBUFFER, self.app_colour_render_buffer);
    if(![[self.app_view context] presentRenderbuffer:GL_RENDERBUFFER]) {
      LOGERROR("Could not present render buffer!");
    }
  }
}

void processPhase(UITouchPhase phase, CGPoint touchPoint) {
  
  switch (phase){
      case UITouchPhaseBegan:
        GameLoopAndInput::handle_input(touchPoint.x, touchPoint.y, true, false);
        break;
      case UITouchPhaseMoved:
        GameLoopAndInput::handle_input(touchPoint.x, touchPoint.y, true, true);
        break;
      case UITouchPhaseEnded:
        GameLoopAndInput::keyInput.enter = 0;
        GameLoopAndInput::handle_input(touchPoint.x, touchPoint.y, false, false);
        break;
      default:
        break;
    }
  
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
  for (UITouch *touch in [event allTouches]) {
    CGPoint touchPoint = [touch locationInView:self.view];
    
    processPhase([touch phase], touchPoint);
  }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
  for (UITouch *touch in [event allTouches]) {
    CGPoint touchPoint = [touch locationInView:self.view];
    
    processPhase([touch phase], touchPoint);
  }
  
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
  for (UITouch *touch in [event allTouches]) {
    CGPoint touchPoint = [touch locationInView:self.view];
    
    processPhase([touch phase], touchPoint);
  }
}


@end




