/**
* \file Graphe.cpp
* \brief Implémentation d'un graphe orienté.
* \author Hamza Hajjam
* \version 0.5
* \date mars 2023
*
*  Travail pratique numéro 2
*
*/

#include "Graphe.h"

//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    /**
     * \fn Graphe::Graphe(size_t nbSommets)
     * \brief Constructeur, initialise les vecteurs et mets à jours le nombre d'arc et de sommet
     * \param[in] nbSommets
     */
	Graphe::Graphe(size_t nbSommets){
        this->nbSommets = nbSommets;
        this->nbArcs = 0;

        sommets.resize(nbSommets);
        listesAdj.resize(nbSommets);
	}

    /**
     * \fn Graphe::~Graphe()
     * \brief deConstructeur
     */
	Graphe::~Graphe(){
	}

    /**
     * \fn void Graphe::resize(size_t nouvelleTaille)
     * \brief Change la taille du graphe en utilisant un nombre de sommet = nouvelleTaille
     * \param[in] nouvelleTaille
     */
	void Graphe::resize(size_t nouvelleTaille){
        this->nbSommets = nouvelleTaille;

        sommets.resize(nouvelleTaille);
        listesAdj.resize(nouvelleTaille);
	}

    /**
     * \fn void Graphe::nommer(size_t sommet, const std::string& nom)
     * \brief Donne un nom à un sommet en utlisant son numéro (indice dans le vector).
      * \exception logic_error si sommet supérieur ou égal à nbSommets
     * \param[in] sommet,nom
     */
	void Graphe::nommer(size_t sommet, const std::string& nom) {
        if (sommet >= nbSommets) {
            throw std::logic_error("Erreur: l'indice du sommet est supérieur ou égal au nombre de sommets.");
        }

        sommets.at(sommet) = nom;
	}

    /**
     * \fn void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout, int ns)
     * \brief Ajoute un arc au graphe
      * \exception logic_error si source ou destination supérieur ou égal à nbSommets
      * \exception logic_error si l'arc existe déjà dans le graphe
     * \param[in] source,destination,duree,cout,ns
     */
	void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout, int ns) {
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("Erreur: l'indice de la source ou de la destination est supérieur ou égal au nombre de sommets.");
        }

        for (const auto& arc : listesAdj.at(source)) {
            if (arc.destination == destination) {
                throw std::logic_error("Erreur: l'arc existe déjà dans le graphe.");
            }
        }

        Ponderations ponderations(duree, cout, ns);

        Arc nouvelArc(destination, ponderations);

        listesAdj.at(source).push_back(nouvelArc);

        nbArcs++;
	}

    /**
     * \fn void Graphe::enleverArc(size_t source, size_t destination)
     * \brief enleve un arc au graphe
      * \exception logic_error si source ou destination supérieur ou égal à nbSommets
     * \param[in] source,destination
     */
	void Graphe::enleverArc(size_t source, size_t destination) {
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("Erreur: l'indice de la source ou de la destination est supérieur ou égal au nombre de sommets.");
        }

        bool arcTrouve = false;

        for (auto it = listesAdj.at(source).begin(); it != listesAdj.at(source).end(); ++it) {
            if (it->destination == destination) {
                listesAdj.at(source).erase(it);
                arcTrouve = true;
                nbArcs--;
                break;
            }
        }

        if (!arcTrouve) {
            throw std::logic_error("Erreur: l'arc n'existe pas dans le graphe.");
        }
	}

    /**
     * \fn bool Graphe::arcExiste(size_t source, size_t destination)
     * \brief Vérifie si un arc existe
      * \exception logic_error si source ou destination supérieur ou égal à nbSommets
     * \param[in] source,destination
     */
	bool Graphe::arcExiste(size_t source, size_t destination) const {
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("Erreur: l'indice de la source ou de la destination est supérieur ou égal au nombre de sommets.");
        }

        for (const auto& arc : listesAdj.at(source)) {
            if (arc.destination == destination) {
                return true;
            }
        }

        return false;
	}

    /**
     * \fn std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet)
     * \brief Retourne la liste de successeurs d'un sommmet
      * \exception logic_error si sommet supérieur ou égal à nbSommets
     * \param[in] sommet
     */
	std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const {
        if (sommet >= nbSommets) {
            throw std::logic_error("Erreur: l'indice du sommet est supérieur ou égal au nombre de sommets.");
        }

        std::vector<size_t> v;

        for (const auto& arc : listesAdj.at(sommet)) {
            v.push_back(arc.destination);
        }

		return v;
	}

    /**
     * \fn std::string Graphe::getNomSommet(size_t sommet)
     * \brief Retourne le nom d'un sommmet
      * \exception logic_error si l'index du sommet supérieur ou égal à nbSommets
     * \param[in] sommet
     */
	std::string Graphe::getNomSommet(size_t sommet) const {
        if (sommet >= nbSommets) {
            throw std::logic_error("Erreur: l'indice du sommet est supérieur ou égal au nombre de sommets.");
        }

        return sommets[sommet];
	}

    /**
     * \fn std::string Graphe::getNomSommet(size_t sommet)
     * \brief Retourne le nom d'un sommmet
      * \exception logic_error si l'index du sommet supérieur ou égal à nbSommets
     * \param[in] sommet
     */
	size_t Graphe::getNumeroSommet(const std::string& nom) const {
        for (size_t i = 0; i < nbSommets; ++i) {
            if (sommets[i] == nom) {
                return i;
            }
        }

        throw std::logic_error("Erreur: le nom du sommet n'existe pas dans le graphe.");
	}

    /**
     * \fn int Graphe::getNombreSommets()
     * \brief Retourne le nombre de sommets
     */
	int Graphe::getNombreSommets() const {
		return nbSommets;
	}

    /**
     * \fn int Graphe::getNombreSommets()
     * \brief Retourne le nombre de d'arcs
     */
	int Graphe::getNombreArcs() const {
		return nbArcs;
	}

    /**
     * \fn Ponderations Graphe::getPonderationsArc(size_t source, size_t destination)
     * \brief Retourne les pondérations se trouvant dans un arc (source -> destination)
     * \exception logic_error si source ou destination supérieur ou égal à nbSommets
     * \exception logic_error si arc inexistant
     * \param[in] source,destination
     */
	Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const {
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("Erreur: l'indice de la source ou de la destination est supérieur ou égal au nombre de sommets.");
        }

        for (const auto& arc : listesAdj.at(source)) {
            if (arc.destination == destination) {
                return arc.poids;
            }
        }

        throw std::logic_error("Erreur: l'arc n'existe pas dans le graphe.");
	}

    /**
     * \fn bool Graphe::sommetExiste(const std::string &nom)
     * \brief Permet de savoir si un sommet existe
     * \param[in] nom
     */
    bool Graphe::sommetExiste(const std::string &nom) const {
        for (const auto& sommet : sommets) {
            if (sommet == nom) {
                return true;
            }
        }
        return false;
    }

}//Fin du namespace
