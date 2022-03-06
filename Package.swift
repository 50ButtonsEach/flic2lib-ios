// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "flic2lib",
    platforms: [
        .macOS(.v11), .iOS(.v12), .tvOS(.v14)
    ],
    products: [
        .library(
            name: "flic2lib",
            targets: ["flic2lib"])
    ],
    targets: [
        .binaryTarget(
            name: "flic2lib",
            path: "flic2lib.xcframework"
        )
    ]
)
