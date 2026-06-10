//
//  FLICButton.h
//  flic2lib
//
//  Created by Anton Meier on 2019-04-11.
//  Copyright © 2020 Shortcut Labs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <flic2lib/FLICEnums.h>

NS_ASSUME_NONNULL_BEGIN

@class FLICButtonEvent;

typedef NS_ENUM(NSInteger, FLICButtonFallDetectionState) {
    FLICButtonFallDetectionStateTriggered,
    FLICButtonFallDetectionStatePreFallDataCollected,
    FLICButtonFallDetectionStateCompleted,
    FLICButtonFallDetectionStateDisabled,
};

typedef NS_ENUM(NSInteger, FLICButtonEnableAccelerometerStreamingResult) {
    FLICButtonEnableAccelerometerStreamingResultOK = 0,
    FLICButtonEnableAccelerometerStreamingResultInvalidConfig,
    FLICButtonEnableAccelerometerStreamingResultBusy,
    FLICButtonEnableAccelerometerStreamingResultNotReady,
    FLICButtonEnableAccelerometerStreamingResultNotSupported,
    FLICButtonEnableAccelerometerStreamingResultFirmwareUpdateNeeded,
};

typedef NS_ENUM(NSInteger, FLICButtonEnableFallDetectionResult) {
    FLICButtonEnableFallDetectionResultOK = 0,
    FLICButtonEnableFallDetectionResultInvalidConfig,
    FLICButtonEnableFallDetectionResultBusy,
    FLICButtonEnableFallDetectionResultNotReady,
    FLICButtonEnableFallDetectionResultNotSupported,
    FLICButtonEnableFallDetectionResultFirmwareUpdateNeeded,
};

typedef NS_ENUM(NSInteger, FLICButtonSetAlwaysReconnectResult) {
    FLICButtonSetAlwaysReconnectResultSuccess = 0,
    FLICButtonSetAlwaysReconnectResultNotConnected,
    FLICButtonSetAlwaysReconnectResultNotSupported,
    FLICButtonSetAlwaysReconnectResultFirmwareUpdateNeeded,
};

@interface FLICButtonAccelerometerDataPoint : NSObject

@property (nonatomic, readonly) float x;
@property (nonatomic, readonly) float y;
@property (nonatomic, readonly) float z;

@end

@interface FLICButtonAccelerometerData : NSObject

@property (nonatomic, readonly, copy) NSArray<FLICButtonAccelerometerDataPoint *> *points;

@end

@interface FLICButtonFallDetectionEvent : NSObject

@property (nonatomic, readonly) FLICButtonFallDetectionState state;
@property (nonatomic, readonly) uint16_t preFallSampleRate;
@property (nonatomic, readonly) uint16_t preFallExpectedSampleCount;
@property (nonatomic, readonly, strong) FLICButtonAccelerometerData *preFallAccelerometerData;
@property (nonatomic, readonly) uint16_t postFallSampleRate;
@property (nonatomic, readonly) uint16_t postFallExpectedSampleCount;
@property (nonatomic, readonly, strong) FLICButtonAccelerometerData *postFallAccelerometerData;

@end

@interface FLICButtonAccelerometerConfig : NSObject

@property (nonatomic, readonly) uint8_t lowPowerMode;
@property (nonatomic, readonly) uint8_t mode;
@property (nonatomic, readonly) uint8_t outputDataRate;
@property (nonatomic, readonly) uint8_t bandwidthFilter;
@property (nonatomic, readonly) uint8_t fullScaleSelection;
@property (nonatomic, readonly) uint8_t filterDatatypeSelection;
@property (nonatomic, readonly) uint8_t lowNoise;
@property (nonatomic, readonly) uint8_t highPassRefMode;
@property (nonatomic, readonly) uint8_t onlyWhilePressed;
@property (nonatomic, readonly) uint8_t samplesPerBurst;

- (instancetype)initWithLowPowerMode:(uint8_t)lowPowerMode
                                mode:(uint8_t)mode
                      outputDataRate:(uint8_t)outputDataRate
                     bandwidthFilter:(uint8_t)bandwidthFilter
                  fullScaleSelection:(uint8_t)fullScaleSelection
             filterDatatypeSelection:(uint8_t)filterDatatypeSelection
                            lowNoise:(uint8_t)lowNoise
                     highPassRefMode:(uint8_t)highPassRefMode
                    onlyWhilePressed:(uint8_t)onlyWhilePressed
                     samplesPerBurst:(uint8_t)samplesPerBurst;

@end

@interface FLICButtonBuzzerNote : NSObject

@property (nonatomic, readonly) int hz;
@property (nonatomic, readonly) float duration;

