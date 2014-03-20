#ifndef __MY__SDL_H__
#define __MY__SDL_H__

#include <string>
#include <memory>
#include <exception>

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

namespace sdl {
  struct sdl_error : std::exception {
    sdl_error();
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
    
    template<typename C, typename H>
      struct handle_t {
	typedef C concrete_type;
	typedef H dereferenced_type;
	typedef H* handle_type;

      handle_t(handle_type h) : _handle(h) {};
	  
	handle_type handle() { return _handle.get(); }
      
      private:
	struct destroyer {

	  void operator()(handle_type h) {
	    concrete_type::destroy(h);
	  }
	};
	std::unique_ptr<dereferenced_type, destroyer> _handle;
      };
  }

  struct initializer {
    initializer();
    ~initializer();
  };
  
  void delay(int ms);


  class surface : public details::handle_t<surface, SDL_Surface> {
    typedef details::handle_t<surface, SDL_Surface> base_type;
    friend base_type;

    surface(SDL_Surface *h) : base_type(h) {}
    static void destroy(SDL_Surface *h);
	  
    friend surface load_bitmap(const std::string& filename);
  };

  surface load_bitmap(const std::string& filename);
  

  struct texture : details::handle_t<texture, SDL_Texture> {
    typedef details::handle_t<texture, SDL_Texture> base_type;
    friend base_type;

  texture(SDL_Texture *h) : base_type(h) {}

  private:
    static void destroy(SDL_Texture *h);
  };


  class window {
    struct window_handle : details::handle_t<window_handle, SDL_Window> {
      typedef details::handle_t<window_handle, SDL_Window> base_type;
      friend base_type;

    window_handle(SDL_Window *h) : base_type(h) {}
      static void destroy(SDL_Window *h);
    };

    struct render_handle : details::handle_t<render_handle, SDL_Renderer> {
      typedef details::handle_t<render_handle, SDL_Renderer> base_type;
      friend base_type;

    render_handle(SDL_Renderer *h) : base_type(h) {}
      static void destroy(SDL_Renderer *h);
    };

  public:
    class renderer_type {
    public:
      texture create_texture(surface &s);
      void clear();
      void copy(texture& t);
      void present();

    renderer_type(render_handle& h) : _handle(h) {}

    private:
      render_handle& _handle;
    };

    renderer_type renderer() { return renderer_type(_renderer); }

    window(const std::string& name, int x, int y, int w, int h);

  private:
    window_handle _handle;
    render_handle _renderer;
  };
}

#endif
