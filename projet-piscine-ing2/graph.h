  #ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <fstream>
#include <vector>
#define Nbaretes 10

/**************************************************************
    Ici sont propos�es 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Ar�te ou Arc)
            Graph (=Graphe)

    Les ar�tes et les sommets et le graphe qu'ils constituent
    "travaillent" �troitement ensemble : pour cette raison les
    Vertex et Edge se d�clarent amis (friend) de Graph pour que
    ce dernier puisse librement acc�der aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent �tres compl�t�es. Il est �galement possible
    de les d�river mais il est malheureusement assez difficile
    de d�river le "triplet" des 3 classes en maintenant des relations
    coh�rentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute pr�f�rable, si possible,
    de "customiser" ces classes de base directement, sans h�ritage.

    Le mod�le propos� permet de repr�senter un graphe orient� �ventuellement
    pond�r�, les arcs portent une ou des informations suppl�mentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse �tre assez facilement
    lue et �crite en fichier, et b�n�ficie d'une bonne lisibilit� en cas de bugs...

    Chaque arc poss�de 2 attributs principaux (en plus de son �ventuelle pond�ration)
        -> m_from (indice du sommet de d�part de l'arc )
        -> m_to (indice du sommet d'arriv�e de l'arc )

    Chaque sommet poss�de 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs)
        -> m_out (liste des indices des arcs partant du sommet : acc�s aux successeurs)

    Cependant le probl�me des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entit� (un arc et/ou un sommet sont enlev�s du graphe) alors :

    - Soit il faut reprendre toute la num�rotation pour "boucher le trou"
      (par exemple on a supprim� le sommet n�4, le sommet n�5 devient le 4, 6 devient 5 etc...)
      ce qui pose des probl�mes de stabilit� et de coh�rence, et une difficult� � r�-introduire
      le(s) m�me(s) �l�ment supprim� (au m�me indice)

    - Soit on admet que la num�rotation des sommets et arcs n'est pas contigue, c�d qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La num�rotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet � des indices arbitraires (pas forc�ment contigus)

    C'est cette 2�me approche qui est propos�e ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra �tre attentif au fait que par rapport � un simple vecteur, le parcours des �l�ments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours � it�rateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'int�r�t se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associ�e � chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non repr�sent�s � l'�cran
    Imaginons par exemple qu'on doive g�n�rer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas repr�senter graphiquement ces 1000 graphes, et les
    interfaces p�sent lourd en ressource, avec cette organisation on est libre de r�server ou
    pas une interface de pr�sentation associ�e aux datas (d�couplage donn�es/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "grman/grman.h"


/***************************************************
                    VERTEX
****************************************************/

class VertexInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

        // La boite qui contient toute l'interface d'un sommet
        grman::WidgetBox m_top_box;

        // Un slider de visualisation/modification de la valeur du sommet
        grman::WidgetVSlider m_slider_value;

        // Un label de visualisation de la valeur du sommet
        grman::WidgetText m_label_value;

        // Une image de "remplissage"
        grman::WidgetImage m_img;

        // Un label indiquant l'index du sommet
        grman::WidgetText m_label_idx;

        // Une boite pour le label pr�c�dent
        grman::WidgetText m_box_label_idx;

    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

    private :
        /// liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs
        std::vector<int> m_in;

        /// liste des indices des arcs partant du sommet : acc�s aux successeurs
        std::vector<int> m_out;

        /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
        double m_value;

        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<VertexInterface> m_interface = nullptr;

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne pr�c�dente est en gros �quivalent � la ligne suivante :
        // VertexInterface * m_interface = nullptr;
        ///nom du sommet
        std::string m_name;
        ///nom de l'image correspondante
        std::string m_image;
        ///emplacement du sommet
        int m_posx;
        int m_posy;
        int m_number;
        int m_actif;


    public:

        std::string Getname(){return m_name;}
        void Setname(std::string name){m_name=name;}

        std::string Getimage(){return m_image;}
        void Setimage(std::string image){m_image=image;}

        int Getposx() {return m_posx;}
        void Setposx(int posx) {m_posx=posx;}

        int Getposy() {return m_posy;}
        void Setposy(int posy) {m_posy=posy;}

        int Getnumber() {return m_number;}
        void Setnumber(int number){m_number=number;}

        double Getvalue(){return m_value;}
        void Setvalue(double value){m_value=value;}

        int Getactif() {return m_actif;}
        void Setactif(int actif){m_actif=actif;}

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Vertex (double value=0, VertexInterface *interface=nullptr) :
            m_value(value), m_interface(interface)  {  }

        /// Vertex �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
        /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
        /// Voir l'impl�mentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
   // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Edge;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge;

        // Une boite pour englober les widgets de r�glage associ�s
        grman::WidgetBox m_box_edge;

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight;

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight;

    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        EdgeInterface(Vertex& from, Vertex& to);
};


