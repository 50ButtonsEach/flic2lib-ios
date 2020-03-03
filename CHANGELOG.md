![Flic Logo Black](https://user-images.githubusercontent.com/2717016/70526105-1bbaa200-1b49-11ea-9aa0-49e7959300c3.png)

# Changelog

This file documents the changes between different build versions of the `flic2lib.framework`

## flic2lib-ios 1.0.4

### Minor Changes

* Removed SSL pinning from the automatic firmware update feature. This is mainly to avoid any certificate issues in the future. All firmware images are signed by Shortcut Labs regardless, so SSL pinning is not necessary.

## flic2lib-ios 1.0.3

### Minor Changes

* Changed the bluetooth connection parameters in order to achieve lower click latency on events that occur while the Flic is connected. This should not affect the Flic battery life.
* Added a property `isScanning` to FLICManager.
* Add callback `button:didUpdateNickname:` to FLICButton to let the app know when the nickname has been updated.
* Add callback `button:didUpdateBatteryVoltage:` to FLICButton to let the app know when the battryVoltage has been updated.

### Bug Fixes

* Fixed a bug that could cause the `scanForButtonsWithStateChangeHandler:completion:` to end up in an infinite loop if `stopScan` was called from within the completion handler.
* Changed the way that the lib syncs the nickname between different applications.

## flic2lib-ios 1.0.2

### Major Changes

* Framework build target changed to iOS 9.0 and slices for both arm64 and armv7 are now included. This makes easier to include the framework in applications that targets both armv7 and arm64 devices.

	The framework still only support iOS 12 and up. Any version below 12, meaning iOS 9, 10 and 11, will not be able to connect Flic buttons. When you initially configure the manager you will get the `manager:didUpdateBluetoothState:` as usual, but the state will be `StateUnsupported` instead of `StatePoweredOn`. If you try to use the `scanForButtonsWithStateChangeHandler:completion:` despite this, then you will immediately get a completion with an error `FLICErrorDomain` with a code `FLICErrorUnsupportedOSVersion`.

* CBManagerState enum has been replaced with FLICManagerState enum. This means that the manager property `CBManagerState cbState` has now been replaced with `FLICManagerState state` and the manager callback `manager:didUpdateBluetoothState:` and changed name to `manager:didUpdateState:`.

### Minor Changes

* Added a missing `_Nullable` type specifier on the button parameter in the completion handler of the `scanForButtonsWithStateChangeHandler:completion:` method.

* The `uint32_t batteryLevel` property has been changed to `float batteryVoltage`, since it better represents the data. Keep in mind that you can not make a perfect conversion of battery voltage to a battery percentage, which is why we provide the voltage instead.

* Removed the codesign from the framework all together. It is not necessary to include this since the developer will re-sign it on release anyways (Embed & Sign).

* Added error codes `FLICErrorUnsupportedOSVersion` and `FLICErrorAlreadyForgotten`. 

### Bug Fixes

* When writing the nickname, the truncation to 23 bytes now properly handles all kinds of UTF8 characters, such as 2, 3 and 4 byte characters.

* Fixed a bug that could cause the `isReady` property to not update properly on re-connection.


## flic2lib-ios 1.0.1

Minor change

## flic2lib-ios 1.0.0

Initial version