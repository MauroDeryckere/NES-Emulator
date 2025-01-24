#ifndef NES_EMULATOR_RENDERER
#define NES_EMULATOR_RENDERER

class Renderer final
{
	public:
		Renderer() = default;
		~Renderer() = default;
		Renderer(Renderer const&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer const&) = delete;
		Renderer& operator=(Renderer&&) = delete;	
		
		bool Initialize() noexcept;

		void Render() const;

	private:
}

#endif