class Edge
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class EdgeInterface;

    private :
        /// indice du sommet de d�part de l'arc
        int m_from;

        /// indice du sommet d'arriv�e de l'arc
        int m_to;

        /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
        double m_weight;

        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<EdgeInterface> m_interface = nullptr;


    public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Edge (double weight=0, EdgeInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }

        /// Edge �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
        /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
        /// Voir l'impl�mentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

    private :

     grman::WidgetBox m_top_box;
        grman::WidgetBox m_fond;

        /// Dans cette boite seront ajout�s les (interfaces des) sommets et des arcs...
        grman::WidgetBox m_main_box;

        /// Dans cette boite seront ajout�s des boutons de contr�le etc...
        grman::WidgetBox m_tool_box;


         grman::WidgetBox m_boite_boutons;    // Sera la boite � boutons en haut � droite
        grman::WidgetButton m_bouton1;       // Sera le bouton avec le texte NEW
        grman::WidgetText m_bouton1_label;   // Le texte NEW
        grman::WidgetText m_bouton2_label;
        grman::WidgetButton m_bouton2;       // Sera le bouton avec l'image de clown m�chant
        grman::WidgetImage m_bouton2_image;  // L'image de clown m�chant
        grman::WidgetText m_dragme;
        grman::WidgetButton m_bouton3;
         grman::WidgetText m_bouton3_label;
          grman::WidgetImage m_img;
          grman::WidgetButton m_bouton_sauvegarde;
          grman::WidgetButton m_bouton_retour;
          grman::WidgetText m_bouton_sauvegarde_txt;
          grman::WidgetText m_bouton_retour_txt;
          grman::WidgetButton m_bouton_jour;
          grman::WidgetText m_bouton_txt_jour;
           grman::WidgetButton m_bouton_connexe;
          grman::WidgetText m_bouton_txt_connexe;
         // grman::WidgetButton m_bouton_pause;
        grman::WidgetText m_bouton_txt_pause;
        grman::WidgetCheckBox m_bouton_pause;

          /// variables savane



        grman::WidgetImage m_image_savane;            // Sera l'arbre � droite
        grman::WidgetImage m_image_buffle;
        grman::WidgetImage m_image_gnou;
        grman::WidgetImage m_image_hippo;
        grman::WidgetImage m_image_leopard;
        grman::WidgetImage m_image_lion;
        grman::WidgetImage m_image_phacochere;
        grman::WidgetImage m_image_vegetaux;
        grman::WidgetImage m_image_babouin;
        grman::WidgetImage m_image_fourmis;
        grman::WidgetImage m_image_gazelle;
        grman::WidgetImage m_image_guepard;
        grman::WidgetImage m_image_hyene;
        grman::WidgetImage m_image_zebre;

         grman::WidgetButton m_bouton_buffle;
        grman::WidgetButton m_bouton_gnou;
        grman::WidgetButton m_bouton_hippo;
        grman::WidgetButton m_bouton_leopard;
        grman::WidgetButton m_bouton_lion;
        grman::WidgetButton m_bouton_phacochere;
        grman::WidgetButton m_bouton_vegetaux;
        grman::WidgetButton m_bouton_babouin;
        grman::WidgetButton m_bouton_fourmis;
        grman::WidgetButton m_bouton_gazelle;
        grman::WidgetButton m_bouton_guepard;
        grman::WidgetButton m_bouton_hyene;
         grman::WidgetButton m_bouton_zebre;
         grman::WidgetBox m_boite_a_outil;

         /// variable banquise
        grman::WidgetBox m_boite_a_outil_banquise;
        grman::WidgetImage m_image_banquise;
        grman::WidgetImage m_image_krill;
        grman::WidgetImage m_image_lion_mer;
        grman::WidgetImage m_image_macareux;
        grman::WidgetImage m_image_morse;
        grman::WidgetImage m_image_ours;
        grman::WidgetImage m_image_petit_poisson;
        grman::WidgetImage m_image_phoque;
        grman::WidgetImage m_image_pingouin;
        grman::WidgetImage m_image_renard;
        grman::WidgetButton m_bouton_krill;
        grman::WidgetButton m_bouton_lion_mer;
        grman::WidgetButton m_bouton_macareux;
        grman::WidgetButton m_bouton_morse;
        grman::WidgetButton m_bouton_ours;
        grman::WidgetButton m_bouton_petit_poisson;
        grman::WidgetButton m_bouton_phoque;
        grman::WidgetButton m_bouton_pingouin;
        grman::WidgetButton m_bouton_renard;

        /// variable banquise
        grman::WidgetBox m_boite_a_outil_australie;
        grman::WidgetImage m_image_australie;
        grman::WidgetImage m_image_dingo;
        grman::WidgetImage m_image_diable_tasmanie;
        grman::WidgetImage m_image_serpent;
        grman::WidgetImage m_image_echidne;
        grman::WidgetImage m_image_wombat;
        grman::WidgetImage m_image_kangourou;
        grman::WidgetImage m_image_dromadaire;
        grman::WidgetImage m_image_termite;
        grman::WidgetImage m_image_vegetaux_aus;
        grman::WidgetImage m_image_emeu;

        grman::WidgetButton m_bouton_dingo;
        grman::WidgetButton m_bouton_diable_tasmanie;
        grman::WidgetButton m_bouton_serpent;
        grman::WidgetButton m_bouton_echidne;
        grman::WidgetButton m_bouton_wombat;
        grman::WidgetButton m_bouton_kangourou;
        grman::WidgetButton m_bouton_dromadaire;
        grman::WidgetButton m_bouton_termite;
        grman::WidgetButton m_bouton_vegetaux_aus;
        grman::WidgetButton m_bouton_emeu;

    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        GraphInterface(int x, int y, int w, int h, std::string nom);
};


