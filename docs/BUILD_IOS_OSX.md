# Building Mapbox GL Native for iOS

The Mapbox iOS SDK is a fully-featured package for embedding interactive map views and scalable, customizable vector maps. This document explains how to build the iOS SDK from source and integrate it into a Cocoa Touch application. It is intended for advanced developers who wish to contribute to Mapbox GL and the Mapbox iOS SDK. For information on using the Mapbox iOS SDK in production software, [visit the SDK’s official website](https://www.mapbox.com/ios-sdk/).

### Build

1. Install [appledoc](http://appledoc.gentlebytes.com/appledoc/) for API docs generation.

   ```
   curl -L -o appledoc.zip https://github.com/tomaz/appledoc/releases/download/v2.2-963/appledoc.zip
   unzip appledoc.zip
   cp appledoc /usr/local/bin
   cp -Rf Templates/ ~/.appledoc
   ```

1. Run `make ipackage`. The packaging script will produce a `build/ios/pkg/` folder containing:
  - a `dynamic` folder containing a dynamically-linked fat framework
  - a `static` folder containing a statically-linked framework
  - a `documentation` folder with HTML API documentation
  - an example `Settings.bundle` for providing the required Mapbox Metrics opt-out setting

### Install

In the context of your own app, you can now either:

#### CocoaPods

Currently, until [#1437](https://github.com/mapbox/mapbox-gl-native/issues/1437) is completed, to install a _development version_ of Mapbox GL using CocoaPods you will need to build it from source manually per above.

1. Zip up the build product.

    ```
    cd build/ios/pkg/
    ZIP=mapbox-ios-sdk.zip
    rm -f ../${ZIP}
    zip -r ../${ZIP} *
    ```

1. Customize [`Mapbox-iOS-SDK.podspec`](../ios/Mapbox-iOS-SDK.podspec) to download this zip file.

    ```rb
    {...}

    m.source = {
        :http => "http://{...}/mapbox-ios-sdk.zip",
        :flatten => true
    }

    {...}
    ```

1. Update your app's `Podfile` to point to the `Mapbox-iOS-SDK.podspec`.

    ```rb
    pod 'Mapbox-iOS-SDK', :podspec => 'http://{...}/Mapbox-iOS-SDK.podspec'
    ```

1. Run `pod update` to grab the newly-built library.

#### Dynamic framework

This is the recommended workflow for manually integrating the SDK into an application targeting iOS 8 and above:

1. Build from source manually per above.

1. Open the project editor and select your application target. Drag `build/ios/pkg/dynamic/Mapbox.framework` into the “Embedded Binaries” section of the General tab. In the sheet that appears, make sure “Copy items if needed” is checked, then click Finish.

1. In the Build Phases tab, click the + button at the top and select “New Run Script Phase”. Enter the following code into the script text field:

```bash
bash "${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}/Mapbox.framework/strip-frameworks.sh"
```

(The last step, courtesy of [Realm](https://github.com/realm/realm-cocoa/), is required for working around an [iOS App Store bug](http://www.openradar.me/radar?id=6409498411401216) when archiving universal binaries.)

#### Static framework

If your application targets iOS 7.x, you’ll need to install the static framework instead:

1. Build from source manually per above.

1. Open the project editor and select your application target. Drag `build/ios/pkg/static/Mapbox.framework` into the “Embedded Binaries” section of the General tab. In the sheet that appears, make sure “Copy items if needed” is checked, then click Finish.

1. Add the following Cocoa Touch frameworks and libraries to the “Linked Frameworks and Libraries” section:

   - `GLKit.framework`
   - `ImageIO.framework`
   - `MobileCoreServices.framework`
   - `QuartzCore.framework`
   - `SystemConfiguration.framework`
   - `libc++.dylib`
   - `libsqlite3.dylib`
   - `libz.dylib`

1. In the Build Settings tab, add `-ObjC` to the “Other Linker Flags” (`OTHER_LDFLAGS`) build setting.

### Use

1. Mapbox vector tiles require a Mapbox account and API access token. In the project editor, select the application target. In the Info tab, set `MGLMapboxAccessToken` to your access token. You can obtain one from the [Mapbox account page](https://www.mapbox.com/studio/account/tokens/).

1. In a XIB or storyboard, add a Custom View and set its custom class to `MGLMapView`. If you need to manipulate the map view programmatically, import the `Mapbox` module (Swift) or `Mapbox.h` umbrella header (Objective-C).

## Troubleshooting

On OS X, you can also try clearing the Xcode cache with `make clear_xcode_cache`.
