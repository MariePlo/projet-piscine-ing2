#include "grman/grman.h"
#include <iostream>
#include "graph.h"
#include "allegro.h"

void afficher_menu()
{


    grman::init();
    grman::set_pictures_path("IMAGES");
    BITMAP* fond;
    BITMAP* buffer;
    Graph g;
    std::vector<Vertex*> animal;
    std::vector<Vertex*> animal_2;
    std::vector<Vertex*> animal_3;

    int choix;

    fond=load_bitmap("IMAGES/fond.bmp", NULL);


    buffer=create_bitmap(800,600);

    while(!key[KEY_ESC])
    {
        // affichage du fond
        blit(fond, buffer, 0, 0, 0, 0, 800, 600);
        blit(buffer, screen, 0, 0, 0, 0, 800, 600);



// selection de la chaine alimentaire a visualiser
        if(mouse_b&1 && mouse_x>250 && mouse_x<550 && mouse_y>192 && mouse_y<260)
        {

            choix=1;
        }

        if(mouse_b&1 && mouse_x>250 && mouse_x<550 && mouse_y>291 && mouse_y<356)
        {

            choix=2;
        }


        if(mouse_b&1 && mouse_x>250 && mouse_x<550 && mouse_y>390 && mouse_y<454)
        {

            choix=3;
        }
        switch(choix)
        {

        case 1:
        {
            Graph g;
            g.lecture_fichier_chaine("chaine_afrique.txt", animal);
            g.lecture_fichier_matrice("matrice_chaine_afrique.txt");
            g.make_example(animal, "afrique");
            while(!key[KEY_ESC])
            {
                g.update(animal,"chaine_afrique.txt");
                g.test_mort(animal);
                grman::mettre_a_jour();
                if(key[KEY_N])
                {
                    g.reset();
                    g.lecture_fichier_chaine("chaine_afrique.txt", animal);
                    g.lecture_fichier_matrice("matrice_chaine_afrique.txt");
                    g.make_example(animal, "afrique");
                }

            }

            break;
        }

        case 2:
        {
            Graph g2;
            g2.lecture_fichier_chaine("chaine_arctique.txt", animal_2);
            g2.lecture_fichier_matrice("matrice_chaine_arctique.txt");
            g2.make_example(animal_2,"arctique");
            while(!key[KEY_ESC])
            {
                g2.update(animal_2, "chaine_arctique.txt");
                g2.test_mort(animal_2);
                grman::mettre_a_jour();
            }

            break;
        }
        case 3:
        {
            Graph g3;
            g3.lecture_fichier_chaine("chaine_australie.txt", animal_3);
            g3.lecture_fichier_matrice("matrice_chaine_australie.txt");
            g3.make_example(animal_3,"australie");
            while(!key[KEY_ESC])
            {
                g3.update(animal_3, "chaine_australie.txt");
                g3.test_mort(animal_3);
                grman::mettre_a_jour();
            }

            break;
        }


        default :
            break;




        }



    }

}
