#pragma once

#include "Model.h"

#include <string>

void decoderSTL(const char* filepath, Model& model);

void decoderOBJ(const char* filepath, Model& model);

bool chargerFichier(const std::string& addresse, std::string* const donnees);