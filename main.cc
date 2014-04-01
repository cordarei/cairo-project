#include <iostream>
#include <memory>
#include <exception>
#include <string>
#include <vector>

#include <xcb/xcb.h>
#include <unistd.h>

namespace util {
  template<typename Concrete, typename Handle>
  struct handle_base {
    typedef Concrete concrete_type;
    typedef Handle dereferenced_type;
    typedef Handle* handle_type;
    
    handle_base(handle_type h) : _handle(h) {}
    handle_type handle() { return _handle.get(); }
    
  protected:
    struct destroyer {
      void operator()(handle_type h) {
	concrete_type::destroy(h);
      }
    };

    std::unique_ptr<dereferenced_type, destroyer> _handle;
  };
}

namespace xcb {

  struct window {
    window(xcb_window_t id) : _id(id) {}
    xcb_window_t id() const { return _id; }
  private:
    xcb_window_t _id;
  };

  struct connection : util::handle_base<connection, xcb_connection_t> {
    typedef util::handle_base<connection, xcb_connection_t> base_type;
    
    connection(): base_type(nullptr), _screen(-1) {
      _handle.reset(xcb_connect(nullptr, &_screen));
    }
    
    int screen() const { return _screen; }

    std::vector<xcb_screen_t> get_screens() {
      std::vector<xcb_screen_t> vec;

      auto it = xcb_setup_roots_iterator(xcb_get_setup(handle()));
      for (; it.rem > 0; xcb_screen_next(&it)) {
	vec.push_back(*(it.data));
      }

      return vec;
    }
    
    window create_window(int x, int y, int w, int h) {
      auto id = xcb_generate_id(handle());
      auto screens = get_screens();
      auto cookie = xcb_create_window(handle(),
				      XCB_COPY_FROM_PARENT,
				      id,
				      screens[0].root,
				      x, y, w, h,
				      10,
				      XCB_WINDOW_CLASS_INPUT_OUTPUT,
				      screens[0].root_visual,
				      0, nullptr);
      return window(id);
    }
    void map(const window &w) {
      xcb_map_window(handle(), w.id());
    }
    
    void flush() { xcb_flush(handle()); }

  private:
    friend base_type;
    static void destroy(xcb_connection_t *c) {
      xcb_disconnect(c);
    }
    
    int _screen;
  };
  
}

int main()
{
  xcb::connection c;
  
  std::cout << "Current screen: " << c.screen() << std::endl;

  auto screens = c.get_screens();
  for (int i = 0; i < screens.size(); ++i) {
    auto &s = screens[i];
    std::cout << "Screen " << i << ": " << s.width_in_pixels << "x" << s.height_in_pixels << std::endl;
  }
  
  xcb::window w = c.create_window(0, 0, 150, 150);
  c.map(w);
  c.flush();
  
  pause();

  return 0;
}
