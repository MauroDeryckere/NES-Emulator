#ifndef NES_EMULATOR_MACROS
#define NES_EMULATOR_MACROS

// Configuration
#define NES_EM_USE_STATIC_CONSTEXPR_TABLE 1
#define NES_EM_DEBUG_MODE 1

// Defines required when in debug or other config modes
#if NES_EM_DEBUG_MODE
// Initialize the string implementation array for our instructions
#define NES_EM_INIT_STRING_REPRESENTATION 1
#endif

// Compiler specific force inline
// For MSVC
#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
// For GCC and Clang
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

// Allow swapping between static and non static tables
#if NES_EM_USE_STATIC_CONSTEXPR_TABLE
#define NES_EM_TABLE static constexpr
#else
#define NES_EM_TABLE const
#endif

#endif