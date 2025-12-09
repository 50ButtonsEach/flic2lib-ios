//
//  FLICButtonEvent.h
//  fliclib
//
//  Created by Oskar Öberg on 2025-06-09.
//  Copyright © 2025 Shortcut Labs. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, FLICButtonEventClass) {
	FLICButtonEventClassUpOrDown,
	FLICButtonEventClassClickOrHold,
	FLICButtonEventClassSingleOrDoubleClick,
	FLICButtonEventClassSingleOrDoubleClickOrHold
};

typedef NS_ENUM(NSUInteger, FLICButtonEventType) {
	FLICButtonEventTypeUp,
	FLICButtonEventTypeDown,
	FLICButtonEventTypeClick,
	FLICButtonEventTypeSingleClick,
	FLICButtonEventTypeDoubleClick,
	FLICButtonEventTypeHold
};

@interface FLICButtonEvent : NSObject

@property (nonatomic, assign) FLICButtonEventClass eventClass;
@property (nonatomic, assign) FLICButtonEventType type;

/**
 * An event counter that starts at zero when the Flic 2 boots and always increases.
 *
 * This value divided by four indicates roughly how many times the button has been pressed and released.
 *
 * More specific, event_count mod 4 should be 1: down, 2: hold, 3: up, 0: single click timeout.
 */
@property (nonatomic, assign) uint32_t eventCount;

/**
 * Which physical button was pressed.
 *
 * For Flic duo, it can be 0 or 1. For Flic 2, always 0.
 */
@property (nonatomic, assign) uint8_t buttonNumber;

/**
 * Indicates if this button event was queued, i.e. it was pressed some time ago before connection setup completed.
 */
@property (nonatomic, assign) BOOL wasQueued;

/**
 * If this event was queued, this value contains the age of the event, in seconds. If this event was not queued, the value will be zero.
 */
@property (nonatomic, assign) double age;

/**
 * Accelerometer values.
 */
@property (nonatomic, assign) int8_t x;
@property (nonatomic, assign) int8_t y;
@property (nonatomic, assign) int8_t z;

/**
 * Gesture, -1 if no gesture.
 */
@property (nonatomic, assign) int gesture;

/**
 * Button was held down at least 0.5 seconds.
 */
@property (nonatomic, assign) BOOL downAtLeastHalfASecond;

- (instancetype)initWithEventClass:(FLICButtonEventClass)eventClass eventType:(FLICButtonEventType)eventType eventCount:(uint32_t)eventCount buttonNumber:(uint32_t)buttonNumber wasQueued:(BOOL)wasQueued age:(double)age x:(int8_t)x y:(int8_t)y z:(int8_t)z gesture:(int)gesture downAtLeastHalfASecond:(BOOL)downAtLeastHalfASecond;

- (BOOL)isOnWall;
- (BOOL)isButtonDown;
- (BOOL)isButtonDownOnButtonNumber:(int)buttonNumber;

@end



/**
 * Button event class.
 *
 * <p>Each time the button is interacted with, one or more events will be sent.</p>
 *
 * <p>Usually an application only needs to listen to one event class.</p>
 *
 * <p>Since distinguishing between single and double click needs some waiting time after the first
 * click to detect if a second press will occur or not, single click events will be delayed for the
 * last two event classes but not for the first two, it is important to pick the right event class
 * for the use case.</p>
 *
 * <p>For a particular event class, only the specified button event types may be emitted.</p>
 */
enum Flic2EventButtonEventClass {
    /**
     * Up or down.
     *
     * <p>Triggered on every button down or release.</p>
     */
    FLIC2_EVENT_BUTTON_EVENT_CLASS_UP_OR_DOWN,
    
    /**
     * Click or hold.
     *
     * <p>Used if you want to distinguish between click and hold.</p>
     */
    FLIC2_EVENT_BUTTON_EVENT_CLASS_CLICK_OR_HOLD,
    
    /**
     * Single or double click.
     *
     * <p>Used if you want to distinguish between a single click and a double click.</p>
     */
    FLIC2_EVENT_BUTTON_EVENT_CLASS_SINGLE_OR_DOUBLE_CLICK,
    
    /**
     * Single or double click or hold.
     *
     * <p>Used if you want to distinguish between a single click, a double click and a hold.</p>
     */
    FLIC2_EVENT_BUTTON_EVENT_CLASS_SINGLE_OR_DOUBLE_CLICK_OR_HOLD
};

/**
 * Button event type.
 */
enum Flic2EventButtonEventType {
    /**
     * The button was pressed.
     */
    FLIC2_EVENT_BUTTON_EVENT_TYPE_UP,
    
    /**
     * The button was released.
     */
    FLIC2_EVENT_BUTTON_EVENT_TYPE_DOWN,
    
    /**
     * The button was clicked, and was held for at most 1 seconds between press and release.
     */
    FLIC2_EVENT_BUTTON_EVENT_TYPE_CLICK,
    
    /**
     * The button was clicked once.
     */
    FLIC2_EVENT_BUTTON_EVENT_TYPE_SINGLE_CLICK,
    
    /**
     * The button was clicked twice. The time between the first and second press must be at most 0.5 seconds.
     */
    FLIC2_EVENT_BUTTON_EVENT_TYPE_DOUBLE_CLICK,
    
    /**
     * The button was held for at least 1 second.
     */
    FLIC2_EVENT_BUTTON_EVENT_TYPE_HOLD
};

/**
 * Flic 2 event - button event.
 */
struct Flic2EventButtonEvent {
    /**
     * The button event class for this event.
     */
    enum Flic2EventButtonEventClass event_class;
    
    /**
     * The button event type for this event.
     */
    enum Flic2EventButtonEventType event_type;
    
    /**
     * An event counter that starts at zero when the Flic 2 boots and always increases.
     *
     * <p>This value divided by four indicates roughly how many times the button has been pressed and released.</p>
     *
     * <p>More specific, event_count mod 4 should be 1: down, 2: hold, 3: up, 0: single click timeout.</p>
     */
    uint32_t event_count;

    /**
     * Which physical button was pressed.
     *
     * <p>For Flic duo, it can be 0 or 1. For Flic 2, always 0.</p>
     */
    uint8_t button_number;
    
    /**
     * Indicates if this button event was queued, i.e. it was pressed some time ago before connection setup completed.
     */
    bool was_queued;
    
    /**
     * If this event was queued, this value contains the age of the event, in seconds. If this event was not queued, the value will be zero.
     */
    double age;

    /**
     * Accelerometer values.
     */
    int8_t x, y, z;

    /**
     * Gesture recognition.
     */
    int gesture;

    /**
     * Button was held down at least 0.5 seconds.
     */
    bool down_at_least_half_a_second;
};