- (instancetype)initWithHz:(int)hz duration:(float)duration;

@end

@interface FLICButtonFallDetectionConfig : NSObject

/// The acceleration magnitude threshold, in mg, used to enter the low-G state.
/// The average acceleration magnitude must remain below this threshold for at least lowGDurationMs before the firmware starts listening for an impact.
@property (nonatomic, readonly) uint16_t lowGThresholdMg;

/// The minimum duration, in milliseconds, that the average acceleration magnitude must remain below lowGThresholdMg to enter the low-G state.
@property (nonatomic, readonly) uint16_t lowGDurationMs;

/// The maximum time, in milliseconds, allowed between entering the low-G state and detecting the high-G impact event.
/// If no matching high-G event is detected within this timeout, the fall detection sequence is not considered a fall.
@property (nonatomic, readonly) uint16_t highGTimeoutMs;

/// The acceleration magnitude threshold, in mg, used to detect the impact after the low-G state has been entered.
@property (nonatomic, readonly) uint16_t highGThresholdMg;

/// The high-G smoothing window, in milliseconds, used when evaluating the impact threshold.
/// The acceleration magnitude must remain at or above highGThresholdMg for this window, which filters out short spikes while still accepting impact-like thuds.
@property (nonatomic, readonly) uint16_t highGTimeWindowMs;

/// The duration, in milliseconds, that accelerometer samples are recorded after a fall has been detected.
/// When a high-G event is detected within highGTimeoutMs, the firmware sends a fall detection triggered event and continues recording for this duration before streaming the post-event data to the host.
@property (nonatomic, readonly) uint16_t postEventRecordDurationMs;

/// The accelerometer full-scale range selection used while fall detection is active.
@property (nonatomic, readonly) uint8_t fullScaleSelection;

- (instancetype)initWithLowGThresholdMg:(uint16_t)lowGThresholdMg
                         lowGDurationMs:(uint16_t)lowGDurationMs
                         highGTimeoutMs:(uint16_t)highGTimeoutMs
                       highGThresholdMg:(uint16_t)highGThresholdMg
                      highGTimeWindowMs:(uint16_t)highGTimeWindowMs
              postEventRecordDurationMs:(uint16_t)postEventRecordDurationMs
                     fullScaleSelection:(uint8_t)fullScaleSelection;

@end

@protocol FLICButtonDelegate;

/// An instance of this class represents a physical Flic.
@interface FLICButton : NSObject

/// This identifier is guaranteed to be the same for each Flic paired to a particular iOS device. Thus it can be used to identify a Flic within an app.
/// However, If you need to identify Flics cross different apps on different iOS devices, then you should have look at the either uuid, serialNumber, or bluetoothAddress.
@property(readonly, nonatomic, strong, nonnull) NSUUID *identifier;

/// The delegate that will receive events related to this particular Flic.
/// You can either set this delegate manually for each button, or let the manager do so automatically using the buttonDelegate as default.
@property(weak, nonatomic, nullable) id<FLICButtonDelegate> delegate;

/// The bluetooth advertisement name of the Flic. This will be the same name that is shown by iOS it its bluetooth settings.
@property(nonatomic, readonly, strong, nullable) NSString *name;

/// With this property you can read out the display name that the user may change in for example the Flic app. This value can also be changed from third party apps
/// integrating this framework (including your app). The purpose of this is to provide more human readable name that the user can use to identify its Flic's across apps.
/// For example "Kitchen Flic" or "Bedroom Lights". The nickname has a maximum length limit of 23 bytes. Keep in mind that this is the length in bytes, and not the
/// number of UTF8 characters (which may be up to 4 bytes long). If you write anything longer than 23 bytes then the nickname will automatically be truncated to at
/// most 23 bytes. When truncating the string, the framework will always cut between UTF8 character, so you don't have to worry about writing half an emoji, for example.
@property(nonatomic, readwrite, strong, nullable) NSString *nickname;

/// The bluetooth address of the Flic. This will be a string representation of a 49 bit long address. Example: "00:80:e4:da:12:34:56"
@property(nonatomic, readonly, strong, nonnull) NSString *bluetoothAddress;

/// This is a unique identifier string that best used to identify a Flic. This is for example used to identify Flics on all our API endpoints.
@property(nonatomic, readonly, strong, nonnull) NSString *uuid;

/// The serial number is a production identifier that is printed on the backside of the Flic inside the battery hatch.
/// This serves no other purpose than allowing a user to identify a button by manually looking at it. Can be useful in some cases.
@property(nonatomic, readonly, strong, nonnull) NSString *serialNumber;

