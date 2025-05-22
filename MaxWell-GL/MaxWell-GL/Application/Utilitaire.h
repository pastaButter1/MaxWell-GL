#pragma once

#include <string>

/** Afficher un message dans la console sous la forme "ERREUR | ..." */
void afficherErreur(const std::string& msg, ...);

/** Afficher un message dans la console sous la forme "LOG | ..." */
void afficherLog(const std::string& msg, ...);