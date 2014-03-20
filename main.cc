#include <iostream>
#include <memory>
#include <exception>
#include <string>

#include "sdl_wrap.h"


int main(int /*argc*/, char **/*argv*/)
{
  sdl::initializer init;
  
  sdl::window win("Hello World!", 100, 100, 640, 480);

  sdl::surface bmp = sdl::load_bitmap("hello.bmp");
  
  sdl::texture tex = win.renderer().create_texture(bmp);

  win.renderer().clear();
  win.renderer().copy(tex);
  win.renderer().present();
  
  sdl::delay(2000);

  return 0;
}