/// Use this property to let the flic2lib know what type of click events you are interested it. By default you will get Click, Double Click and Hold events.
/// However, if you for example are only interested in Click events then you can set this property to FLICButtonTriggerModeClick. Doing so will allow the flic2lib to
/// deliver the events quicker since it can now ignore Double Click and Hold.
@property(nonatomic, readwrite) FLICButtonTriggerMode triggerMode;

/// Lets you know if the Flic is Connected, Disconnected, Connecting, or Disconnecting.
@property(nonatomic, readonly) FLICButtonState state;

/// The number of times the Flic has been clicked since last time it booted.
@property(nonatomic, readonly) uint32_t pressCount;

/// The revision of the firmware currently running on the Flic.
@property(nonatomic, readonly) uint32_t firmwareRevision;

/// When a Flic connects it will go through a quick cryptographic verification to ensure that it is both a genuine Flic and that it is the correct Flic.
/// Once this is completed this property will be set to YES and it is not until after that that you will start receiving click events (if any). As soon as the button disconnects
/// this will be set to NO again.
@property(nonatomic, readonly) BOOL isReady;

/// This will be the last know battery sample taken on the Flic. If this value is 0 then you should assume that no sample has yet been taken. It is important to know that
/// the voltage may fluctuate depending on many different factors, such as temperature and workload. For example, heavy usage of the LED will temporarily lower the voltage,
/// but it is likely to recover shortly after. Therefore we do not recomend to exactly translate this value into a battery percentage, instead consider showing a
/// "change the battery soon"-status in your app once the voltage goes below 2.65V.
@property(nonatomic, readonly) float batteryVoltage;

/// If this property is YES, then it means that this app's pairing with this specific Flic is no longer valid. This can for example occur if the Flic has been factory reset,
/// or if the maximum number of pairings have been reached. In this case you will need to delete the button from the manager and then scan for it again.
@property(nonatomic, readonly) BOOL isUnpaired;

/// Lets you switch between two different latency modes. For most use-cases it is recommend to keep the default FLICLatencyModeNormal.
/// FLICLatencyModeLow should ideally only be used for foreground applications, such as games, where low latency is needed. Keep in mind that the
/// energy consumption will be significantly higher in the low latency mode.
@property(nonatomic, readwrite) FLICLatencyMode latencyMode;

/// Attempts to connect the Flic. If the Flic is not available, due to either being out of range or not advertising, then it will be connected once it becomes
/// available as this call does not time out. This is often called a pending connection. It can be canceled by calling disconnect.
- (void)connect;

/// Disconnect a currently connected Flic or cancel a pending connection.
- (void)disconnect;

/// Playes a series of notes on Flic Duos buzzer. The maximum number of notes is 30
- (void)playBuzzerSound:(NSArray<FLICButtonBuzzerNote *> *)notes;

/// Enables accelerometer streaming.
- (void)enableAccelerometerStreamingWithConfig:(FLICButtonAccelerometerConfig *)config completionHandler:(void (^)(FLICButtonEnableAccelerometerStreamingResult result))completionHandler;

/// Disables accelerometer streaming.
- (void)disableAccelerometerStreaming;

/// Enables fall detection.
/// - Parameter alwaysReconnect: If YES, also enables always-reconnect advertising. This ensures the button will always reconnect after losing a connection. It might consume more battery if the button is often out of range, but ensures fall detection works even after the button has been disconnected by iOS. If this is set to YES, the library acts as if you would have called setAlwaysReconnect:YES just right before calling this method.
- (void)enableFallDetectionWithConfig:(FLICButtonFallDetectionConfig *)config alwaysReconnect:(BOOL)alwaysReconnect completionHandler:(void (^)(FLICButtonEnableFallDetectionResult result))completionHandler;

/// Sets whether the button should always reconnect when disconnected
/// If set to YES, the button will always try to reconnect regardless if it has anything to report. This can be useful if you want to monitor battery levels even if the button is left unused for a long time but might have negative impact on battery performance.
/// If set to NO it typically only reconnects if pressed or lost connection.
/// This setting is persisted on the device
- (void)setAlwaysReconnect:(BOOL)alwaysReconnect completionHandler:(void (^)(FLICButtonSetAlwaysReconnectResult result))completionHandler;

/// Disables fall detection.
/// - Parameter disableAlwaysReconnect: If YES, also disables always-reconnect advertising.
- (void)disableFallDetection:(BOOL)disableAlwaysReconnect;

@end

/// The delegate of a FLICButton instance must adopt the FLICButtonDelegate protocol. All calls to the delegate methods will be on the main dispatch queue.
@protocol FLICButtonDelegate <NSObject>

