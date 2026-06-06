  1. Hardware Strategy

  Core Controller
   * Recommended Hardware: ESP32-S3-WROOM-1-N16R8 (16MB Flash, 8MB PSRAM).
   * Rationale: The 8MB PSRAM is critical for handling the 1600x1200 image buffers and the MobileNet-based TFLite model. The S3's dual-core architecture allows one core to handle the Web Server/Connectivity while the other manages AI inference and motor control.

  Peripherals
   * Camera: OV2640 or OV5640. These are compatible with the ESP32-S3's parallel DVP interface.
   * Motor Drivers: The existing H-Bridge drivers (DRV8833 or similar) are 3.3V logic compatible and can be driven directly by ESP32 PWM pins.
   * Servos: ESP32 can generate precise PWM for the 4 servos (Swing, Tilt, Roll, Lid) using its LEDC or MCPWM hardware peripherals.

  2. Software Architecture

  Framework & Libraries
   * Development Environment: PlatformIO or Arduino IDE (C++).
   * AI Inference: TensorFlow Lite Micro (TFLM). The existing model_int8.tflite will be converted to a C++ header file using xxd.
   * Vision: Use the esp_camera library for capture and custom pixel-based algorithms for "bright spot" detection (replacing the OpenCV threshold and findContours logic used for sun orientation).
   * Web Interface: ESPAsyncWebServer to replicate the Flask-based control panel.
   * Storage: LittleFS for serving the web UI and an SD Card (via SPI) for saving logs and images.

  3. Proposed Pin Mapping (ESP32-S3)

  ┌──────────────────┬──────────────────┬─────────────────────────────────┐
  │ Function         │ ESP32-S3 GPIO    │ Note                            │
  ├──────────────────┼──────────────────┼─────────────────────────────────┤
  │ Camera DVP       │ 1-14, 38, 39, 40 │ Standard ESP32-S3 Camera Pinout │
  │ Robot (Wheels) L │ 15, 16           │ Phase/Enable Control            │
  │ Robot (Wheels) R │ 17, 18           │ Phase/Enable Control            │
  │ Z-Axis L         │ 19, 20           │ Phase/Enable Control            │
  │ Z-Axis R         │ 21, 35           │ Phase/Enable Control            │
  │ Servo: Lid       │ 41               │ PWM (LEDC)                      │
  │ Servo: Tilt      │ 42               │ PWM (LEDC)                      │
  │ Servo: Roll      │ 45               │ PWM (LEDC)                      │
  4. Implementation Plan

  Phase 1: Research & Setup
   1. Model Conversion: Convert model_int8.tflite to a C array. Verify memory requirements (MobileNetV2 INT8 usually fits in ~3-4MB).
   2. Logic Porting: Translate the Python sunTracker() and orientToSun() algorithms into C++. Replace imutils and cv2 calls with optimized loops for the ESP32's camera_fb_t buffer.

  Phase 2: Execution
   1. Firmware Core: Implement the Wi-Fi Access Point and Async Web Server.
   2. Actuator Layer: Develop the PhaseEnableRobot and AngularServo classes in C++ to match the gpiozero behavior.
   3. Vision Layer: Implement image capture, cropping (to 224x224), and TFLM inference.

  Phase 3: Validation
   1. Motor Test: Run the test-full movements to ensure GPIO and PWM timings are correct.
   2. Inference Test: Compare the classification output of the ESP32 against the original RPi implementation using a set of reference images.
   3. Field Test: Validate the sun-tracking orientation loop, which is the most timing-sensitive part of the project.

  Key Advantages of ESP32 Transition
   * Instant-On: ESP32 boots in milliseconds compared to the RPi's minute-long Linux boot.
   * Power Efficiency: Significantly lower power consumption, extending solar battery life.
   * Reliability: No SD card corruption issues common with RPi power cycles.
