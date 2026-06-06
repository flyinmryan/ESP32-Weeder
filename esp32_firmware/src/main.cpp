#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "esp_camera.h"
#include "config.h"
#include "Actuators.h"
#include "WeederLogic.h"
#include "model_data.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Hardware Objects
PhaseEnableRobot robot(WHEEL_L_PHASE, WHEEL_L_ENABLE, WHEEL_R_PHASE, WHEEL_R_ENABLE);
PhaseEnableRobot zaxis(ZAXIS_L_PHASE, ZAXIS_L_ENABLE, ZAXIS_R_PHASE, ZAXIS_R_ENABLE);
AngularServo lid(SERVO_LID_PIN, 180, 0, 180, 1200, 2800);
AngularServo tilt(SERVO_TILT_PIN, 110, 0, 180, 500, 2800);
AngularServo roll(SERVO_ROLL_PIN, 90, 0, 180, 900, 1400);
AngularServo swing(SERVO_SWING_PIN, 90, 0, 180, 1200, 1700);

WeederLogic logic(robot, zaxis, lid, tilt, roll, swing);

// Web Server
AsyncWebServer server(80);

// TFLite Variables
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  const int kTensorArenaSize = 1024 * 512; // 512KB for arena
  uint8_t tensor_arena[kTensorArenaSize];
}

void setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; // PIXFORMAT_GRAYSCALE for orientToSun

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // 1600x1200
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void setupTFLite() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(g_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter, "Model version mismatch!");
    return;
  }

  static tflite::AllOpsResolver resolver;
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }
  input = interpreter->input(0);
}

int classify() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) return -1;

  // 1. Resize/Crop fb->buf to input->dims (e.g., 224x224)
  // 2. Normalize and fill input->data.f
  // 3. Run interpreter->Invoke()
  // 4. Get output tensor and find max probability

  esp_camera_fb_return(fb);
  return 0; // Placeholder for predicted class
}

void setup() {
  Serial.begin(MONITOR_SPEED);
  
  pinMode(QUAD1_PIN, INPUT_PULLUP);
  pinMode(QUAD2_PIN, INPUT_PULLUP);
  pinMode(QUAD3_PIN, INPUT_PULLUP);
  pinMode(QUAD4_PIN, INPUT_PULLUP);

  setupCamera();
  setupTFLite();

  WiFi.softAP("weeder", "LetsWeed");
  Serial.println("AP Started: 192.168.4.1");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Weeder ESP32-S3 Online");
  });

  server.on("/run", HTTP_GET, [](AsyncWebServerRequest *request){
    // Start weeding run
    request->send(200, "text/plain", "Weeder started");
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    robot.stop();
    request->send(200, "text/plain", "Weeder stopped");
  });

  server.begin();
}

void loop() {
  logic.sunTracker(QUAD1_PIN, QUAD2_PIN, QUAD3_PIN, QUAD4_PIN);
  delay(100);
}
