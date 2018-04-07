#include "grman/grman.h"
#include <iostream>
#include "graph.h"
#include "menu.h"

int main()
{

    afficher_menu();
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
   /* grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("IMAGES");



    /// Un exemple de graphe

    Graph g;
    std::vector<Vertex*> animal;
    g.lecture_fichier_chaine("chaine_afrique.txt", animal);
    g.lecture_fichier_matrice("matrice_chaine_afrique.txt");

    g.make_example(animal);


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {

        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();


        if(key[KEY_SPACE])
        {
            std::cout<<" yo ";
            g.test_remove_vertex(6,animal);
            rest(200);
        }

        if(key[KEY_D])
        {
            g.une_journee(animal);
            rest(30);
        }


        if(key[KEY_P])
        {
            g.ajouter_sommet(animal);
            rest(30);
        }
        if (key[KEY_H])
        {
            std::cout<<"voila "<<g.Getmatrice()[4][0];
        }


        if(key[KEY_B])
        {
            g.sauvegarder(animal);
            rest(10);
        }
        if(key[KEY_N])
        {
            std::cout<<g.Getmedges().size()<<"   ";
        }

        if(key[KEY_G])
        {
            for(int i=0; i<g.GetNbSommet(); i++)
            {
                for(int j=0; j<g.GetNbSommet(); j++)
                {
                    std::cout<<"avant?"<<std::endl;
                    std::cout<<g.toutesLesComposantesFortementConnexes()[i][j]<<std::endl;
                    std::cout<<"apres?"<<std::endl;
                    rest(20);
                }
            }

        }

        g.test_mort(animal);



        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

    }

    grman::fermer_allegro();

    return 0;*/
}
END_OF_MAIN();


