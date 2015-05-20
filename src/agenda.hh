/**
 * \file agenda.hh
 * \brief Classe Agenda
 */

#ifndef AGENDA_HH
#define AGENDA_HH

/// \cond HIDE
#include <map>
#include <string>
#include <sstream>
//#include <vector> TODO si necessitem el vector menu
#include <list>
#include <iostream>
/// \endcond
#include "tasca.hh"
#include "data.hh"
using namespace std;

/** \class Agenda
 *  \brief Representa una agenda amb un conjunt de tasques amb etiquetes
    \invariant 
    - No hi han tasques amb la mateixa data
    - No es poden modificar les tasques del passat
     
    \par Notació
    -# Una data és "del passat" o "passada" si és anterior al rellotge del p.i.
    -# Una tasca és "del passat" si la seva data associada ho és.
 */
class Agenda {
    private:
        /** \struct Ordre Instant
         *  \brief Defineix l'ordre entre iteradors a tasques */
        struct ordre_instant {
            /** \brief Funció d'ordre dels instants
             *  \param[in] a lhs
             *  \param[in] b rhs
             *  \pre a i b són dereferenciables
             *  \post retorn si l'element apuntat per a és anterior a l'apuntat
             *  per b */
            bool operator()(const instant& a, const instant& b) const;
        };
        typedef set<instant, ordre_instant> set_instant;
        typedef map<string, set_instant> tag_set;

        pair<Data, instant> clock_;
        map<Data, Tasca> tasks_;
        map<string, set_instant> tags_;
        list<instant> menu_;

        /** \brief Escriu una línia del menú
         *  \param[in] i id de la línia
         *  \pre true
         *  \post s'ha escrit una línia en format:
         *  i titol data etiquetes */
        void print_menu_item(int i, const cinstant& it) const;

        /** \brief Escriu el menú
         *  \pre true
         *  \post s'ha escrit el menú del p.i */
        void print_menu() const;

        bool p_set_data(list<instant>::iterator& it, Data data);

        /** \brief Afegeix una tasca (private)
         *  \param[in] data la data de la tasca a afegir
         *  \param[in] tasca la tasca a afegir
         *  \return iterator a la tasca insertada i bool si s'ha pogut inserir
         *  \pre  no is_passat(data)
         *  \post si return.second, el p.i conté la tasca t i return.first és un
         *  instant que hi apunta */
        pair<instant, bool> p_add_tasca(const Data& data, const Tasca& t);

        /** \brief Fusiona 2 rangs de tasques fent la intersecció
         *  \param[in] in1 inici del 1r rang
         *  \param[in] in2 final del 1r rang
         *  \param[in][out] l 2n rang i contenedor de la intersecció
         *  \pre in1 i in2 són iteradors a contenidors d'instants
         *  \post l conté la intersecció de [in1, in2) i l */
        template<typename Iterator>
        void merge_and(Iterator in1, Iterator in2, list<instant>& l);

        /** \brief Fusiona 2 rangs de tasques fent la unió
         *  \param[in] in1 inici del 1r rang
         *  \param[in] in2 final del 1r rang
         *  \param[in][out] l 2n rang i contenedor de la unió
         *  \pre in1 i in2 són iteradors a contenidors d'instants
         *  \post l conté la unió de [in1, in2) i l */
        template<typename Iterator>
        void merge_or(Iterator in1, Iterator in2, list<instant>& l);

        /** \brief Fa la búsqueda i escriu el menú d'un rang
         *  \param[in] in1 inici del rang
         *  \param[in] in2 final del rang
         *  \pre in1 apunta a una tasca anterior a la de in2, i les dues no passades
         *  menú està buit
         *  \post el menú conté les tasques de [in1, in2) i s'ha mostrat el menú */
        void menu_directe(instant& in1, instant& in2);

        /** \brief Fa la búsqueda i escriu el menú d'un rang
         *  \param[in] in1 inici del rang
         *  \param[in] in2 final del rang
         *  \pre in1, in2 apunten a instants de tasques no passades i in1 anterior a in2
         *  menú està buit
         *  \post el menú conté les tasques de [*in1, *in2) i s'ha mostrat el menú */
        void menu_directe(set_instant::iterator& in1, set_instant::iterator& in2);

        void extract_tag(istringstream& exp, string& tag);

        set_instant::iterator safe_bound(tag_set::iterator& tag, const instant& in);

        /** \brief Avalua una expressió parentitzada i guarda el resultat
         *  \param[in] in1 inici del rang
         *  \param[in] r2 final del rang
         *  \param[in] exp flux de l'expressió parentitzada
         *  \param[out][in] llista on es guarda el resultat
         *  \pre in1 i in2 apuntent a tasques no passades i in1 anterior a in2
         *  \post l conté les tasques del rang que compleixen l'expressió  */
        void exp_parentitzada(const instant& in1, const instant& in2, istringstream& exp, list<instant>& l);

