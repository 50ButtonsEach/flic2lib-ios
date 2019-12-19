//
//  FLICManager.h
//  flic2lib
//
//  Created by Anton Meier on 2019-04-11.
//  Copyright © 2019 Shortcut Labs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import "FLICButton.h"

NS_ASSUME_NONNULL_BEGIN

@protocol FLICManagerDelegate;

/*!
 *  @class FLICManager
 *
 *  @discussion     This interface is intended to be used as a singleton. It keeps track of all the buttons that have been paired with this particular app and restores them on each
 *                  application launch.
 *
 */
@interface FLICManager : NSObject

/*!
 *  @property delegate
 *
 *  @discussion     The delegate that all the FLICManagerDelegate events will be sent to. Usually this is configured only once when the application is first launched, using the
 *                  configureWithDelegate:buttonDelegate:background: method.
 *
 */
@property(weak, nonatomic, nullable) id<FLICManagerDelegate> delegate;

/*!
 *  @property buttonDelegate
 *
 *  @discussion     Once set, this delegate will automatically be set to every button instance on each application launch. This will also be assigned to new buttons that are discovered
 *                  using the scanForButtonsWithStateChangeHandler:completionHandler method. Using this delegate also ensures that the click events are delivered as fast as possible
 *                  when an application is restored in the background.
 *
 */
@property(weak, nonatomic, nullable) id<FLICButtonDelegate> buttonDelegate;

/*!
 *  @property cbState
 *
 *  @discussion     This is the manager state of Apple's CoreBluetooth framework. You will only be able to communicate with Flic buttons while the manager is in the
 *                  CBManagerStatePoweredOn state.
 *
 */
@property(readonly) CBManagerState cbState;

/*!
 *  @method sharedManager
 *
 *  @discussion     This class method return the singleton manager, assuming that it has been configured first using the configureWithDelegate:buttonDelegate:background: method first,
 *                  otherwise nil is returned.
 *
 */
+ (instancetype _Nullable)sharedManager;

/*!
 *  @method
 *
 *  @param delegate                 The delegate to be used with the manager singleton.
 *  @param buttonDelegate    The delegate to be automatically assigned to each FLICButton instance.
 *  @param background             Whether or not you intend to use this application in the background.
 *
 *  @discussion     This configuration method must be called before the manager can be used. It is recommended that this is done as soon as possible after your application has launched
 *                  in order to minimize the delay of any pending click events.
 *
 */
+ (instancetype _Nullable)configureWithDelegate:(id<FLICManagerDelegate> _Nullable)delegate
                                 buttonDelegate:(id<FLICButtonDelegate> _Nullable)buttonDelegate
                                     background:(BOOL)background;

/*!
 *  @method buttons
 *
 *  @discussion     This array will contain every button that is currently paired with this application. Each button will be added to the list after a call to
 *                  scanForButtonsWithStateChangeHandler:completionHandler: has completed without error. It is important to know that you may not try to access this list until after the
 *                  managerDidRestoreState: method has been sent to the manager delegate.
 *
 */
- (NSArray<FLICButton *> *)buttons;

/*!
 *  @method forgetButton:
 *
 *  @param button            The button that you wish to delete from the manager.
 *  @param completion   This returns the identifier of the button instance that was just removed along with an error, if needed.
 *
 *  @discussion     This will delete this button from the manager. After a successful call to this method you will no longer be able to communicate with the associated Flic button unless you
 *                  pair it again using the scanForButtonsWithStateChangeHandler:completionHandler:. On completion, the button will no longer be included in the manager's buttons array.
 *
 */
- (void)forgetButton:(FLICButton *)button completion:(void (^)(NSUUID *uuid, NSError * _Nullable error))completion;

/*!
 *  @method
 *
 *  @param stateHandler      This handler returns status events that lets you know what the scanner is currently doing. The purpose of this handler is to provide a predefined states where
 *                        you may update your application UI.
 *  @param completion           The completion handler will always run and if successful it will return a new FLICButton instance, otherwise it will provide you with an error.
 *
 *  @discussion     This method lets you add new Flic buttons to the manager. The scan flow is automated and the stateHandler will let you know what information you should provide to
 *                  your application end user.
 *
 */
- (void)scanForButtonsWithStateChangeHandler:(void (^)(FLICButtonScannerStatusEvent event))stateHandler
                                  completion:(void (^)(FLICButton *button, NSError * _Nullable error))completion;

/*!
 *  @method stopScan
 *
 *  @discussion     Cancel an ongoing button scan. This will result in a scan completion with an error.
 *
 */
- (void)stopScan;

@end

/*!
 *  @protocol FLICManagerDelegate
 *
 *  @discussion     The delegate of a FLICManager instance must adopt the FLICManagerDelegate protocol. All calls to the delegate methods will be on the main dispatch queue.
 *
 */
@protocol FLICManagerDelegate <NSObject>

/*!
 *  @method managerDidRestoreState:
 *
 *  @param manager      The manager instance that the event originates from. Since this is intended to be used as a singleton, there should only ever be one manager instance.
 *
 *  @discussion     This is called once the manager has been restored. This means that all the FLICButton instances from your previous session are restored as well. After this method
 *                  has been called you may start using the manager and communicate with the Flic buttons. This method will only be called once on each application launch.
 *
 */
- (void)managerDidRestoreState:(FLICManager *)manager;

/*!
 *  @method manager:didUpdateBluetoothState:
 *
 *  @param manager      The manager instance that the event originates from. Since this is intended to be used as a singleton, there should only ever be one manager instance.
 *  @param state           The bluetooth state of Apple's CoreBluetooth framework.
 *
 *  @discussion     Each time the bluetooth state changes on the iOS device, this method will be called. It will also be called at least one time after the manager has been configured.
 *
 */
- (void)manager:(FLICManager *)manager didUpdateBluetoothState:(CBManagerState)state;

@end

NS_ASSUME_NONNULL_END
