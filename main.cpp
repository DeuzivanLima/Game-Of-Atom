#include <iostream>
#include <memory>
#include <list>

extern "C"
{

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

}

class Atom final
{
	public:
		Atom(const std::string &symbol, const float weight, const std::list<std::uint8_t> energy_levels) noexcept
		{
			this->setWeight(weight);
			this->setSymbol(symbol);
			this->setEnergyLevels(energy_levels);
		}

		virtual void setEnergyLevels(const std::list<std::uint8_t> energy_levels) noexcept
		{
			this->__energy_levels = energy_levels;
		}
		const std::list<std::uint8_t> getEnergyLevels() const noexcept
		{
			return this->__energy_levels;
		}
		virtual void setSymbol(const std::string &symbol) noexcept
		{
			this->__symbol = symbol;
		}
		const std::string &getSymbol() const noexcept
		{
			return this->__symbol;
		}
		virtual void setWeight(const float weight) noexcept
		{
			this->__weight = weight;
		}
		const float &getWeight() const noexcept
		{
			return this->__weight;
		}
		virtual void setTemperature(const std::uint16_t temperature) noexcept 
		{
			this->__temperature = temperature;
		}
		const std::uint16_t &getTemperature() const noexcept
		{
			return this->__temperature;
		}

		~Atom() noexcept
		{
		}
	private:
		std::list<std::uint8_t> __energy_levels;
		std::string __symbol;
		float __weight;
		std::uint16_t __temperature;
};

int main(int argc, char *argv[])
{
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window (
		SDL_CreateWindow("SandBox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_SHOWN),
		SDL_DestroyWindow
	);
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer (
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
		SDL_DestroyRenderer
	);
	SDL_Event event;

	Atom hydrogen("H", 1.0008, std::list<std::uint8_t>(1));

	while(window.get() && renderer.get())
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				window.~unique_ptr();
		}
		SDL_SetRenderDrawColor(renderer.get(), 10, 10, 10, 255);
		SDL_RenderClear(renderer.get());


		SDL_RenderPresent(renderer.get());
	}

	return EXIT_SUCCESS;
}