        /** \brief Indica si es pot modificar la tasca #id del menu
         * \param[in] id número de la tasca en el menu
         * \pre true
         * \post si es pot modificar retorna un iterador del menu i true
         *       si no retorna false */
        pair<list<instant>::iterator, bool> menu(const int id);
    public:
        /** \brief Constructor d'una agenda per defecte
         *  \pre true
         *  \post el p.i és una agenda buida amb rellotge
         */
        Agenda();

        //Modificadores
        /** \brief Avança el rellotge
         *  \param[in] data data fins on avançar
         *  \pre no es_passat(data)
         *  \post si el rellotge del p.i marca \e data */
        void set_rellotge(Data data);

        /** \brief Afegeix una tasca
         *  \param[in] data la data de la tasca a afegir
         *  \param[in] tasca la tasca a afegir
         *  \return si s'ha pogut afegir la tasca
         *  \pre  no is_passat(data)
         *  \post si retorna true el p.i conté la tasca t */
        bool add_tasca(const Data &data, const Tasca& t);

        /** \brief Canvia el títol d'una tasca del menú
         *  \param[in] id nº de la tasca al menú
         *  \param[in] titol nou títol de la tasca
         *  \pre true
         *  \post si retorna true, la tasca \e id del menú té com a títol \e titol */
        bool set_titol(const int id, string titol);

        /** \brief Canvia la data d'una tasca del menú
         *  \param[in] id nº de la tasca al menú
         *  \param[in] data nova data de la tasca
         *  \pre  no existeix(data)
         *  \post si retorna true, la tasca \e id del menú té com a data \e data */
        bool set_dia(const int id, Dia d);
        bool set_hora(const int id, Hora d);
        bool set_data(const int id, Data d);

        /** \brief Afegeix una etiqueda a una tasca del menú
         *  \param[in] id nº de la tasca al menú
         *  \param[in] etiqueta etiqueta a afegir
         *  \pre  true
         *  \post si retorna true, la tasca \e id del menú té l'etiqueta \e etiqueta */
        bool add_etiqueta(const int id, string etiqueta);

        /** \brief Esborra una etiqueda d'una tasca del menú
         *  \param[in] id nº de la tasca al menú
         *  \param[in] etiqueta etiqueta a esborrar
         *  \pre  true
         *  \post si retorna true, la tasca \e id no té l'etiqueta \e etiqueta
         */
        bool del_etiqueta(const int id, const string etiqueta);

        /** \brief Esborra totes les etiquedes d'una tasca del menú
         *  \param[in] id nº de la tasca al menú
         *  \pre true
         *  \post si retorna true, la tasca \e id del menú no té cap etiqueta
         */
        bool del_etiquetes(const int id);

        /** \brief Esborra una tasca del menú
         *  \param[in] id nº de la tasca al menú
         *  \pre true
         *  \post si retorna true, el p.i no conté la tasca \e id del menú */
        bool del_tasca(const int id);

        // Consultores
        /** \brief Consulta si una data és passada
         *  \param[in] data la data a evaluar
         *  \pre true
         *  \post retorna (data anterior al rellotge del p.i) */
        bool es_passat(Data data) const;

        /** \brief  Obté el dia del rellotge
         *  \pre true
         *  \post retorna el dia del rellotge del p.i */
        Dia get_dia() const;

        /** \brief  Obté la hora del rellotge
         *  \pre true
         *  \post retorna el rellotge del rellotge del p.i */
        Hora get_hora() const;

        /** \brief Genera el menu per busqueda en rang i expressió
         *  \param[in] dia1 cota temporal inferior de la búsqueda
         *  \param[in] dia2 cota temporal superior de la búsqueda
         *  \param[in] expressio expressió booleana sobre les etiquetes
         *  \pre true
         *  \post el menú conté les tasques amb data en [\e data1, \e data2] no passades
         *  amb un conjunt d'etiquetes que compleix \e expressio i es mostra el menú */
        void consulta(Dia dia1, Dia dia2, string expressio = "");

        /** \brief Genera el menu corresponen a la búsqueda en un dia
         *  \param[in] dia dia en què s'ha de buscar
         *  \param[in] expressio expressió booleana sobre les etiquetes
         *  \pre true
         *  \post el menú conté les tasques no passades amb dia \e dia i un un conjunt
         *  d'etiquetes que compleix \e expressio i es mostra el menú */
        void consulta(Dia dia, string expressio = "");

        /** \brief Genera el menu de les tasques que compleixen expressió
         *  \pre true
         *  \post el menú conté les tasques no passades amb un conjunt d'etiquetes que
         *  compleix expressio i es mostra el menú */
        void consulta(string expressio = "");

        // Escriptura
        /** \brief Escriu totes les tasques del passat
         * \pre true
         * \post es mostren totes les tasques del passat */
        void passat();

        void print_llista(const list<instant>& l);
        void print_map_data_tasca();
        void print_map_tags();
};
#endif
