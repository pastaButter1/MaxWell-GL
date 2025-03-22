#pragma once

#include <vector>

template<typename TypeCompteur, typename Contenant>
class ListeIndexUnique
{
	std::vector<TypeCompteur> tamponIndex;
	std::vector<Contenant> tamponDonnees;
	uint32_t compteurTotal = 0;

	const TypeCompteur compteurMax = (~(TypeCompteur)0); 

public:
	ListeIndexUnique()
	{

	}

	void reserverLongueur(const uint32_t longueur)
	{
		if (longueur <= tamponIndex.size())
			return;

		tamponIndex.reserve(longueur);
		tamponDonnees.reserve(longueur);
	}

	TypeCompteur ajouter()
	{
		TypeCompteur indexUnique = compteurTotal;

		if (compteurTotal < compteurMax)
		{
			compteurTotal++;
			tamponIndex.emplace_back(indexUnique);
			tamponDonnees.emplace_back();
		}

		return indexUnique;
	}

	TypeCompteur ajouter(const Contenant& item)
	{
		TypeCompteur indexUnique = ajouter();

		if (indexUnique < compteurMax)
		{
			tamponDonnees[tamponDonnees.size() - 1] = item;
		}

		return indexUnique;
	}

	void retirerIndex(const TypeCompteur index)
	{
		if (index < tamponIndex.size())
		{
			for (int i = index; i < tamponIndex.size() - 1; i++)
			{
				tamponIndex[i] = tamponIndex[i + 1];
			}

			for (int i = index; i < tamponIndex.size() - 1; i++)
			{
				tamponDonnees[i] = tamponDonnees[i + 1];
			}

			tamponIndex.pop_back();
			tamponDonnees.pop_back();
		}
	}

	void retirerIndexUnique(const TypeCompteur indexUnique)
	{
		TypeCompteur index = retournerIndex(indexUnique);

		if (index < compteurMax)
		{
			retirerIndex(index);
		}
	}

	const TypeCompteur retournerIndex(const TypeCompteur indexUnique) const
	{
		if (indexUnique >= compteurTotal)
			return compteurMax;

		const uint32_t tailleTampon = tamponIndex.size();
		const uint32_t essaiDepart = (uint32_t)indexUnique * tailleTampon / compteurTotal;

		uint32_t essai = essaiDepart < tailleTampon ? essaiDepart : tailleTampon;
		uint32_t resultat = compteurMax;
		uint32_t essaiMin = 0;
		uint32_t essaiMax = tailleTampon;
		uint32_t dst = 0;

		// Recherche binaire
		// Les indexes sont toutes en ordre croissant

		resultat = tamponIndex[essai];

		while (resultat != indexUnique)
		{
			resultat = tamponIndex[essai];

			if (resultat < indexUnique)
			{
				dst = essaiMax - essai;

				essaiMin = essai;

				essai += dst >> 1;
			}

			else if (resultat > indexUnique)
			{
				dst = essai - essaiMin;

				essaiMax = essai;

				essai -= dst >> 1;
			}

			if (essaiMax - essaiMin > 1)
			{
				essai = compteurMax;
				break;
			}
		}

		return essai;
	}

	Contenant& rechercherIndex(const TypeCompteur index)
	{
		if (index < compteurMax - 1)
		{
			return tamponDonnees[index];
		}

		Contenant* const ptrNull = nullptr;

		return *ptrNull;
	}

	Contenant& rechercherIndexUnique(const TypeCompteur indexUnique)
	{
		return rechercherIndex(retournerIndex(indexUnique));
	}

	const Contenant& rechercherIndex(const TypeCompteur index) const
	{
		return ((ListeIndexUnique < TypeCompteur, Contenant>*)this)->rechercherIndex(index);
	}

	const Contenant& rechercherIndexUnique(const TypeCompteur indexUnique) const
	{
		return ((ListeIndexUnique < TypeCompteur, Contenant>*)this)->rechercherIndexUnique(indexUnique);
	}
};