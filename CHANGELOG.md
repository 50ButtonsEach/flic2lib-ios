![Flic Logo Black](https://user-images.githubusercontent.com/2717016/70526105-1bbaa200-1b49-11ea-9aa0-49e7959300c3.png)

# Changelog

This file documents the changes between different build versions of the `flic2lib.framework`

## flic2lib-ios 1.2.0

### Changes

* The XCFramework bundle now adds an empty Mac Catalyst implementation for both Intel and Apple Silicon. This means that while Flic 2 functionality is still not supported on Mac, you will now at least be able to compile and run your project with Mac Catalyst.

* The build for iOS Simulator now includes slices for arm64 (Apple Silicon). Slices for i386 (32 bit simulator) have been removed.

* Frameworks up to date with with iphoneos14.5 sdk.

## flic2lib-ios 1.1.0

### Significant Changes

* The framework is now distributed using the new XCFramework packaging format. This means that instead of a single `flic2lib.framework` file, you will need to use a `flic2lib.xcframework` bundle. This bundle is essentially a folder containing framework builds for all supported architectures. The main benefit of this is that the framework will from now on run in the iOS Simulator, by default, with no configuration needed.

	This change switch was necessary due to the fact that Xcode 11.4 now throws a compiler error if any embedded framework is not build for Simulator. This would only generate a warning in previous versions. Thus the previously provided simulator files would no longer work without the use of cumbersome build scripts.
	
	Xcode 11 is required in order to use XCFramework bundles.

### Minor Changes

* New property, `latencyMode`, added. This property can be set using the new `FLICLatencyMode` enums. The purpose of this is to allow you to reduce the click latency on events that occur while the Flic is connected. This may be useful if you are developing a foreground application, such as a game, where a lower latency is needed. However, keep in mind that this will affect expected battery life.

## flic2lib-ios 1.0.5

### Significant Changes

* Adjustments made to the framework in order to support Core Bluetooth API changes indtroduced in iOS 13.4. Older versions of this framework will still work on iOS 13.4, but we do recommend updating. If you do not update the framework then there is a risk that the button connection will not be re-set properly if connection is lost during the Bluetooth LE encryption exchange/setup process. This is particularily important for applications that uses long-term execution in the background.

### Minor Changes

* Added human readable descriptions to all flic2lib error codes of the `FLICErrorDomain` and `FLICButtonScannerErrorDomain` error domains. This would be the enums `FLICButtonScannerErrorCode` and `FLICError`. The description can be read using the `NSLocalizedDescriptionKey` key of the error’s userInfo dictionary, which may be helpful for troubleshooting.

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