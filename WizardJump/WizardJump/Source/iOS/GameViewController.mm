#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#import "Cube.h"
#include <GEngine.h>
#include <EntryPoint.h>
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"


@interface GameViewController () {}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController



- (void)viewDidLoad
{
    [super viewDidLoad];

    UITextField* txt = (UITextField*)[self.view viewWithTag:1];
    
    GEngine::Mobile_Input_Callback::SetShowKeyboardFunc([txt](){
        txt.becomeFirstResponder;
    });
    GEngine::Mobile_Input_Callback::SetHideKeyboardFunc([txt](){
        txt.resignFirstResponder;
    });
    
    
    
    GEngine::Mobile_Input_Callback::SetKeyboardTextFunc([txt](std::string text) {
        txt.text = [NSString stringWithUTF8String:text.c_str()];
    });
    
   GEngine::Mobile_Input_Callback::SetGetKeyboardTextFunc([txt]() {
       return std::string([txt.text UTF8String]);
    });
    
    GEngine::Mobile_Input_Callback::SetGetTimeFunc([self]() {
        return (double)self.timeSinceLastUpdate;
    });
        
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

       if (!self.context) {
           NSLog(@"Failed to create ES context");
       }
       
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    
   
    GEngine::Mobile_Input_Callback::SetGetViewContext([view](){
        return (UIView*)view;
    });
    
    GEngine::Mobile_Input_Callback::SetGetSafeArea([self](int* top, int* bottom, int* left, int* right) {
        *top = self.view.safeAreaInsets.top*self.view.contentScaleFactor;
        *bottom = self.view.safeAreaInsets.bottom*self.view.contentScaleFactor;
        *left =  self.view.safeAreaInsets.left*self.view.contentScaleFactor;
        *right = self.view.safeAreaInsets.right*self.view.contentScaleFactor;
    } );

          
    [self setupGL];
    
   
    
   
}

- (void) viewSafeAreaInsetsDidChange {
    [super viewSafeAreaInsetsDidChange ];
    printf("SAFE VIEW: %f, %f, %f, %f", self.view.safeAreaInsets.top, self.view.safeAreaInsets.bottom, self.view.safeAreaInsets.left, self.view.safeAreaInsets.right);
    App_Resize(self.view.bounds.size.width*self.view.contentScaleFactor, self.view.bounds.size.height*self.view.contentScaleFactor);
}

- (IBAction)textEntered:(UITextField *)sender forEvent:(UIEvent *)event {
    NSLog(@"TEXT CHANGED");
}

- (void)dealloc
{
    [self tearDownGL];

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (BOOL) prefersHomeIndicatorAutoHidden {
    return true;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;

        [self tearDownGL];

        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    App_Create(self.view.bounds.size.width*self.view.contentScaleFactor,
               self.view.bounds.size.height*self.view.contentScaleFactor);
    
    //Cube_setupGL(self.view.bounds.size.width, self.view.bounds.size.height);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    //App_Shutdown();
    //Cube_tearDownGL();
}
/*
- (IBAction)onTouch:(UILongPressGestureRecognizer *)sender {
    CGPoint touchPoint = [sender locationInView: self.view];
    
    
    
    if (sender.state == UIGestureRecognizerStateBegan) {
        Mobile_Input_Callback::Touched(0, touchPoint.x*self.view.contentScaleFactor,touchPoint.y*self.view.contentScaleFactor);
    } else if (sender.state == UIGestureRecognizerStateEnded) {
        Mobile_Input_Callback::Touched(2, touchPoint.x*self.view.contentScaleFactor,touchPoint.y*self.view.contentScaleFactor);
    } else if (sender.state == UIGestureRecognizerStateChanged) {
        Mobile_Input_Callback::Touched(1, touchPoint.x*self.view.contentScaleFactor,touchPoint.y*self.view.contentScaleFactor);
    }
}

 */
- (IBAction)onTextValueChange:(UITextField *)sender {
    std::string txt = std::string([sender.text UTF8String]);
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
[coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext>  _Nonnull context) {

    // change any properties on your views

} completion:^(id<UIViewControllerTransitionCoordinatorContext>  _Nonnull context) {
    UIDeviceOrientation orientation = [UIDevice currentDevice].orientation;
    App_Resize(self.view.bounds.size.width*self.view.contentScaleFactor, self.view.bounds.size.height*self.view.contentScaleFactor);
    if( UIDeviceOrientationIsPortrait(orientation) ) {
        
    } /*else {
        App_Resize(self.view.bounds.size.width, self.view.bounds.size.height);
    }
       */
}];
}
#pragma mark - GLKView and GLKViewController delegate methods


- (void)update
{
   // Cube_update(self.timeSinceLastUpdate, self.view.bounds.size.width, self.view.bounds.size.height);
    //Cube_update();
    //App_Update();
    App_Update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    //Cube_prepare();
    //Cube_draw();
    App_Draw();
    
    
}

@end

@interface TouchableView () {}

@end
@implementation TouchableView


- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    for (NSObject* obj : touches) {
        UITouch* touch = (UITouch*)obj;
        CGPoint touchPoint = [touch locationInView:touch.view];
        UITouchPhase phase = touch.phase;

        GEngine::Mobile_Input_Callback::Touched(touch.hash, 0, touchPoint.x*touch.view.contentScaleFactor,touchPoint.y*touch.view.contentScaleFactor, touch.force);
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    for (NSObject* obj : touches) {
        UITouch* touch = (UITouch*)obj;
        CGPoint touchPoint = [touch locationInView:touch.view];
        GEngine::Mobile_Input_Callback::Touched(touch.hash, 1, touchPoint.x*touch.view.contentScaleFactor,touchPoint.y*touch.view.contentScaleFactor, touch.force);
        }
    }

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
for (NSObject* obj : touches) {
    UITouch* touch = (UITouch*)obj;
    CGPoint touchPoint = [touch locationInView:touch.view];
    GEngine::Mobile_Input_Callback::Touched(touch.hash, 2, touchPoint.x*touch.view.contentScaleFactor,touchPoint.y*touch.view.contentScaleFactor, touch.force);
    }
}
   
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
for (NSObject* obj : touches) {
    UITouch* touch = (UITouch*)obj;
    CGPoint touchPoint = [touch locationInView:touch.view];
    GEngine::Mobile_Input_Callback::Touched(touch.hash, 2, touchPoint.x*touch.view.contentScaleFactor,touchPoint.y*touch.view.contentScaleFactor, touch.force);
    }
}



@end

