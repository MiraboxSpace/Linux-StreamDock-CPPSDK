# Overview

The `Linux StreamDock SDK` is an SDK designed for direct control of `StreamDock` devices on `Linux` systems, bypassing software interfaces. With this SDK, you can easily implement direct control of the device.

<img src="https://cdn1.key123.vip/creator/creator_img/SDK.png" alt="Linux StreamDock SDK">

## How the `Linux StreamDock SDK` Communicates with the Device

The `Linux StreamDock SDK` communicates with the device using `HIDAPI`, a simple cross-platform library for communicating with `HID` devices. [Learn more](https://github.com/libusb/hidapi).

::: tip
It is recommended to use `C++11 or later` versions, `Ubuntu 16.04` for Linux, `zlib` version `1.2.11`, `libpng` version `1.6.37`, and `libjpeg` version `v9b`.
:::

Please see the [document](https://creator.key123.vip/en/linux/cpp/dependency.html) for details