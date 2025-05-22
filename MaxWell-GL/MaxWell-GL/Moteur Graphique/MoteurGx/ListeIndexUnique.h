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

	/** Allocation d'un bloc de donn�e sur le heap */
	void reserverLongueur(const uint32_t longueur)
	{
		if (longueur <= tamponIndex.size())
			return;

		tamponIndex.reserve(longueur);
		tamponDonnees.reserve(longueur);
	}

	/** Ajouter un �l�ment selon son constructeur de base ainsi que son index unique associ� � la fin de la pile */
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

	/** Ajouter un �l�ment cr�� � l'avance ainsi que son index unique associ� � la fin de la pile */
	TypeCompteur ajouter(const Contenant& item)
	{
		TypeCompteur indexUnique = ajouter();

		if (indexUnique < compteurMax)
		{
			tamponDonnees[tamponDonnees.size() - 1] = item;
		}

		return indexUnique;
	}

	/** Retire un �l�ment du tampon � l'aide de sa position dans le tampon par rapport au d�but du tampon et d�place les �l�ments suivant l'�l�ment retir� pour refermer le trou g�n�r� */
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

	/** Retire un �l�ment du tampon � l'aide de son index unique et d�place les �l�ments suivant l'�l�ment retir� pour refermer le trou g�n�r� */
	void retirerIndexUnique(const TypeCompteur indexUnique)
	{
		TypeCompteur index = retournerIndex(indexUnique);

		if (index < compteurMax)
		{
			retirerIndex(index);
		}
	}

	/** Retourne la position de l'�l�ment dans le tampon par rapport au d�but du tampon gr�ce � l'index unique associ� � cet �l�ment */
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

	/** Retourne la r�f�rence � un �l�ment gr�ce � sa position dans le tampon */
	Contenant& rechercherIndex(const TypeCompteur index)
	{
		if (index < compteurMax - 1)
		{
			return tamponDonnees[index];
		}

		Contenant* const ptrNull = nullptr;

		return *ptrNull;
	}

	/** Retourne la r�f�rence � un �l�ment gr�ce � son index unique */
	Contenant& rechercherIndexUnique(const TypeCompteur indexUnique)
	{
		return rechercherIndex(retournerIndex(indexUnique));
	}

	/** Retourne la r�f�rence constante � un �l�ment gr�ce � sa position dans le tampon */
	const Contenant& rechercherIndex(const TypeCompteur index) const
	{
		return ((ListeIndexUnique < TypeCompteur, Contenant>*)this)->rechercherIndex(index);
	}

	/** Retourne la r�f�rence constante � un �l�ment gr�ce � son index unique */
	const Contenant& rechercherIndexUnique(const TypeCompteur indexUnique) const
	{
		return ((ListeIndexUnique < TypeCompteur, Contenant>*)this)->rechercherIndexUnique(indexUnique);
	}
};