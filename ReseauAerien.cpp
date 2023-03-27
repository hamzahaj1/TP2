/**
 * \file ReseauAerien.cpp
 * \brief Implémentattion de la classe ReseauAerien.
 * \author Hamza Hajjam
 * \version 0.1
 * \date mars 2023
 *
 *  Travail pratique numéro 2
 *
 */

#include "ReseauAerien.h"
#include <sstream>
#include <fstream>
#include <utility>
#include <limits>
#include <set>
#include <algorithm>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    /**
     * \fn ReseauAerien::ReseauAerien(std::string nomReseau, size_t nbVilles)
     * \brief Constructeur
     * \param[in] nomReseau, nbVilles
     */
    ReseauAerien::ReseauAerien(std::string nomReseau, size_t nbVilles) {
        this->nomReseau = std::move(nomReseau);
        this->unReseau = Graphe(nbVilles);
    }

    /**
     * \fn ReseauAerien::~ReseauAerien()
     * \brief Deconstructeur
     */
    ReseauAerien::~ReseauAerien() {
    }

    /**
     * \fn void ReseauAerien::resize(size_t nouvelleTaille)
     * \brief Change la taille du réseau en utilisant un nombre de villes = nouvelleTaille
     * \param[in] nouvelleTaille
     */
    void ReseauAerien::resize(size_t nouvelleTaille) {
        unReseau.resize(nouvelleTaille);
    }

    // Méthode fournie
    /**
     * \fn void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
     * \brief Permet de charger le réseau à partir d'un fichier texte
     * \param[in] le fichier contenant l'information sur le réseau
     * \pre fichierEntree a déjà été ouvert
     * \post Le réseau a été chargé
     * \exception logic_error si le fichier n'est pas déjà ouvert 
     */
    void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
    {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauAerien::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;
        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 15);

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while(buff != "Liste des trajets:")
        {
            std::string ville = buff;

            unReseau.nommer(i, ville);

            getline(fichierEntree, buff);
            i++;
        }

        while(!fichierEntree.eof())
        {
            getline(fichierEntree, buff);
            std::string source = buff;

            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            int ns;
            iss >> ns;
            if(!source.empty() && !destination.empty()){
                unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout, ns);
            }
        }
    }

    /**
     * \fn Chemin ReseauAerien::rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout)
     * \brief Retourne le plus court chemin selon l'algorithme de Dijkstra
     * \param[in] origine, destination du voyage ainsi qu'un param boolean dureeCout
     * \exception logic_error si origine et/ou destination absent du réseau
     * \return chemin le plus court avec l'algorithme de Dijkstra
     */
    Chemin ReseauAerien::rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout) const
    {
        if (!unReseau.sommetExiste(origine) || !unReseau.sommetExiste(destination)) {
            throw std::logic_error("Erreur: Origine ou destination non présentes dans le réseau aérien.");
        }
        Chemin chemin;

        // Initialise les structures de données nécessaires.
        const float MAX = std::numeric_limits<float>::max();
        std::vector<float> d(unReseau.getNombreSommets(), MAX);
        std::vector<float> p(unReseau.getNombreSommets(), -1);

        d[unReseau.getNumeroSommet(origine)] = 0;

        // Initialise les ensembles T et Q.
        std::set<int> T;
        std::set<int> Q;

        for (auto i = 0; i < unReseau.getNombreSommets(); ++i) {
            Q.insert(i);
        }

        for (size_t i = 0; i < unReseau.getNombreSommets(); ++i) {
            // Trouve le sommet avec la distance minimale dans Q.
            int u_min = -1;
            float min_distance = MAX;

            for (int u : Q) {
                if (d[u] < min_distance) {
                    min_distance = d[u];
                    u_min = u;
                }
            }

            // Si aucune distance minimale n'est trouvée, termine l'algorithme.
            if (min_distance == MAX) {
                break;
            }

            // Met à jour les ensembles T et Q.
            Q.erase(u_min);
            T.insert(u_min);

            // Parcourt les voisins de u_min et met à jour les distances.
            for (int u : Q) {
                if (unReseau.arcExiste(u_min, u)) {
                    float temp = d[u_min] + (dureeCout ? unReseau.getPonderationsArc(u_min, u).duree : unReseau.getPonderationsArc(u_min, u).cout);

                    // Si la nouvelle distance est plus courte, met à jour les distances et les prédécesseurs.
                    if (temp < d[u]) {
                        d[u] = temp;
                        p[u] = u_min;
                    }
                }
            }
        }

        chemin.reussi = false;

        // Vérifie si un chemin existe et reconstruit le chemin à partir des prédécesseurs.
        if (d[unReseau.getNumeroSommet(destination)] != MAX) {
            chemin.reussi = true;
            chemin.listeVilles.push_back(destination);

            int current = unReseau.getNumeroSommet(destination);
            while (p[current] != -1) {
                current = p[current];
                chemin.listeVilles.push_back(unReseau.getNomSommet(current));
            }

            std::reverse(chemin.listeVilles.begin(), chemin.listeVilles.end());

            if (dureeCout) {
                chemin.dureeTotale = d[unReseau.getNumeroSommet(destination)];
            } else {
                chemin.coutTotal = d[unReseau.getNumeroSommet(destination)];
            }
        }
        return chemin;
    }

    /**
     * \fn Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &origine, const std::string &destination, int dureeCoutNiveau)
     * \brief Retourne le plus court chemin selon l'algorithme de BellManFord
     * \param[in] origine, destination du voyage ainsi qu'un param boolean dureeCout
     * \exception logic_error si origine et/ou destination absent du réseau
     * \exception logic_error si dureeCoutNiveau hors limite
     * \return chemin le plus court avec l'algorithme de BellManFord
     */
    Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &origine, const std::string &destination, int dureeCoutNiveau) const
    {
        // Vérifie si les sommets existent et si dureeCoutNiveau est valide.
        if (!unReseau.sommetExiste(origine) || !unReseau.sommetExiste(destination)) {
            throw std::logic_error("Erreur: Origine ou destination non présentes dans le réseau aérien.");
        }
        if (dureeCoutNiveau < 1 || dureeCoutNiveau > 3) {
            throw std::logic_error("Erreur: dureeCoutNiveau invalide.");
        }

        // Initialise les vecteurs d et p.
        const float MAX = std::numeric_limits<float>::max();
        size_t indDestination = unReseau.getNumeroSommet(destination);
        std::vector<float> d(unReseau.getNombreSommets(), MAX);
        std::vector<float> p(unReseau.getNombreSommets(), -1);
        d[unReseau.getNumeroSommet(origine)] = 0;

        // algorithme de Bellman-Ford.
        for (size_t i = 0; i < unReseau.getNombreSommets() - 1; ++i) {
            // Parcours de tous les arcs du graphe.
            for (size_t u = 0; u < unReseau.getNombreSommets(); ++u) {
                for (size_t v = 0; v < unReseau.getNombreSommets(); ++v) {
                    // Si un arc existe entre u et v, effectue un relâchement.
                    if (unReseau.arcExiste(u, v)) {
                        float poids;
                        if (dureeCoutNiveau == 1) {
                            poids = unReseau.getPonderationsArc(u, v).duree;
                        } else if (dureeCoutNiveau == 2) {
                            poids = unReseau.getPonderationsArc(u, v).cout;
                        } else {
                            poids = unReseau.getPonderationsArc(u, v).ns;
                        }
                        // Tentative de relâchement de l'arc (u, v).
                        float temp = d[u] + poids;
                        if (temp < d[v]) {
                            d[v] = temp;
                            p[v] = u;
                        }
                    }
                }
            }
        }

        // Vérifie l'existence d'un cycle de poids négatif.
        for (size_t u = 0; u < unReseau.getNombreSommets(); ++u) {
            for (size_t v = 0; v < unReseau.getNombreSommets(); ++v) {
                // Si un arc existe entre u et v, vérifie si un cycle de poids négatif est détecté.
                if (unReseau.arcExiste(u, v)) {
                    float ponderations;
                    if (dureeCoutNiveau == 1) {
                        ponderations = unReseau.getPonderationsArc(u, v).duree;
                    } else if (dureeCoutNiveau == 2) {
                        ponderations = unReseau.getPonderationsArc(u, v).cout;
                    } else {
                        ponderations = unReseau.getPonderationsArc(u, v).ns;
                    }
                    // Vérification de l'existence d'un cycle de poids négatif.
                    if (d[v] > d[u] + ponderations) {
                        Chemin chemin;
                        chemin.reussi = false;
                        return chemin;
                    }
                }
            }
        }

        Chemin chemin;
        chemin.reussi = false;

        // Reconstruit le chemin le plus court et le retourne.
        if (d[indDestination] != MAX) {
            chemin.reussi = true;
            chemin.listeVilles.push_back(destination);

            int current = indDestination;
            while (p[current] != -1) {
                current = p[current];
                chemin.listeVilles.push_back(unReseau.getNomSommet(current));
            }

            std::reverse(chemin.listeVilles.begin(), chemin.listeVilles.end());

            if (dureeCoutNiveau == 1) {
                chemin.dureeTotale = d[indDestination];
            } else if (dureeCoutNiveau == 2) {
                chemin.coutTotal = d[indDestination];
            } else {
                chemin.nsTotal = d[indDestination];
            }
        }

        return chemin;
    }

}//Fin du namespace