class Graph
{
    private :

        ///nombre de sommets
        int m_NbSommet;
        /// La "liste" des ar�tes
        std::map<int, Edge> m_edges;

        /// La liste des sommets
        std::map<int, Vertex> m_vertices;

        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;
        int ** m_matrice;

        int m_compteur;

        int m_pop;

        std::vector<int> m_color;


    public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="");
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

        /// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
        /// Voir impl�mentation dans le .cpp
        /// Cette m�thode est � enlever et remplacer par un syst�me
        /// de chargement de fichiers par exemple.
        void make_example(std::vector<Vertex*> animal, std::string nom_chaine);
        void lecture_fichier_chaine(std::string nom_fichier, std::vector<Vertex*> &animal);
        void lecture_fichier_matrice(std::string nom_fichier);
        bool allocationMatriceAdjacent();
        void test_remove_vertex(int vidx,std::vector<Vertex*> &animal);
        void test_remove_edge(int eidx);
        void sauvegarder(std::vector<Vertex*> &animal, std::string nom_fichier);
        void une_journee(std::vector<Vertex*> &animal);
        void test_mort(std::vector<Vertex*> &animal);
        void ajouter_sommet(std::vector<Vertex*> &animal);
        int *uneComposanteFortementConnexe (int s);
        int ** toutesLesComposantesFortementConnexes();
        void evolution_pop();
        void reset();


        /// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
        void update(std::vector<Vertex*> &animal, std::string nom);

        int GetNbSommet() {return m_NbSommet;}
        void SetNbSommet(int nbsommet) {m_NbSommet=nbsommet;}

         int GetCompteur() {return m_compteur;}
        void SetCompteur(int compteur) {m_compteur=compteur;}

        int** Getmatrice(){return m_matrice;}
        void Setmatrice(int** matrice){m_matrice=matrice;}


        std::map<int, Edge>Getmedges(){return m_edges;}

};



#endif // GRAPH_H_INCLUDED
