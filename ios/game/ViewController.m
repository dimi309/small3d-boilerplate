//
//  ViewController.m
//  game
//
//  Created by me on 07/01/2022.
//

#import "ViewController.h"
#include "interop.h"
#include "Game.hpp"

@implementation ViewController {
  CADisplayLink* _displayLink;
}

- (void)viewDidLoad {
  [super viewDidLoad];
    
    
  // Do any additional setup after loading the view.
  app_window = (__bridge void*) self.view.layer;
  
    
  small3d::initLogger();
    
    
  try {
    
    GameLoopAndInput::init(process, render, initGraphics, pauseGraphics, resumeGraphics, getScreenInfo);
    
    init();
    initGraphics();
        
    GameLoopAndInput::instantiated = true;
        
  }
  catch (std::exception& e) {
    LOGERROR(std::string(e.what()));
  }
    
  GameLoopAndInput::get_screen_info();
  
  _displayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(renderLoop)];
  _displayLink.preferredFramesPerSecond = 60;
  //[_displayLink preferredFramesPerSecond: 60 ];
  [_displayLink addToRunLoop: NSRunLoop.currentRunLoop forMode: NSDefaultRunLoopMode];
    
}

- (void) viewDidAppear:(BOOL)animated {
    
}

- (void) viewWillDisappear:(BOOL)animated {
    
}

-(void) renderLoop {
  if (GameLoopAndInput::appActive) {
    process(GameLoopAndInput::keyInput);
    render();
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

@implementation View
+(Class) layerClass { return [CAMetalLayer class]; }
@end




