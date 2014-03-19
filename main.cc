#include <iostream>
#include <memory>
#include <exception>
#include <string>

#include <SDL.h>

namespace sdl {

  struct sdl_error : std::exception {
    sdl_error() : _msg(SDL_GetError()) {}
    const char *what() const noexcept { return _msg.c_str(); }
  private:
    std::string _msg;
  };
  
  namespace details {
    template<typename T>
    T check_ptr(T p) {
      if (p == nullptr) throw sdl_error();
      else return p;
    }
  }

  struct initializer {
    initializer() {
      if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	throw sdl_error();
      }
    }

    ~initializer() {
      SDL_Quit();
    }
  };
  

  struct window;
  struct renderer;
  

  struct surface {
  private:
    struct destroyer { void operator()(SDL_Surface *s) { SDL_FreeSurface(s); } };

    surface(SDL_Surface *handle): _handle(handle) {}
    
    static SDL_Surface *create_from_bitmap(const std::string& filename) {
      return details::check_ptr(SDL_LoadBMP(filename.c_str()));
    }

    std::unique_ptr<SDL_Surface, destroyer> _handle;

    // Friends:
    friend surface load_bitmap(const std::string& filename);
  };
  
  surface load_bitmap(const std::string& filename) {
    return surface::create_from_bitmap(filename);
  }
  

  struct texture {
  private:
    struct destroyer { void operator()(SDL_Texture *t) { SDL_DestroyTexture(t); } };

    texture(SDL_Texture *handle): _handle(handle) {}

    std::unique_ptr<SDL_Texture, destroyer> _handle;
    
    //Friends:
    friend struct renderer;
  };

  struct renderer {

    void clear() { SDL_RenderClear(_handle); }
    void present() { SDL_RenderPresent(_handle); }

  private:
    friend struct window;

    renderer() : _handle(nullptr) {}
    renderer(window& win) : _handle(create(win)) {}

    ~renderer() {
      destroy();
    }
      
    void destroy() {
      if (_handle != nullptr) {
	SDL_DestroyRenderer(_handle);
	_handle = nullptr;
      }
    }
      
    static SDL_Renderer *create(window& win) {
      return details::check_ptr(SDL_CreateRenderer(win._handle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    }

    SDL_Renderer *_handle;
  };
  
  struct window {
    
    window(const std::string& name, int x, int y, int w, int h)
      : _handle(create(name, x, y, w, h)),
	_renderer(*this) {}

    ~window() {
      _renderer.destroy();
      SDL_DestroyWindow(_handle);
    }
    
    
    renderer& renderer() { return _renderer; }

  private:
    friend struct renderer;

    static SDL_Window *create(const std::string& name, int x, int y, int w, int h) {
      return details::check_ptr(SDL_CreateWindow(name.c_str(), x, y, w, h, SDL_WINDOW_SHOWN));
    }

    SDL_Window *_handle;
    renderer _renderer;
  };

}


int main(int /*argc*/, char **/*argv*/)
{
  sdl::initializer init;
  
  sdl::window win("Hello World!", 100, 100, 640, 480);

  sdl::surface bmp = sdl::load_bitmap("hello.bmp");
  
  // SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
  // SDL_FreeSurface(bmp);
  // if (tex == nullptr){
  //   std::cout << "SDL_CreateTextureFromSurface Error: "
  // 	      << SDL_GetError() << std::endl;
  //   return 1;
  // }

  // SDL_RenderClear(ren);
  // SDL_RenderCopy(ren, tex, NULL, NULL);
  // SDL_RenderPresent(ren);

  // SDL_Delay(2000);
 
  // SDL_DestroyTexture(tex);
  // SDL_DestroyRenderer(ren);

  return 0;
}
