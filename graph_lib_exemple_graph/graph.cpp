#include "graph.h"




/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
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

/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.

void Graph::make_example(std::vector<Vertex*> animal)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    /*add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);*/

    for(int i=0; i<GetNbSommet(); i++)
    {
        //std::cout << animal[i]->Getnumber() <<" "<< animal[i]->Getvalue() <<" "<< animal[i]->Getposx() <<" "<< animal[i]->Getposy() <<" "<< animal[i]->Getimage() <<" "<< std::endl;
        if(animal[i]->Getactif()!=0)
        {
            std::cout<< " oui  \n";
            add_interfaced_vertex(animal[i]->Getnumber(),animal[i]->Getvalue(), animal[i]->Getposx(), animal[i]->Getposy(), animal[i]->Getimage());
        }

    }


    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    /*add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);*/

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


    //std::cout<<"/n/n/n"<<compteur<<std::endl;

}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
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

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name)
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }

    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name);

    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
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
            fichier >> nom_animal >> number >> population >> posx >> posy >>actif;  //on lit jusqu'� l'espace et on stocke ce qui est lu dans la variable indiqu�e
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
/// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

/// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

/// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
   m_edges.erase( eidx );

/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
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
          std::cout<<" c    "<<idx_max;


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
void Graph::sauvegarder(std::vector<Vertex*> &animal)
{
    std::ofstream fichier("chaine_afrique.txt");
    if(fichier)
    {
        fichier<<m_NbSommet<<std::endl;
        for(int i=0; i<m_NbSommet;i++)
        {  std::cout<<"coucou  ";
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
    for(int i=0; i<m_NbSommet;i++)
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

    for(int i=0; i<m_NbSommet;i++)
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

