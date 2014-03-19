#include "sdl_wrap.h"
#include "SDL.h"


namespace sdl {

	sdl_error::sdl_error() : _msg(SDL_GetError()) {}

	initializer::initializer() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			throw sdl_error();
		}
	}
	initializer::~initializer() {
		SDL_Quit();

	}


	void surface::destroy(SDL_Surface *h) {
		SDL_FreeSurface(h);
	}

	surface load_bitmap(const std::string& filename) {
		return details::check_ptr(SDL_LoadBMP(filename.c_str()));
	}


	void texture::destroy(SDL_Texture *h) {
		SDL_DestroyTexture(h);
	}

	void window::window_handle::destroy(SDL_Window *h) {
		SDL_DestroyWindow(h);
	}

	void window::render_handle::destroy(SDL_Renderer *h) {
		SDL_DestroyRenderer(h);
	}

	window::window(const std::string& name, int x, int y, int w, int h)
		: _handle(details::check_ptr(SDL_CreateWindow(name.c_str(), x, y, w, h, SDL_WINDOW_SHOWN))),
		_renderer(details::check_ptr(SDL_CreateRenderer(_handle.handle(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
	}

	void window::renderer_type::clear() {
		SDL_RenderClear(_handle.handle());
	}

	void window::renderer_type::present() {
		SDL_RenderPresent(_handle.handle());
	}

	void window::renderer_type::copy(texture& t) {
		SDL_RenderCopy(_handle.handle(), t.handle(), nullptr, nullptr);
	}

	texture window::renderer_type::create_texture(surface& s) {
		return details::check_ptr(SDL_CreateTextureFromSurface(_handle.handle(), s.handle()));
	}
}
