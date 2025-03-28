#include "Moteur Graphique/Model/DecoderFichier.h"

#include "Model.h"

#include <stdio.h>
#include <vector>

void decoderSTL(const char* addresse, Model* const model)
{
	std::string tampon;
	chargerFichier(addresse, &tampon);

	char* ptr = (char*)tampon.c_str();

	ptr += 80;

	model->nbTriangle = *(uint32_t*)ptr;

	model->triangles = new Triangle[model->nbTriangle];

	ptr += 4;

	for (int i = 0; i < model->nbTriangle; i++)
	{
		Triangle& triangle = model->triangles[i];

		glm::vec3 norm = *((glm::vec3*)ptr);

		triangle.verts[0].norm = norm;
		triangle.verts[1].norm = norm;
		triangle.verts[2].norm = norm;

		ptr += sizeof(Vertex::norm);

		model->triangles[i].verts[0].pos = *((glm::vec3*)ptr);
		ptr += sizeof(Vertex::pos);
		model->triangles[i].verts[1].pos = *((glm::vec3*)ptr);
		ptr += sizeof(Vertex::pos);
		model->triangles[i].verts[2].pos = *((glm::vec3*)ptr);
		ptr += sizeof(Vertex::pos);

		ptr += 2;
	}
}

void decoderOBJ(const char* addresse, Model* const model)
{
	std::string tampon;
	chargerFichier(addresse, &tampon);

	std::vector<glm::vec3> posList;
	std::vector<glm::vec2> uvList;
	std::vector<glm::vec3> normList;
	std::vector<Triangle> triangleList;

	char* ptr = (char*)tampon.c_str();

	bool sauterLigne = false;
	bool premierChar = true;
	bool debutDonnee = false;
	uint32_t typeDonnee = 0;
	uint32_t c1 = 0;
	uint32_t c2 = 0;

	constexpr uint32_t tailleAccumulateur = 32;
	char accumulateur[tailleAccumulateur];
	char* ptrAccumulateur = accumulateur;
	while (ptr < tampon.c_str() + tampon.size())
	{
		if (*ptr == '\0') {
			break;
		}

		else if (!sauterLigne)
		{
			switch (*ptr)
			{
			case '#':
			case 'o':
			case 'm':
			case 'u':
			case 's':
				sauterLigne = true;
				break;
			case 'f':
				typeDonnee = 0;
				break;
			case 'v':
				typeDonnee = 1;
				break;
			case 't':
				typeDonnee = 2;
				break;
			case 'n':
				typeDonnee = 3;
				break;
			case '/':
				if (typeDonnee == 0 && debutDonnee)
				{
					*ptrAccumulateur = '\0';

					int index = std::atoi(accumulateur) - 1;

					switch (c2)
					{
					case 0:
						(triangleList.end() - 1)->verts[c1].pos = posList[index];
						break;
					case 1:
						(triangleList.end() - 1)->verts[c1].uv = uvList[index];
						break;
					}

					ptrAccumulateur = accumulateur;

					c2++;
				}
				break;
			case '\n':
			case ' ':
				if (debutDonnee)
				{
					*ptrAccumulateur = '\0';

					float val = std::atof(accumulateur);
					int index = std::atoi(accumulateur) - 1;

					switch (typeDonnee)
					{
					case 0:
						(triangleList.end() - 1)->verts[c1].norm = normList[index];
						c2 = 0;
						break;
					case 1:
						(*(posList.end() - 1))[c1] = val;
						break;
					case 2:
						(*(uvList.end() - 1))[c1] = val;
						break;
					case 3:
						(*(normList.end() - 1))[c1] = val;
						break;
					}

					ptrAccumulateur = accumulateur;

					c1++;

					if (*ptr == '\n')
					{
						c1 = 0;
					}
				}

				else
				{
					switch (typeDonnee)
					{
					case 0:
						triangleList.emplace_back();
						break;
					case 1:
						posList.emplace_back();
						break;
					case 2:
						uvList.emplace_back();
						break;
					case 3:
						normList.emplace_back();
						break;
					}
				}

				debutDonnee = true;
				break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
			case '.':
				*ptrAccumulateur = *ptr;
				
				if (ptrAccumulateur - accumulateur + 1 < tailleAccumulateur)
				{
					ptrAccumulateur++;
				}
				break;
			}

		}

		if (*ptr == '\n')
		{
			sauterLigne = false;
			premierChar = true;
			debutDonnee = false;
		}

		ptr++;
	}

	model->nbTriangle = triangleList.size();

	model->triangles = new Triangle[model->nbTriangle];

	memcpy_s(model->triangles, model->nbTriangle * sizeof(Triangle), triangleList.data(), triangleList.size() * sizeof(Triangle));
}

bool chargerFichier(const std::string& addresse, std::string* const donnees)
{
	FILE* fichier;

	errno_t erreur = fopen_s(&fichier, addresse.c_str(), "r");

	if (erreur != 0)
	{
		printf("ERREUR | Fichier non-trouve a l'addresse : %s", addresse.c_str());

		return false;
	}

	fseek(fichier, 0, SEEK_SET);
	uint32_t p0 = ftell(fichier);

	fseek(fichier, 0, SEEK_END);
	uint32_t p1 = ftell(fichier);

	fseek(fichier, 0, SEEK_SET);

	uint32_t l = p1 - p0;
	donnees->resize(l);

	fread_s((void*)donnees->c_str(), donnees->size(), 1, l, fichier);

	fclose(fichier);

	return true;
}