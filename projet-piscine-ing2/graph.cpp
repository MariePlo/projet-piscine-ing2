#include "graph.h"
#include "menu.h"




/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}

/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h, std::string nom)
{
    m_top_box.set_frame(0,0,800,600);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_frame(0,0,100,600);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_frame(100,0,700,600);
    m_main_box.set_bg_color(BLANCJAUNE);

    /// bouton sauvegarde
    m_main_box.add_child( m_bouton_sauvegarde );
    m_bouton_sauvegarde.set_frame(0,0,100,50);
    m_bouton_sauvegarde.add_child(m_bouton_sauvegarde_txt);
    m_bouton_sauvegarde.set_bg_color(BLANC);
    m_bouton_sauvegarde_txt.set_message("SAUVEGARDE");

    /// bouton retour
    m_main_box.add_child( m_bouton_retour );
    m_bouton_retour.set_frame(150,0,100,50);
    m_bouton_retour.add_child(m_bouton_retour_txt);
    m_bouton_retour.set_bg_color(BLANC);
    m_bouton_retour_txt.set_message("RETOUR");

    /// bouton avancer jour

    m_main_box.add_child( m_bouton_jour );
    m_bouton_jour.set_frame(300,0,100,50);
    m_bouton_jour.add_child(m_bouton_txt_jour);
    m_bouton_jour.set_bg_color(BLANC);
    m_bouton_txt_jour.set_message("+1 JOUR");

    ///bouton graphe connexe
    m_main_box.add_child( m_bouton_connexe );
    m_bouton_connexe.set_frame(450,0,100,50);
    m_bouton_connexe.add_child(m_bouton_txt_connexe);
    m_bouton_connexe.set_bg_color(BLANC);
    m_bouton_txt_connexe.set_message("CONNEXE");

    ///bouton pause
    m_main_box.add_child( m_bouton_pause );
    m_bouton_pause.set_frame(600,0,100,50);
    m_bouton_pause.add_child(m_bouton_txt_pause);
    m_bouton_pause.set_bg_color(BLANC);
    m_bouton_txt_pause.set_message("PAUSE");



    /// boite a outil afrique

    if (nom=="afrique")
    {
        m_top_box.add_child( m_boite_a_outil );
        m_boite_a_outil.set_frame(0,0,100,600);
        m_boite_a_outil.add_child(m_image_savane);
        m_image_savane.set_pic_name("menu_savane.bmp");

        m_boite_a_outil.add_child(m_bouton_buffle);
        m_bouton_buffle.set_frame(25,10,50,40);
        m_bouton_buffle.add_child(m_image_buffle);
        m_image_buffle.set_pic_name("buffle_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_gnou);
        m_bouton_gnou.set_frame(25,60,50,40);
        m_bouton_gnou.add_child(m_image_gnou);
        m_image_gnou.set_pic_name("gnou_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_lion);
        m_bouton_lion.set_frame(25,110,50,40);
        m_bouton_lion.add_child(m_image_lion);
        m_image_lion.set_pic_name("lion_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_phacochere);
        m_bouton_phacochere.set_frame(25,160,50,40);
        m_bouton_phacochere.add_child(m_image_phacochere);
        m_image_phacochere.set_pic_name("phacochere_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_leopard);
        m_bouton_leopard.set_frame(25,210,50,40);
        m_bouton_leopard.add_child(m_image_leopard);
        m_image_leopard.set_pic_name("leopard_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_hippo);
        m_bouton_hippo.set_frame(25,260,50,40);
        m_bouton_hippo.add_child(m_image_hippo);
        m_image_hippo.set_pic_name("hippopotame_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_vegetaux);
        m_bouton_vegetaux.set_frame(25,310,50,40);
        m_bouton_vegetaux.add_child(m_image_vegetaux);
        m_image_vegetaux.set_pic_name("vegetaux_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_babouin);
        m_bouton_babouin.set_frame(25,360,50,40);
        m_bouton_babouin.add_child(m_image_babouin);
        m_image_babouin.set_pic_name("babouin_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_hyene);
        m_bouton_hyene.set_frame(25,410,50,40);
        m_bouton_hyene.add_child(m_image_hyene);
        m_image_hyene.set_pic_name("hyene_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_zebre);
        m_bouton_zebre.set_frame(25,460,50,40);
        m_bouton_zebre.add_child(m_image_zebre);
        m_image_zebre.set_pic_name("zebre_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_guepard);
        m_bouton_guepard.set_frame(25,510,50,40);
        m_bouton_guepard.add_child(m_image_guepard);
        m_image_guepard.set_pic_name("guepard_menu.bmp");

        m_boite_a_outil.add_child(m_bouton_fourmis);
        m_bouton_fourmis.set_frame(25,560,50,40);
        m_bouton_fourmis.add_child(m_image_fourmis);
        m_image_fourmis.set_pic_name("fourmis_menu.bmp");
    }


    /// boite a outil arctique
    if(nom=="arctique")
    {
        m_top_box.add_child( m_boite_a_outil_banquise );
        m_boite_a_outil_banquise.set_frame(0,0,100,600);
        m_boite_a_outil_banquise.add_child(m_image_banquise);
        m_image_banquise.set_pic_name("banquise.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_renard);
        m_bouton_renard.set_frame(25,10,50,40);
        m_bouton_renard.add_child(m_image_renard);
        m_image_renard.set_pic_name("renard_menu.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_krill);
        m_bouton_krill.set_frame(25,60,50,40);
        m_bouton_krill.add_child(m_image_krill);
        m_image_krill.set_pic_name("krill_menu.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_lion_mer);
        m_bouton_lion_mer.set_frame(25,110,50,40);
        m_bouton_lion_mer.add_child(m_image_lion_mer);
        m_image_lion_mer.set_pic_name("lion_mer_menu.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_macareux);
        m_bouton_macareux.set_frame(25,160,50,40);
        m_bouton_macareux.add_child(m_image_macareux);
        m_image_macareux.set_pic_name("macareux_menu.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_morse);
        m_bouton_morse.set_frame(25,210,50,40);
        m_bouton_morse.add_child(m_image_morse);
        m_image_morse.set_pic_name("morse_menu.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_petit_poisson);
        m_bouton_petit_poisson.set_frame(25,260,50,40);
        m_bouton_petit_poisson.add_child(m_image_petit_poisson);
        m_image_petit_poisson.set_pic_name("petit_poisson_menu.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_phoque);
        m_bouton_phoque.set_frame(25,310,50,40);
        m_bouton_phoque.add_child(m_image_phoque);
        m_image_phoque.set_pic_name("phoque_menu.bmp");


        m_boite_a_outil_banquise.add_child(m_bouton_pingouin);
        m_bouton_pingouin.set_frame(25,360,50,40);
        m_bouton_pingouin.add_child(m_image_pingouin);
        m_image_pingouin.set_pic_name("pingouin_menu.bmp");

        m_boite_a_outil_banquise.add_child(m_bouton_ours);
        m_bouton_ours.set_frame(25,410,50,40);
        m_bouton_ours.add_child(m_image_ours);
        m_image_ours.set_pic_name("ours_menu.bmp");

    }

    /// boite a outil australie

    if (nom=="australie")
    {
        m_top_box.add_child( m_boite_a_outil_australie );
        m_boite_a_outil_australie.set_frame(0,0,100,600);
        m_boite_a_outil_australie.add_child(m_image_australie);
        m_image_australie.set_pic_name("australie.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_dingo);
        m_bouton_dingo.set_frame(25,10,50,40);
        m_bouton_dingo.add_child(m_image_dingo);
        m_image_dingo.set_pic_name("dingo_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_diable_tasmanie);
        m_bouton_diable_tasmanie.set_frame(25,60,50,40);
        m_bouton_diable_tasmanie.add_child(m_image_diable_tasmanie);
        m_image_diable_tasmanie.set_pic_name("diable_tasmanie_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_serpent);
        m_bouton_serpent.set_frame(25,110,50,40);
        m_bouton_serpent.add_child(m_image_serpent);
        m_image_serpent.set_pic_name("serpent_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_echidne);
        m_bouton_echidne.set_frame(25,160,50,40);
        m_bouton_echidne.add_child(m_image_echidne);
        m_image_echidne.set_pic_name("echidne_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_wombat);
        m_bouton_wombat.set_frame(25,210,50,40);
        m_bouton_wombat.add_child(m_image_wombat);
        m_image_wombat.set_pic_name("wombat_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_kangourou);
        m_bouton_kangourou.set_frame(25,260,50,40);
        m_bouton_kangourou.add_child(m_image_kangourou);
        m_image_kangourou.set_pic_name("kangourou_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_dromadaire);
        m_bouton_dromadaire.set_frame(25,310,50,40);
        m_bouton_dromadaire.add_child(m_image_dromadaire);
        m_image_dromadaire.set_pic_name("dromadaire_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_termite);
        m_bouton_termite.set_frame(25,360,50,40);
        m_bouton_termite.add_child(m_image_termite);
        m_image_termite.set_pic_name("termite_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_vegetaux_aus);
        m_bouton_vegetaux_aus.set_frame(25,410,50,40);
        m_bouton_vegetaux_aus.add_child(m_image_vegetaux_aus);
        m_image_vegetaux_aus.set_pic_name("vegetaux_aus_menu.bmp");

        m_boite_a_outil_australie.add_child(m_bouton_emeu);
        m_bouton_emeu.set_frame(25,460,50,40);
        m_bouton_emeu.add_child(m_image_emeu);
        m_image_emeu.set_pic_name("emeu_menu.bmp");

    }
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.

void Graph::make_example(std::vector<Vertex*> animal, std::string nom_chaine)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600, nom_chaine);

    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...


    for(int i=0; i<GetNbSommet(); i++)
    {
        //std::cout << animal[i]->Getnumber() <<" "<< animal[i]->Getvalue() <<" "<< animal[i]->Getposx() <<" "<< animal[i]->Getposy() <<" "<< animal[i]->Getimage() <<" "<< std::endl;
        if(animal[i]->Getactif()!=0)
        {
            std::cout<< " oui  \n";
            add_interfaced_vertex(animal[i]->Getnumber(),animal[i]->Getvalue(), animal[i]->Getposx(), animal[i]->Getposy(), animal[i]->Getimage());
        }

    }


    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    int compteur=0;

    for (int i=0; i<GetNbSommet(); i++)
    {
        for (int y=0; y<GetNbSommet(); y++)
        {
            if(m_matrice[i][y]==1 && animal[i]->Getactif()!=0 && animal[y]->Getactif()!=0)
            {
                compteur++;
                add_interfaced_edge(compteur, i, y, 50);
            }
        }
    }

    SetCompteur(Getmedges().size());

   // m_pop=1;


    //std::cout<<"/n/n/n"<<compteur<<std::endl;

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update(std::vector<Vertex*> &animal, std::string nom_fichier)
{
    /// bouton de l'interface generale

    //7std::cout<<m_pop<<std::endl;

    if(m_interface->m_bouton_pause.get_value()==0)
    {
        evolution_pop();

    }

    if(m_interface->m_bouton_retour.clicked())
    {
        afficher_menu();
    }

    if(m_interface->m_bouton_sauvegarde.clicked())
    {
        sauvegarder(animal,nom_fichier);
    }
    if(m_interface->m_bouton_jour.clicked())
    {
        une_journee(animal);

    }
    if(m_interface->m_bouton_connexe.clicked())
    {
        for(int i=0; i<GetNbSommet(); i++)
        {
            for (int j=0; j<GetNbSommet(); j++)
            {
                std::cout<<toutesLesComposantesFortementConnexes()[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }


    /// boutons pour ajout et retirer les animaux
    if(m_interface->m_bouton_fourmis.clicked() && animal[8]->Getactif()!=0)
    {
        test_remove_vertex(8,animal);

    }
    else if(m_interface->m_bouton_fourmis.clicked() && animal[8]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_lion.clicked() && animal[0]->Getactif()!=0)
    {
        test_remove_vertex(0,animal);

    }
    else if(m_interface->m_bouton_lion.clicked() && animal[0]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }

    if(m_interface->m_bouton_leopard.clicked() && animal[1]->Getactif()!=0)
    {
        test_remove_vertex(1,animal);

    }
    else if(m_interface->m_bouton_leopard.clicked() && animal[1]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_guepard.clicked() && animal[2]->Getactif()!=0)
    {
        test_remove_vertex(2,animal);

    }
    else if(m_interface->m_bouton_guepard.clicked() && animal[2]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_hyene.clicked() && animal[3]->Getactif()!=0)
    {
        test_remove_vertex(3,animal);

    }
    else if(m_interface->m_bouton_hyene.clicked() && animal[3]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_buffle.clicked() && animal[4]->Getactif()!=0)
    {
        test_remove_vertex(4,animal);

    }
    else if(m_interface->m_bouton_buffle.clicked() && animal[4]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_gnou.clicked() && animal[5]->Getactif()!=0)
    {
        test_remove_vertex(5,animal);

    }
    else if(m_interface->m_bouton_gnou.clicked() && animal[5]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_phacochere.clicked() && animal[6]->Getactif()!=0)
    {
        test_remove_vertex(6,animal);

    }
    else if(m_interface->m_bouton_phacochere.clicked() && animal[6]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_zebre.clicked() && animal[7]->Getactif()!=0)
    {
        test_remove_vertex(7,animal);

    }
    else if(m_interface->m_bouton_zebre.clicked() && animal[7]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_hippo.clicked() && animal[9]->Getactif()!=0)
    {
        test_remove_vertex(9,animal);

    }
    else if(m_interface->m_bouton_hippo.clicked() && animal[9]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_vegetaux.clicked() && animal[10]->Getactif()!=0)
    {
        test_remove_vertex(10,animal);

    }
    else if(m_interface->m_bouton_vegetaux.clicked() && animal[10]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_babouin.clicked() && animal[11]->Getactif()!=0)
    {
        test_remove_vertex(11,animal);

    }
    else if(m_interface->m_bouton_babouin.clicked() && animal[11]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_macareux.clicked() && animal[0]->Getactif()!=0)
    {
        test_remove_vertex(0,animal);

    }
    else if(m_interface->m_bouton_macareux.clicked() && animal[0]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_renard.clicked() && animal[1]->Getactif()!=0)
    {
        test_remove_vertex(1,animal);

    }
    else if(m_interface->m_bouton_renard.clicked() && animal[1]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_ours.clicked() && animal[2]->Getactif()!=0)
    {
        test_remove_vertex(2,animal);

    }
    else if(m_interface->m_bouton_ours.clicked() && animal[2]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_pingouin.clicked() && animal[3]->Getactif()!=0)
    {
        test_remove_vertex(3,animal);

    }
    else if(m_interface->m_bouton_pingouin.clicked() && animal[3]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_phoque.clicked() && animal[4]->Getactif()!=0)
    {
        test_remove_vertex(4,animal);

    }
    else if(m_interface->m_bouton_phoque.clicked() && animal[4]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_lion_mer.clicked() && animal[5]->Getactif()!=0)
    {
        test_remove_vertex(5,animal);

    }
    else if(m_interface->m_bouton_lion_mer.clicked() && animal[5]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_krill.clicked() && animal[6]->Getactif()!=0)
    {
        test_remove_vertex(6,animal);

    }
    else if(m_interface->m_bouton_krill.clicked() && animal[6]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_petit_poisson.clicked() && animal[7]->Getactif()!=0)
    {
        test_remove_vertex(7,animal);

    }
    else if(m_interface->m_bouton_petit_poisson.clicked() && animal[7]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_morse.clicked() && animal[8]->Getactif()!=0)
    {
        test_remove_vertex(8,animal);

    }
    else if(m_interface->m_bouton_morse.clicked() && animal[8]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_dingo.clicked() && animal[0]->Getactif()!=0)
    {
        test_remove_vertex(0,animal);

    }
    else if(m_interface->m_bouton_dingo.clicked() && animal[0]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_diable_tasmanie.clicked() && animal[1]->Getactif()!=0)
    {
        test_remove_vertex(1,animal);

    }
    else if(m_interface->m_bouton_diable_tasmanie.clicked() && animal[1]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_serpent.clicked() && animal[2]->Getactif()!=0)
    {
        test_remove_vertex(2,animal);

    }
    else if(m_interface->m_bouton_serpent.clicked() && animal[2]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_echidne.clicked() && animal[3]->Getactif()!=0)
    {
        test_remove_vertex(3,animal);

    }
    else if(m_interface->m_bouton_echidne.clicked() && animal[3]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_wombat.clicked() && animal[4]->Getactif()!=0)
    {
        test_remove_vertex(4,animal);

    }
    else if(m_interface->m_bouton_wombat.clicked() && animal[4]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_kangourou.clicked() && animal[5]->Getactif()!=0)
    {
        test_remove_vertex(5,animal);

    }
    else if(m_interface->m_bouton_kangourou.clicked() && animal[5]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_dromadaire.clicked() && animal[6]->Getactif()!=0)
    {
        test_remove_vertex(6,animal);

    }
    else if(m_interface->m_bouton_dromadaire.clicked() && animal[6]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_termite.clicked() && animal[7]->Getactif()!=0)
    {
        test_remove_vertex(7,animal);

    }
    else if(m_interface->m_bouton_termite.clicked() && animal[7]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_vegetaux_aus.clicked() && animal[8]->Getactif()!=0)
    {
        test_remove_vertex(8,animal);

    }
    else if(m_interface->m_bouton_vegetaux_aus.clicked() && animal[8]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }
    if(m_interface->m_bouton_emeu.clicked() && animal[9]->Getactif()!=0)
    {
        test_remove_vertex(9,animal);

    }
    else if(m_interface->m_bouton_emeu.clicked() && animal[9]->Getactif()==0)
    {
        ajouter_sommet(animal);
    }






    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();


    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}


/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name)
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }

    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name);

    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);

    /* m_vertices[id_vert1].m_out.push_back(idx);
     m_vertices[id_vert2].m_in.push_back(idx);*/
}

void Graph::lecture_fichier_chaine(std::string nom_fichier, std::vector<Vertex*> &animal)
{
    int number;
    std::string nom_image;
    double population;
    std::string nom_animal;
    int ordre;
    int posx, posy;
    int actif;
    //std::vector<Sommet*> animal;
    //std::vector<BITMAP*> vecteur_image;

    //Graphe* graphe;

    std::ifstream fichier(nom_fichier.c_str());

    if(fichier)
    {
        fichier >> ordre;
        SetNbSommet(ordre);
        //std::vector<Vertex*> animal = new std::vector<Vertex*>(ordre);

        for(int i=0; i<ordre; i++)
        {
            animal.push_back(new Vertex);
            // lecture du fichier
            fichier >> nom_animal >> number >> population >> posx >> posy >>actif;  //on lit jusqu'à l'espace et on stocke ce qui est lu dans la variable indiquée
            nom_image= nom_animal+ ".bmp";

            // attribution des photos a son animal
            //animal[i].Setimage();
            animal[i]->Setname(nom_animal);
            animal[i]->Setimage(nom_image);
            animal[i]->Setnumber(number);
            animal[i]->Setvalue(population);
            animal[i]->Setposx(posx);
            animal[i]->Setposy(posy);
            animal[i]->Setactif(actif);

            //std::cout<<animal[i]->Getname()<<" "<<animal[i]->Getposx()<<std::endl;

        }

    }
    else
        std::cout << "Impossible d'ouvrir le fichier !" << std::endl;

    //std::cout << animal.size() << std::endl;

}

bool Graph::allocationMatriceAdjacent()
{

    if(GetNbSommet() == 0)
    {
        std::cout<<"Le nombre de sommet est 0 lors de l allocation de la matrice\n\tPas chargement donnee / Mauvaise donnee"<<std::endl;
    }

    m_matrice = new int*[GetNbSommet()];
    for(int i = 0; i < GetNbSommet(); i++)
    {
        m_matrice[i] = new int[GetNbSommet()];
    }

    return true;
}

void Graph::lecture_fichier_matrice(std::string nom_fichier)
{
    std::ifstream fichier(nom_fichier.c_str());

    try
    {
        //Si echec allocation matrice
        if(!allocationMatriceAdjacent())
        {
            std::cout<<"ERREUR ALLOC"<<std::endl;
        }

        for (int i=0; i<GetNbSommet(); i++)
        {
            for (int y=0; y<GetNbSommet(); y++)
            {
                fichier >> m_matrice[i][y];
            }
        }
        //std::cout<<m_matrice[0][2]<<" "<<m_matrice[2][0];

    }

    catch (...)
    {
        fichier.close();
        std::cout<<"ERREUR FICHIER"<<std::endl;
    }

    fichier.close();
}


void Graph::test_remove_edge(int eidx)
{
/// référence vers le Edge à enlever
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

/// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

/// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );

/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

}

void Graph::test_remove_vertex(int vidx,std::vector<Vertex*> &animal)
{
    Vertex remedv=m_vertices.at(vidx);
    int id_vert1;
    int id_vert2;

    int idx_max=-1;

    for (auto &elt : m_edges)
    {
        if(elt.first>idx_max)
            idx_max=elt.first;

    }


    //std::cout<<"\n"<<id_vert1<<"   "<<id_vert2<<"\n\n" ;
    if(m_interface && remedv.m_interface)
    {

        for(int i=0 ; i<=idx_max ; i++)
        {

            id_vert1 = m_edges[i].m_from  ;
            id_vert2 = m_edges[i].m_to ;

            if(id_vert1==vidx || id_vert2==vidx)
            {
                test_remove_edge(i);

            }


        }
        m_interface->m_main_box.remove_child(remedv.m_interface->m_top_box);
    }

    animal[vidx]->Setactif(0);
    //remedv.Setactif(0);
    m_vertices.erase(vidx);

}
void Graph::sauvegarder(std::vector<Vertex*> &animal, std::string nom_fichier)
{
    std::ofstream fichier(nom_fichier.c_str());
    if(fichier)
    {
        fichier<<m_NbSommet<<std::endl;
        for(int i=0; i<m_NbSommet; i++)
        {
            std::cout<<"coucou  ";
            if(animal[i]->Getactif()==1)
            {
                fichier<<animal[i]->Getname()<<std::endl<<animal[i]->Getnumber()<<std::endl<<m_vertices[i].Getvalue()<<std::endl;
                fichier<<m_vertices[i].m_interface->m_top_box.get_posx()<<std::endl<<m_vertices[i].m_interface->m_top_box.get_posy()<<std::endl;
                fichier<<animal[i]->Getactif()<<std::endl;
            }
            else
            {
                animal[i]->Setposx(50);
                animal[i]->Setposy(10);
                //m_vertices[i].m_interface->m_top_box.set_posx(10);
                //m_vertices[i].m_interface->m_top_box.set_posy(10);
                std::cout <<"hello  ";
                fichier<<animal[i]->Getname()<<std::endl<<animal[i]->Getnumber()<<std::endl<<m_vertices[i].Getvalue()<<std::endl;
                // fichier<<m_vertices[i].m_interface->m_top_box.get_posx()<<std::endl<<m_vertices[i].m_interface->m_top_box.get_posy()<<std::endl;
                fichier<<animal[i]->Getposx()<<std::endl<<animal[i]->Getposy()<<std::endl;
                fichier<<animal[i]->Getactif()<<std::endl;
            }

        }
    }
}
void Graph::test_mort(std::vector<Vertex*> &animal)
{
    for(int i=0; i<m_NbSommet; i++)
    {

        if(m_vertices[i].Getvalue()==0)
        {
            test_remove_vertex(i,animal);
        }

    }
}
void Graph::une_journee(std::vector<Vertex*> &animal)
{
    int id_vert1;
    int id_vert2;

    for(int i=0 ; i<m_edges.size() ; i++)
    {
        id_vert1 = m_edges[i].m_from ;
        id_vert2 = m_edges[i].m_to;
        if(m_edges[i].m_weight==50)
        {
            m_vertices[id_vert1].Setvalue(m_vertices[id_vert1].Getvalue()-3);
            m_vertices[id_vert2].Setvalue(m_vertices[id_vert2].Getvalue()+3);
        }
        if(m_edges[i].m_weight<50)
        {
            m_vertices[id_vert1].Setvalue(m_vertices[id_vert1].Getvalue()-1);
            m_vertices[id_vert2].Setvalue(m_vertices[id_vert2].Getvalue()+1);
        }

        if(m_edges[i].m_weight>50)
        {
            m_vertices[id_vert1].Setvalue(m_vertices[id_vert1].Getvalue()-5);
            m_vertices[id_vert2].Setvalue(m_vertices[id_vert2].Getvalue()+5);
        }

    }
}

void Graph::ajouter_sommet(std::vector<Vertex*> &animal)
{
    int idx_max=-1;
    for (auto &elt : m_edges)
    {
        if(elt.first>idx_max)
            idx_max=elt.first;

    }

    for(int i=0; i<m_NbSommet; i++)
    {
        if(animal[i]->Getactif()==0)
        {

            animal[i]->Setactif(1);
            std::cout<<"\n"<<animal[i]->Getnumber()<<"   " <<animal[i]->Getactif();
            animal[i]->Setvalue(80);
            add_interfaced_vertex(animal[i]->Getnumber(),animal[i]->Getvalue(), animal[i]->Getposx(), animal[i]->Getposy(), animal[i]->Getimage());


            std::cout<<" c    "<<idx_max;
            int compteur=idx_max+1;

            for (int i=0; i<GetNbSommet(); i++)
            {
                for (int y=0; y<GetNbSommet(); y++)
                {
                    if(m_matrice[i][y]==1 && animal[i]->Getactif()==1 && animal[y]->Getactif()==1)
                    {

                        add_interfaced_edge(compteur, i, y, 50);
                        compteur++;
                    }
                }
            }

        }

    }
}

int* Graph:: uneComposanteFortementConnexe (int s)
{
//Variables locales
    int *c1, *c2 ; // composantes connexes directes partant de s et indirectes arrivant vers s
    int *c ; // composante fortement connexe = c1  c2 à retourner
    bool *marques ; // tableau dynamique indiquant si les sommets sont marqués ou non
    int x, y ; // numéros de sommets intermédiaires des composantes connexes
    bool ajoute = true ; // booléen indiquant si une nouvelle composante connexe est ajoutée
// Allouer les tableaux dynamiques c1, c2, c et marques de taille « ordre »

    c1 = new int[GetNbSommet()];
    c2 = new int[GetNbSommet()];
    c= new int[GetNbSommet()];
    marques = new bool[GetNbSommet()];

// Initialiser les valeurs de ces tableaux à 0
    for(int i=0; i<GetNbSommet(); i++)
    {
        c1[i]=0;
        c2[i]=0;
        marques[i]=false;
        c[i]=0;
    }
// Rendre le sommet s connexe
    c1[s] = 1 ;
    c2[s] = 1 ;
// Recherche des composantes connexes partant de s à ajouter dans c1 :
    while (ajoute)
    {
        ajoute = false;
        for (x=0 ; x<GetNbSommet(); x++)
        {
            if (!marques[x] && c1[x]==1)
            {
                marques[x] = true ;
                for (y=0 ; y<GetNbSommet() ; y++)
                {
                    if (m_matrice[x][y]==1 && !marques[y])
                    {
                        c1[y] = 1 ;
                        ajoute = true ; // nouvelle composante connexe ajoutée
                    }
                }
            }
        }
    }
/// Recherche des composantes connexes arrivant à s à ajouter dans c2 :
    ajoute=true;
    for(int i=0; i<GetNbSommet(); i++)
    {
        marques[i]=false;
    }
    while (ajoute)
    {
        ajoute = false;
        for (x=0 ; x<GetNbSommet(); x++)
        {
            if (!marques[x] && c2[x]==1)
            {
                marques[x] = true ;
                for (y=0 ; y<GetNbSommet() ; y++)
                {
                    if (m_matrice[y][x]==1 && !marques[y])
                    {
                        c2[y] = 1 ;
                        ajoute = true ; // nouvelle composante connexe ajoutée
                    }
                }
            }
        }
    }
// Composante fortement connexe c = intersection de c1 et c2

    for (x=0 ; x<GetNbSommet() ; x++)
    {
        // std::cout<<" "<<c1[x];

        c[x] = c1[x] & c2[x] ;

    }

    for (x=0 ; x<GetNbSommet() ; x++)
    {
        //std::cout<<" "<<c2[x];

    }


// Retourner la composante fortement connexe c
    return c ;


}

int ** Graph::toutesLesComposantesFortementConnexes ()
{
// Variables locales
    int **tabc ; // tableau dynamique des composantes fortement connexes à retourner
    bool *marques ; // tableau dynamique indiquant si les sommets sont marqués ou non
    int x, y ; // numéros de sommets intermédiaires comme indices des tableaux
// Allouer les tableaux dynamiques tabc et marques de taille « ordre »
    tabc = new int*[GetNbSommet()];
    for(int i=0; i<GetNbSommet(); i++)
    {
        tabc[i]=new int[GetNbSommet()];
    }
    marques = new bool[GetNbSommet()];

// Initialiser les valeurs de ces tableaux à 0
    for(int i=0; i<GetNbSommet(); i++)
    {

        marques[i]=false;
    }
    for(int j=0; j<GetNbSommet(); j++)
    {
        for(int v=0; v<GetNbSommet(); v++)
        {
            tabc[j][v]=0;
        }

    }
// Pour tous les sommets x non marqués
// Rechercher la composante fortement connexe de x
// Marquer chaque sommet x et marquer les sommets y connectés à x et non marqués
    for (x=0 ; x<GetNbSommet() ; x++)
    {
        if (!marques[x])
        {
            tabc[x] = uneComposanteFortementConnexe(x) ;
            marques[x] = 1 ;
            for (y=0 ; y<GetNbSommet() ; y++)
            {
                if (tabc[x][y]==1&& !marques[y])
                {
                    marques[y] = 1 ;

                }
            }
        }
    }

    m_color.push_back(ROUGECLAIR);
    m_color.push_back(BLEUCLAIR);
    m_color.push_back(ORANGECLAIR);
    m_color.push_back(SABLECLAIR);

    int change=0;
    int u=0;

    for(int i=0; i<GetNbSommet(); i++)
    {
        if(change==1)
        {
            u++;
        }

        for (int j=0; j<GetNbSommet(); j++)
        {
            if(tabc[i][j]==1 && i!=j)
            {
                //COLOR++
                m_vertices[i].m_interface->m_top_box.set_bg_color(m_color[u]);
                m_vertices[j].m_interface->m_top_box.set_bg_color(m_color[u]);
                change=1;
            }
        }
    }
    return tabc ;
}


void Graph::evolution_pop()
{
    double k=0;
    for (auto &elem2:m_vertices)
    {
        for(const auto &elem:elem2.second.m_in)
        {
            for(const auto &elem3:m_edges)
            {
                k = k+ 0.000000000000001* elem3.second.m_weight * m_vertices[elem].m_value;

            }
        }
        if(elem2.second.m_value>0)
            elem2.second.m_value = elem2.second.m_value + 0.00000000000001*elem2.second.m_value*(1-elem2.second.m_value/k)-k;

    }
}

void Graph::reset()
{
    for(int i=0; i<30; i++)
    m_edges.erase(i);

    for(int j=0;j<12;j++)
        m_vertices.erase(j);
   // for (int x=0; x<12)

}

