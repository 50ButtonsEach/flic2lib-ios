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
    .library(name: "Flic2a", targets: ["Flic2b", "Flic2XCFramework"]),
  ],

  targets: [
    .target(name: "Flic2b", dependencies: ["Flic2XCFramework"]),
    .binaryTarget(name: "Flic2XCFramework", path: "flic2lib.xcframework"),
  ]
)
