#ifndef NES_EMULATOR_RENDERER
#define NES_EMULATOR_RENDERER

namespace NesEm
{
	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		Renderer(Renderer const&) = default;
		Renderer(Renderer&&) = default;
		Renderer& operator=(Renderer const&) = default;
		Renderer& operator=(Renderer&&) = default;

		virtual void Render() const = 0;

		virtual void ToggleFullScreen() noexcept = 0;
	};

	class NullRenderer final : public Renderer
	{
	public:
		NullRenderer() = default;
		~NullRenderer() override = default;

		NullRenderer(NullRenderer const&) = default;
		NullRenderer(NullRenderer&&) = default;
		NullRenderer& operator=(NullRenderer const&) = default;
		NullRenderer& operator=(NullRenderer&&) = default;

		void Render() const override {}

		virtual void ToggleFullScreen() noexcept override {}
	};
} 

#endif