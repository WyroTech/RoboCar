#ifndef _Camera
#define _Camera
#include "esp_camera.h"
#include <WiFi.h>

class Camera {
public:
  void camera_init(void);

  void test_camera(void);
};

#endif
