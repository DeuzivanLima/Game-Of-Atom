#include <iostream>
#include <memory>
#include <list>
#include <vector>

extern "C"
{

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

}

class Atom final
{
	public:
		Atom()
		{
		}
		Atom(const std::string &symbol, const float weight, const std::list<std::uint8_t> energy_levels) noexcept
		{
			this->setWeight(weight);
			this->setSymbol(symbol);
			this->setEnergyLevels(energy_levels);
			this->setTemperature(0.f);
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
		virtual void setTemperature(const float temperature) noexcept 
		{
			this->__temperature = temperature;
		}
		const float &getTemperature() const noexcept
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
		float __temperature;
};

class Grid
{
	public:
		Grid(const std::uint16_t slot_amount, SDL_FPoint position = {0.f, 0.f}) noexcept
		{
			this->setSlotAmount(slot_amount);
			this->setPosition(position);
			this->setSlotSize(16.f);
		}

		virtual void setPosition(SDL_FPoint position) noexcept
		{
			this->__position = position;
		}	
		const SDL_FPoint &getPosition() const noexcept
		{
			return this->__position;
		}
		virtual void setSlotAmount(const std::uint16_t slot_amount) noexcept
		{
			this->__slot_amount = slot_amount;
		}
		const std::uint16_t &getSlotAmount() const noexcept
		{
			return this->__slot_amount;
		}
		virtual void setSlotSize(const float slot_size) noexcept
		{
			this->__slot_size = slot_size;
		}
		const float &getSlotSize() const noexcept
		{
			return this->__slot_size;
		}

		virtual void draw(SDL_Renderer *renderer) const noexcept
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			for(int i = 0; i < this->__slot_amount + 1; i++)
			{
				SDL_RenderDrawLineF(renderer,
						this->__position.x + this->__slot_size * i, this->__position.y,
						this->__position.x + this->__slot_size * i, this->__position.y + this->__slot_size * this->__slot_amount);
				SDL_RenderDrawLineF(renderer,
						this->__position.x, this->__position.y + this->__slot_size * i,
						this->__position.x + this->__slot_size * this->__slot_amount, this->__position.y + this->__slot_size * i);
			}
		}
		
		virtual ~Grid() noexcept
		{
		}
	private:
		std::uint16_t __slot_amount;
		SDL_FPoint __position;
		float __slot_size;
};

class Universe
{
	public:
		Universe(const Grid &grid) noexcept
			: __grid{&grid}
		{
			this->_generate();
			Atom hydrogen("H", 1.008, std::list<std::uint8_t>(1));
			this->__atoms[3][3] = &hydrogen;
		}

		virtual void draw(SDL_Renderer *renderer) const noexcept
		{
			for(int y = 0; y < static_cast<int>(this->__atoms.size()); y++)
				for(int x = 0; x < static_cast<int>(this->__atoms[y].size()); x++)
				{
					Atom *atom = this->__atoms[y][x];
					
					if(atom != nullptr)
					{
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
						
						SDL_FRect rect {
							this->__grid->getPosition().x + x * this->__grid->getSlotSize(),
							this->__grid->getPosition().y + y * this->__grid->getSlotSize(),
							this->__grid->getSlotSize(), this->__grid->getSlotSize()
						};

						SDL_RenderFillRectF(renderer, &rect);
					}
				}
		}

		virtual ~Universe() noexcept
		{
			for(auto row : this->__atoms) for(Atom *atom : row)
				if(atom != nullptr)
					delete atom;
		}
	protected:
		void _generate() noexcept
		{
			for(int x = 0; x < this->__grid->getSlotAmount(); x++)
			{
				std::vector<Atom*> row;
				for(int y = 0; y < this->__grid->getSlotAmount(); y++)
					row.push_back(nullptr);
				this->__atoms.push_back(row);
			}
		}

	private:
		std::vector<std::vector<Atom*>> __atoms;
		const Grid *__grid;
};

int main(int, char *[])
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

	Atom hydrogen("H", 1.008, std::list<std::uint8_t>(1));
	Grid grid(16, {720.f / 4.f, 480.f / 4.f});
	Universe universe(grid);

	while(window.get() && renderer.get())
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				window.~unique_ptr();
		}
		SDL_SetRenderDrawColor(renderer.get(), 10, 10, 10, 255);
		SDL_RenderClear(renderer.get());

		universe.draw(renderer.get());
		grid.draw(renderer.get());

		SDL_RenderPresent(renderer.get());
	}

	return EXIT_SUCCESS;
}
