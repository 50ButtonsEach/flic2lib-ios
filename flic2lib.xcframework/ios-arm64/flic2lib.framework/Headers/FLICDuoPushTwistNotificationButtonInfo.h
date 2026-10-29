#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// The state of one of the two buttons in a Duo push-twist notification.
@interface FLICDuoPushTwistNotificationButtonInfo : NSObject

/// Whether this button is currently held down.
@property(nonatomic, readonly, getter=isPressed) BOOL pressed;
/// Whether this is the first push-twist notification since this button was pressed.
@property(nonatomic, readonly, getter=isFirstEvent) BOOL firstEvent;
/// Whether this button has been held down for at least half a second.
@property(nonatomic, readonly, getter=isPressedForAtLeastHalfASecond) BOOL pressedForAtLeastHalfASecond;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
