// swift-tools-version:5.3
import PackageDescription

let package = Package(
  name: "flic2lib",
  platforms: [
    .macOS(.v11),
    .iOS(.v12),
    .tvOS(.v14)
  ],

  products: [
    .library(name: "Flic2", targets: ["flic2lib", "Flic2XCFramework"]),
  ],

  targets: [
    .target(name: "flic2lib", dependencies: ["Flic2XCFramework"]),
    .binaryTarget(name: "Flic2XCFramework", path: "flic2lib.xcframework"),
  ]
)
