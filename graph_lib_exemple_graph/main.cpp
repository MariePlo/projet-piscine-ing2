#include "grman/grman.h"
#include <iostream>
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("IMAGES");



    /// Un exemple de graphe

    Graph g;
    std::vector<Vertex*> animal;
    g.lecture_fichier_chaine("chaine_afrique.txt", animal);
    g.lecture_fichier_matrice("matrice_chaine_afrique.txt");

    g.make_example(animal);


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
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


         if(key[KEY_B])
        {
             g.sauvegarder(animal);
             rest(10);
        }
        if(key[KEY_N])
        {
            std::cout<<g.Getmedges().size()<<"   ";
        }
        g.test_mort(animal);



        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


