// swift-tools-version:5.3
import PackageDescription

let package = Package(
  name: "Flic2",
  platforms: [
    .macOS(.v11),
    .iOS(.v12),
    .tvOS(.v14)
  ],

  products: [
    .library(name: "Flic2", targets: ["Flic2", "Flic2XCFramework"]),
  ],

  targets: [
    .target(name: "Flic2", dependencies: ["Flic2XCFramework"]),
    .binaryTarget(name: "Flic2XCFramework", path: "flic2lib.xcframework"),
  ]
)
