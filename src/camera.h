#ifndef _Camera
#define _Camera
#include <esp_camera.h>

class Camera {
public:
  void camera_init(void);

  void test_camera(void);

  camera_fb_t *get_image(void);

  void clear_image(camera_fb_t *fb);
};

#endif
