#include <clear.hpp>

void clear() {
#ifdef __linux__
	system("clear");
#else
	system("cls");
#endif
}