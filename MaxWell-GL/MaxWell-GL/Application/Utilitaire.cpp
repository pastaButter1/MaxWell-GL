#include "Utilitaire.h"

#include <conio.h>

void afficherErreur(const std::string& msg, ...)
{
	std::string info = "ERREUR | ";

	va_list args;
	__crt_va_start(args, &msg);

	info.append(msg);
	info.append(1, '\n');

	vprintf(info.c_str(), args);

	__crt_va_end(args);
}

void afficherLog(const std::string& msg, ...)
{
	std::string info = "LOG | ";

	va_list args;
	__crt_va_start(args, &msg);

	info.append(msg);
	info.append(1, '\n');

	vprintf(info.c_str(), args);

	__crt_va_end(args);
}
