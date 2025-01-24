#ifndef NES_EMULATOR_SINGLETON
#define NES_EMULATOR_SINGLETON

namespace NesEm
{
	template <typename T>
	class Singleton
	{
	public:
		[[nodiscard]] static T& GetInstance() noexcept
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;

	protected:
		Singleton() = default;
	};
}

#endif 