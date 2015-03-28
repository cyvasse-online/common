#if __has_include(<optional>)
	#include <optional>
	using std::optional;
	using std::make_optional;
	using std::nullopt;
#elif __has_include(<experimental/optional>)
	#include <experimental/optional>
	using std::experimental::optional;
	using std::experimental::make_optional;
	using std::experimental::nullopt;
#else
	#error No optional support
#endif