/// This method is called every time the Flic establishes a new bluetooth connection. Keep in mind that you also have to wait for the buttonIsReady: before
/// the Flic is ready to be used.
///
/// - Parameter button: The FLICButton instance that the event originated from.
- (void)buttonDidConnect:(FLICButton *)button;

/// This method is called after each connection once the Flic has been cryptographically verified. You will not receive any click events before this is called.
///
/// - Parameter button: The FLICButton instance that the event originated from.
- (void)buttonIsReady:(FLICButton *)button;

/// This method is called every time the bluetooth link with the Flic is lost. This can occur for several different reasons. The most common would be that
/// the iOS device and the Flic is no longer within range of each other.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter error: This error lets you know the reason for the disconnect. An error does not necessarily mean that something went wrong.
- (void)button:(FLICButton *)button didDisconnectWithError:(NSError * _Nullable)error;

/// This method is called when a connection attempt to a button fails. This indicates that something has gone wrong and that the pending connection will not be reset.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter error: This error lets you know why the connection attempt failed.
- (void)button:(FLICButton *)button didFailToConnectWithError:(NSError * _Nullable)error;

@optional

/// A receiver for all push related events. This method is required to receive Flic Duo events.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter event: Information about the event
- (void)button:(FLICButton *)button didReceiveButtonEvent:(FLICButtonEvent *)event;

/// A receiver for accelerometer streaming data.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter accelerometerData: A batch of accelerometer samples.
- (void)button:(FLICButton *)button didReceiveAccelerometerData:(FLICButtonAccelerometerData *)accelerometerData;

/// A receiver for fall detection state updates.
///
/// This callback reports every state transition in the fall detection flow.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter event: Current fall detection state and any samples collected so far.
- (void)button:(FLICButton *)button didUpdateFallDetection:(FLICButtonFallDetectionEvent *)event;

/// The Flic registered a button down event.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter queued: Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
/// - Parameter age: If the event was queued, then this will let you know the age of the event rounded to the nearest second.
- (void)button:(FLICButton *)button didReceiveButtonDown:(BOOL)queued age:(NSInteger)age;

/// The Flic registered a button up event.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter queued: Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
/// - Parameter age: If the event was queued, then this will let you know the age of the event rounded to the nearest second.
- (void)button:(FLICButton *)button didReceiveButtonUp:(BOOL)queued age:(NSInteger)age;

/// The Flic registered a button click event.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter queued: Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
/// - Parameter age: If the event was queued, then this will let you know the age of the event rounded to the nearest second.
- (void)button:(FLICButton *)button didReceiveButtonClick:(BOOL)queued age:(NSInteger)age;

/// The Flic registered a double click event.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter queued: Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
/// - Parameter age: If the event was queued, then this will let you know the age of the event rounded to the nearest second.
- (void)button:(FLICButton *)button didReceiveButtonDoubleClick:(BOOL)queued age:(NSInteger)age;

/// The Flic registered a button hold event.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter queued: Whether the event is a queued event that happened before the Flic connected or if it is a real time event.
/// - Parameter age: If the event was queued, then this will let you know the age of the event rounded to the nearest second.
- (void)button:(FLICButton *)button didReceiveButtonHold:(BOOL)queued age:(NSInteger)age;

/// The app no longer has a valid pairing with the Flic button. The isUnpaired property will now be YES and all connection
/// attempts made will immediately fail. To fix this you need to delete the button from the manager and then re-scan it again.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter error: This will always be nil at this time.
- (void)button:(FLICButton *)button didUnpairWithError:(NSError * _Nullable)error;

/// This callback will be sent once the Flic button updates its battery voltage with a new value. Typically this will occurs a few seconds
/// after the button connects. If you show a battery indicator in you app, then this would be a good place to refresh your UI. Please
/// see the description for the batteryVoltage property for more information.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter voltage: Float representation of the latest battery voltage sample.
- (void)button:(FLICButton *)button didUpdateBatteryVoltage:(float)voltage;

/// If the nickname is updated by another app (including the official Flic app), then you will get this callback letting you know that the
/// name has changed. This may either be in real time (if multiple apps are connected at the same time), or a deayed event that
/// occurs after the button connects (if the nickname was changed while your app was not active). If your app displays this nickname,
/// then this would be a good place to refresh your UI.
///
/// - Parameter button: The FLICButton instance that the event originated from.
/// - Parameter nickname: The new nickname that was sent from the Flic.
- (void)button:(FLICButton *)button didUpdateNickname:(NSString *)nickname;

@end

NS_ASSUME_NONNULL_END
