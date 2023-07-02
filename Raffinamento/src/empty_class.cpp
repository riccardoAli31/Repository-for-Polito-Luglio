#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include <vector>
#include <algorithm>
#include "cmath"
#include "empty_class.hpp"
#include "sorting.hpp"


using namespace std;
using namespace Eigen;
using namespace Sorting;
//using namespace Project;





namespace Project{

//void Propagazione(unsigned int idLatoTagliatoVecchio, unsigned int idLatoTagliatoNuovo, Project::Cell2D Triangolo, unsigned int latoMax);


Project::Cell0D::Cell0D(unsigned int& id, unsigned int& marker, Vector2d& coord)
    {
    Id0D = id;
    marker0D = marker;
    Coord = coord;
    };



Project::Cell1D::Cell1D(unsigned int& id,unsigned int& marker,vector<unsigned int>& vertices)
    {
    Id1D = id;
    marker1D = marker;
    //Vertices1D = vertices;

    if (vertices.size() >= 2)
        {
            Vertices1D = vertices;
        }
        else
        {
            cerr << "Invalid number of vertices for Cell1D with id: " << id << endl;

        }
    };

Project::Cell2D::Cell2D(unsigned int& id,array<unsigned int, 3>& Vertices, array<unsigned int, 3>& Edges2D, vector<Project::Cell0D>& vectp2d)
    {
    Id2D = id;
    Vertices2D = Vertices;
    Edges = Edges2D;
    vectp2D = vectp2d;
    };



Project::TriangularMesh::TriangularMesh(unsigned int& numbercell0D12, vector<Project::Cell0D>& vectp12, unsigned int& numbercell1D12, vector<Project::Cell1D>& vects12, unsigned int& numbercell2D12, std::vector<vector<unsigned int>>& LenghtMax12, vector<Project::Cell2D>& vectt12){

    numbercell0D = numbercell0D12;
    vectp1 = vectp12;

    numbercell1D = numbercell1D12;
    vects1 = vects12;

    numbercell2D = numbercell2D12;
    LenghtMax = LenghtMax12;
    vectt1 = vectt12;
};



//unsigned int numbercell0D123;
//vector<Project::Cell0D> vectp;
//unsigned int numbercell1D123;
//vector<Project::Cell1D> vects;
//vector<double> LengthEdges123;
//unsigned int numbercell2D123;
//std::vector<vector<unsigned int>> LenghtMax123;
//vector<Project::Cell2D> vectt;






double Project::Cell1D::LengthEdge(vector<Project::Cell0D>& vectp){
    unsigned int A = Vertices1D[0];
    unsigned int B = Vertices1D[1];
    Vector2d coordOrigin = vectp[A].Coord;
    //double coordOriginx = vectp[A].Coord[0];
    //cout << coordOrigin[0] << endl;
    //double coordOriginy = vectp[A].Coord[1];
    Vector2d coordEnd = vectp[B].Coord;
    //double coordEndx = vectp[B].Coord[0];
    //double coordEndy = vectp[B].Coord[1];

    double len = (coordEnd-coordOrigin).norm();
    //double len = sqrt(pow(coordOriginx - coordEndx, 2)+pow(coordOriginy - coordEndy, 2));
    //cout << len << endl;
    return len;
    }

//PROBLEMA TOLLERANZA
unsigned int Project::Cell2D::maxedge(vector<Project::Cell1D>& vects, vector<Project::Cell0D>& vectp){
    unsigned int indmax = 0;
    double max = vects[Edges[0]].LengthEdge(vectp);
    cout << max << endl;
    for (unsigned int i = 1; i<3; i++){
        if(vects[Edges[i]].LengthEdge(vectp)  > max - tol1D){  // check
            max = vects[Edges[i]].LengthEdge(vectp);
            indmax = i;
        }
    }
    return Edges[indmax];

}

double Project::Cell2D::Area(){
             //Formula dell'area di Gauss
             double A_12 = (vectp2D[this->Vertices2D[0]].Coord[0]*vectp2D[this->Vertices2D[1]].Coord[1]) - (vectp2D[this->Vertices2D[0]].Coord[1]*vectp2D[this->Vertices2D[1]].Coord[0]);
             double A_23 = (vectp2D[this->Vertices2D[1]].Coord[0]*vectp2D[this->Vertices2D[2]].Coord[1]) - (vectp2D[this->Vertices2D[1]].Coord[1]*vectp2D[this->Vertices2D[2]].Coord[0]);
             double A_31 = (vectp2D[this->Vertices2D[2]].Coord[0]*vectp2D[this->Vertices2D[0]].Coord[1]) - (vectp2D[this->Vertices2D[2]].Coord[1]*vectp2D[this->Vertices2D[0]].Coord[0]);
             return abs((A_12+A_23+A_31)/2);
  }

//double Project::Cell2D::Area(){
//    double A = this->AreaProvv(vectp);
//    return A;
//}



//METODI LENEDGE, MAXEDGE, (AREA vince)

// !! se nella parte iterativa il lato non viene tolto ma aggiornato con nuova end, non c'è riscalamento id nei vettori -> non serve ciclo for !!

//metterei double anziché void




//IMPORTAZIONE






bool ImportCell0Ds(vector<Project::Cell0D>& vettorePunti, unsigned int& numbercell0D123, const string& file0)
{

ifstream file;
file.open(file0);

if(file.fail()){
return false;
}

list<string> listLines;
string line;
while (getline(file, line))
    listLines.push_back(line);

file.close();

listLines.pop_front();

numbercell0D123 = listLines.size();

if (numbercell0D123 == 0)
{
    cerr << "There is no cell 0D" << endl;
    return false;
}



for (const string& line : listLines)
{
istringstream converter(line);

unsigned int id;
unsigned int marker;
Vector2d coord;

converter >>  id >> marker >> coord[0] >> coord[1];


Project::Cell0D point = Project::Cell0D(id,marker,coord);
vettorePunti.push_back(point);
cout << point.Coord << endl;

//    if( marker != 0)
//    {
//      if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
//        mesh.Cell0DMarkers.insert({marker, {id}});
//      else
//        mesh.Cell0DMarkers[marker].push_back(id);
//    }

  }
  file.close();
  return true;
}



bool ImportCell1Ds(vector<Project::Cell1D>& vettoreLati, unsigned int& numbercell1D123, vector<Project::Cell0D>& vettorePunti, const string& file1)
{

  ifstream file;
  file.open(file1);

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();

  numbercell1D123 = listLines.size();

  if (numbercell1D123 == 0)
  {
    cerr << "There is no cell 1D" << endl;
    return false;
  }



  for (const string& line : listLines)
  {
    istringstream converter(line);

    unsigned int id;
    unsigned int marker;
    vector<unsigned int> vertices(2);

    converter >>  id >> marker >> vertices[0] >> vertices[1];
    Project::Cell1D segment = Project::Cell1D(id,marker,vertices);
    vettoreLati.push_back(segment);
    cout << segment.LengthEdge(vettorePunti) << endl;
    //LengthEdges123.push_back(segment.LengthEdge(vettorePunti));


  }

  file.close();

  return true;
}



bool ImportCell2Ds(vector<Project::Cell2D>& vettoreTriangoli, unsigned int& numbercell2D123, vector<Project::Cell0D>& vettorePunti, vector<Project::Cell0D>& vectp2DF, const string& file2)
{

  ifstream file;
  file.open(file2);

  if(file.fail())
    return false;

  list<string> listLines;
  string line;
  while (getline(file, line))
    listLines.push_back(line);

  listLines.pop_front();


  numbercell2D123 = listLines.size();

  if (numbercell2D123 == 0)
  {
    cerr << "There is no cell 2D" << endl;
    return false;
  }



  for (const string& line : listLines)
  {
    istringstream converter(line);

    unsigned int id;
    array<unsigned int, 3> vertices;
    array<unsigned int, 3> edges;

    converter >>  id;
    for(unsigned int i = 0; i < 3; i++)
      converter >> vertices[i];

    for(unsigned int i = 0; i < 3; i++)
      converter >> edges[i];


    vectp2DF = {vettorePunti[vertices[0]], vettorePunti[vertices[1]], vettorePunti[vertices[2]]};
    Project::Cell2D triangle = Project::Cell2D(id,vertices,edges, vectp2DF);
    vettoreTriangoli.push_back(triangle);

  }
  file.close();
  return true;
}

//Project::TriangularMesh mesh = Project::TriangularMesh(numbercell0D123, vectp, numbercell1D123, vects, LengthEdges123, numbercell2D123, LenghtMax123, vectt);





//----------------------------------------------------------------------

// RAFFINAMENTO:

// MATRICE DI ADIACENZA [OK]
// ORDINAMENTO PER AREA [Riccardo]
// PUNTO MEDIO LATO LUNGO (CHE SI AGGIORNA CON STESSA ORIGIN E NUOVA END, LEN = LEN/2) (NEW LATO, LEN = LEN/2)
// TAGLIO LATO LUNGO


//MatrAdiac::MatrAdiac(vector<Project::Cell2D>& vectt, vector<Project::Cell1D>& vects) {
vector<vector<unsigned int>> MatrAdiac(vector<Project::Cell2D>& vectt, vector<Project::Cell1D>& vects){
    vector<vector<unsigned int>> MatrAdiac1(vects.size(), vector<unsigned int>());
    for (unsigned int i = 0; i < vectt.size(); i++) {
        for (unsigned int j = 0; j < 3; j++) {
            MatrAdiac1[vectt[i].Edges[j]].push_back(vectt[i].Id2D);
        }
    }
    return MatrAdiac1;
    //Matr = MatrAdiac;
}





void Bisect(Project::Cell2D& triangleToBisect, vector<Project::Cell0D>& vectp, vector<Project::Cell1D>& vects, vector<Project::Cell2D>& vectt, vector<vector<unsigned int>>& Matr){


    unsigned int longest = triangleToBisect.maxedge(vects, vectp);
    vector<Project::Cell0D> vectp2D;
    vector<Project::Cell0D> vectp2D1;
    //serve subito controllo: 1) marker lato lungo 2) lato lungo dell'eventuale altro triangolo
    unsigned int markerMaxEdge = vects[longest].marker1D;
    unsigned int idAltroMaxEdge = 0;
    unsigned int idAltroTri = 0;
    if (markerMaxEdge == 0) {
        for (unsigned int i = 0; i<2; i++) {
            if (Matr[longest][i] != triangleToBisect.Id2D) {
                idAltroTri = Matr[longest][i];
                idAltroMaxEdge = vectt[idAltroMaxEdge].maxedge(vects, vectp);
            }
        }
    }


    // salvo vertici e lati che poi dovrò aggiornare
    array<unsigned int, 3> latiTriNuovo = triangleToBisect.Edges;
    array<unsigned int, 3> vertTriNuovo = triangleToBisect.Vertices2D;
    // inizio bisezione
    Vector2d midCoord;
    midCoord[0] = (vectp[vects[longest].Vertices1D[0]].Coord[0] + vectp[vects[longest].Vertices1D[1]].Coord[0]) *0.5;
    midCoord[1] = (vectp[vects[longest].Vertices1D[0]].Coord[1] + vectp[vects[longest].Vertices1D[1]].Coord[1]) *0.5;

    unsigned int markerP;
    if (vectp[vects[longest].Vertices1D[0]].marker0D == 0 || vectp[vects[longest].Vertices1D[1]].marker0D == 0) {
        markerP = 0;
    }
    else {
    markerP = vectp[vects[longest].Vertices1D[0]].marker0D; // per come sono i dati di partenza non ci sono/possono
                                                                      // essere ulteriori configurazioni
    }
    unsigned int newIndexpoint = vectp.size();
    Project::Cell0D newVertex = Cell0D(newIndexpoint, markerP, midCoord);
    vectp.push_back(newVertex);
    vectp2D.push_back(newVertex);
    vectp2D1.push_back(newVertex);

    unsigned int opposite = 0;
    for(unsigned int i = 0; i < 3; i++)
    {
        if(!(vects[longest].Vertices1D[0] == triangleToBisect.Vertices2D[i] || vects[longest].Vertices1D[1] == triangleToBisect.Vertices2D[i]))
            {
            opposite = triangleToBisect.Vertices2D[i];
            vectp2D.push_back(vectp[opposite]);
            vectp2D1.push_back(vectp[opposite]);
            break;
            }
    }

    vector<unsigned int> MedianaVert = {opposite, newVertex.Id0D};

    unsigned int idNewEdge = vects.size();
    unsigned int idNewEdge2 = idNewEdge + 1;

    unsigned int markerMediana = 0; // NON PUO' ESSERE ALTRIMENTI



     //Creo segm mediana
    Cell1D Mediana = Cell1D(idNewEdge, markerMediana, MedianaVert);
    vects.push_back(Mediana);

    vector<unsigned int> NewSegVert = {newVertex.Id0D, vects[longest].Vertices1D[1]};



    //Creo segm pto medio -> end vecchia
    Cell1D newSegment = Cell1D(idNewEdge2, vects[longest].marker1D, NewSegVert);
    vects.push_back(newSegment);


    //aggiorno segm origin vecchia -> pto medio
    vects[longest].Vertices1D[1] = newVertex.Id0D;  // GUARDATO FINO A QUA



    //vertici effettivi del triangolo nuovo
    for (unsigned int i = 0;i<3;i++) {
        if (vertTriNuovo[i] != opposite && vertTriNuovo[i] != vects[longest].Vertices1D[1]) {
            vertTriNuovo[i] = newVertex.Id0D;
            vectp2D.push_back(vectp[vertTriNuovo[i]]);
            break;
        };
    }

    // vertici effettivi del triangolo aggiornato
    for (unsigned int i = 0;i<3;i++) {
        if (triangleToBisect.Vertices2D[i] != opposite && triangleToBisect.Vertices2D[i] != vects[longest].Vertices1D[0]) {
            triangleToBisect.Vertices2D[i] = newVertex.Id0D;
            vectp2D1.push_back(vectp[triangleToBisect.Vertices2D[i]]);
            triangleToBisect.vectp2D = vectp2D1;
            break;
        };
    }


    // lati effettivi del triangolo aggiornato
    for (unsigned int i=0; i < 3; i++) {
        if ((vects[triangleToBisect.Edges[i]].Vertices1D[0] == opposite && vects[triangleToBisect.Edges[i]].Vertices1D[1] == newSegment.Vertices1D[1]) || (vects[triangleToBisect.Edges[i]].Vertices1D[1] == opposite && vects[triangleToBisect.Edges[i]].Vertices1D[0] == newSegment.Vertices1D[1])) {
            triangleToBisect.Edges[i] = Mediana.Id1D;
            break;
        }
    }

    // lati effettivi del triangolo nuovo


    for (unsigned int i=0; i < 3; i++) {
        if (latiTriNuovo[i] == longest) {
            latiTriNuovo[i] = newSegment.Id1D;
        }
        if ((vects[latiTriNuovo[i]].Vertices1D[0] == opposite && vects[latiTriNuovo[i]].Vertices1D[1] == vects[longest].Vertices1D[0]) || (vects[latiTriNuovo[i]].Vertices1D[1] == opposite && vects[latiTriNuovo[i]].Vertices1D[0] == vects[longest].Vertices1D[0])) {
            latiTriNuovo[i] = Mediana.Id1D;
        }
    }

    // creazione triangolo nuovo
    unsigned int idnewTri = vectt.size();
    Cell2D newTri = Cell2D(idnewTri, vertTriNuovo, latiTriNuovo, vectp2D);
    vectt.push_back(newTri);


    // aggiorno matrice di adiacenza
    // aggiungo mediana
    Matr.push_back({newTri.Id2D, triangleToBisect.Id2D});
    // aggiungo newSegment
    Matr.push_back({newTri.Id2D, idAltroTri});
    // aggiorno terzo lato
    for (unsigned int i=0; i<3; i++) {
        if(newTri.Edges[i] != Mediana.Id1D && newTri.Edges[i] != newSegment.Id1D){
            for (unsigned int j = 0; j < 2; j++) {
                if(Matr[newTri.Edges[i]][j] == triangleToBisect.Id2D){
                    Matr[newTri.Edges[i]][j] = newTri.Id2D;
                    break;
                }
            }
            break;
        }
    }

    if (markerMaxEdge == 0) {
        Cell2D& AltroTri = vectt[idAltroTri];
        //unsigned int recursionDepth = 0;
        unsigned int newSegmentId = newSegment.Id1D;
        Propagazione(longest, newSegmentId, AltroTri, idAltroMaxEdge, vectp, vects, vectt, Matr); // recursionDepth);
    }


} // fine Bisect



void Propagazione(unsigned int& idLatoTagliatoVecchio, unsigned int& idLatoTagliatoNuovo, Cell2D& Triangolo, unsigned int& latoMax, vector<Project::Cell0D>& vectp, vector<Project::Cell1D>& vects, vector<Project::Cell2D>& vectt, vector<vector<unsigned int>>& Matr)
{ //, unsigned int& numberRecurs){

    vector<Project::Cell0D> vectpUlti;
    vector<Project::Cell0D> vectpPenu;
    vector<Project::Cell0D> vectpResi;
    if (idLatoTagliatoVecchio == latoMax){

        // collega pto medio e vertice opposto

        array<unsigned int, 3> latiUltimoTri = Triangolo.Edges;
        array<unsigned int, 3> vertUltimoTri = Triangolo.Vertices2D;

        unsigned int newopposite = 0;
        for(unsigned int i = 0; i < 3; i++)
        {
            if(!(vects[idLatoTagliatoVecchio].Vertices1D[0] == Triangolo.Vertices2D[i] || vects[idLatoTagliatoNuovo].Vertices1D[1] == Triangolo.Vertices2D[i]))
                {
                newopposite = Triangolo.Vertices2D[i];
                vectpUlti.push_back(vectp[newopposite]);
                break;
                }
        }

        unsigned int marker0 = 0;
        unsigned int size = vects.size();
        vector<unsigned int> vettore = {vects[idLatoTagliatoNuovo].Vertices1D[0], newopposite};
        //creo ultimo lato
        Cell1D Unione = Cell1D(size, marker0, vettore);
        vects.push_back(Unione);

        // creo Ultimo triangolo
        for (unsigned int i = 0; i<3; i++) {
            if (vertUltimoTri[i] == vects[idLatoTagliatoVecchio].Vertices1D[0]) {
                vertUltimoTri[i] = vects[idLatoTagliatoVecchio].Vertices1D[1];
                vectpUlti.push_back(vectp[vertUltimoTri[i]]);  // CONTROLLAREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
                break;
            }
        }

        for (unsigned int i=0; i < 3; i++) {
            if (latiUltimoTri[i] == latoMax) {
                latiUltimoTri[i] = idLatoTagliatoNuovo;
            }
            if ((vects[latiUltimoTri[i]].Vertices1D[0] == newopposite && vects[latiUltimoTri[i]].Vertices1D[1] == vects[latoMax].Vertices1D[0]) || (vects[latiUltimoTri[i]].Vertices1D[1] == newopposite && vects[latiUltimoTri[i]].Vertices1D[0] == vects[latoMax].Vertices1D[0])) {
                latiUltimoTri[i] = Unione.Id1D;
            }
        }

        unsigned int sizeT = vectt.size();

        Cell2D UltimoTriangolo = Cell2D(sizeT, vertUltimoTri, latiUltimoTri, vectpUlti);
        vectt.push_back(UltimoTriangolo);

        // aggiorno vertici
        for (unsigned int i = 0; i<3; i++) {
            if (Triangolo.Vertices2D[i] == vects[idLatoTagliatoNuovo].Vertices1D[1]) {
                Triangolo.Vertices2D[i] = vects[idLatoTagliatoNuovo].Vertices1D[0];
                break;
            }
        }

        // aggiorno lati
        for (unsigned int i=0; i < 3; i++) {
            if ((vects[Triangolo.Edges[i]].Vertices1D[0] == newopposite && vects[Triangolo.Edges[i]].Vertices1D[1] == vects[idLatoTagliatoNuovo].Vertices1D[1]) || (vects[Triangolo.Edges[i]].Vertices1D[1] == newopposite && vects[Triangolo.Edges[i]].Vertices1D[0] == vects[idLatoTagliatoNuovo].Vertices1D[1])) {
                Triangolo.Edges[i] = Unione.Id1D;
            }
        }


        //aggiorno matrice di adiacenza
        Matr.push_back({Triangolo.Id2D, UltimoTriangolo.Id2D});


        for (unsigned int i=0; i<3; i++) {
            if(UltimoTriangolo.Edges[i] != Unione.Id1D && UltimoTriangolo.Edges[i] != idLatoTagliatoNuovo){
                for (unsigned int j = 0; j < 2; j++) {
                    if(Matr[UltimoTriangolo.Edges[i]][j] == Triangolo.Id2D){
                        Matr[UltimoTriangolo.Edges[i]][j] = UltimoTriangolo.Id2D;
                        break;
                    }
                }

            }

        }
        for (unsigned int i=0; i<2; i++) {
            if (Matr[idLatoTagliatoNuovo][i] == Triangolo.Id2D) {
                Matr[idLatoTagliatoNuovo][i] = UltimoTriangolo.Id2D;
            }
        }


    } //fine if (stesso lato max)
    else {


        // va fatto un if che mette l'end di latoMax = end latotagliatonuovo (caso 1)
        //                            origin latoMax = origin latotagliatovecchio (caso 2)

        for (unsigned int i = 0; i <2 ; i++) {
            if (vects[latoMax].Vertices1D[i] == vects[idLatoTagliatoVecchio].Vertices1D[0] && 1==1){
                unsigned int supporto = vects[latoMax].Vertices1D[i];
                vects[latoMax].Vertices1D[i] = vects[latoMax].Vertices1D[i-1];
                vects[latoMax].Vertices1D[i-1] = supporto;

            }
            else if (vects[latoMax].Vertices1D[i] == vects[idLatoTagliatoVecchio].Vertices1D[1] && 1==0) {
                unsigned int supporto = vects[latoMax].Vertices1D[i];
                vects[latoMax].Vertices1D[i] = vects[latoMax].Vertices1D[i+1];
                vects[latoMax].Vertices1D[i+1] = supporto;
            }
        }



        unsigned int markerMaxEdgePropa = vects[latoMax].marker1D;
        unsigned int idAltroMaxEdgePropa = 0;
        unsigned int idAltroTriPropa = 0;
        if (markerMaxEdgePropa == 0) {
            for (unsigned int i = 0; i<2; i++) {
                if (Matr[latoMax][i] != Triangolo.Id2D) {
                    idAltroTriPropa = Matr[latoMax][i];
                    idAltroMaxEdgePropa = vectt[Matr[latoMax][i]].maxedge(vects, vectp);
                }
            }
        }



        array<unsigned int, 3> latiTriNuovoPropa = Triangolo.Edges;
        array<unsigned int, 3> vertTriNuovoPropa = Triangolo.Vertices2D;

        Vector2d midCoordPropa;
        midCoordPropa[0] = (vectp[vects[latoMax].Vertices1D[0]].Coord[0] + vectp[vects[latoMax].Vertices1D[1]].Coord[0]) *0.5;
        midCoordPropa[1] = (vectp[vects[latoMax].Vertices1D[0]].Coord[1] + vectp[vects[latoMax].Vertices1D[1]].Coord[1]) *0.5;

        unsigned int markerPPropa;
        if (vectp[vects[latoMax].Vertices1D[0]].marker0D == 0 || vectp[vects[latoMax].Vertices1D[1]].marker0D == 0) {
            markerPPropa = 0;
        }
        else {
        markerPPropa = vectp[vects[latoMax].Vertices1D[0]].marker0D; // per come sono i dati di partenza non ci sono/possono
                                                                     // essere ulteriori configurazioni
        }

        unsigned int newIndexpointPropa = vectp.size();
        Project::Cell0D newVertexPropa = Cell0D(newIndexpointPropa, markerPPropa, midCoordPropa);
        vectp.push_back(newVertexPropa);
        vectpPenu.push_back(newVertexPropa);  // CONTROLLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

        unsigned int oppositePropa = 0;
        for(unsigned int i = 0; i < 3; i++)
        {
            if(!(vects[latoMax].Vertices1D[0] == Triangolo.Vertices2D[i] || vects[latoMax].Vertices1D[1] == Triangolo.Vertices2D[i]))
                {
                oppositePropa = Triangolo.Vertices2D[i];
                vectpPenu.push_back(vectp[oppositePropa]); // CONTROLLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                break;
                }
        }

        vector<unsigned int> MedianaVertPropa = {oppositePropa, newVertexPropa.Id0D};

        unsigned int idNewEdgePropa = vects.size();

        unsigned int markerMedianaPropa = 0; // NON PUO' ESSERE ALTRIMENTI

        Cell1D MedianaPropa = Cell1D(idNewEdgePropa, markerMedianaPropa, MedianaVertPropa);
        vects.push_back(MedianaPropa);

        vector<unsigned int> NewSegVertPropa = {newVertexPropa.Id0D, vects[latoMax].Vertices1D[1]};


        unsigned int idNewEdgeTemp = idNewEdgePropa + 1;

        Cell1D newSegmentPropa = Cell1D(idNewEdgeTemp, vects[latoMax].marker1D, NewSegVertPropa);
        vects.push_back(newSegmentPropa);

        // aggiorno end pto medio propa
        vects[latoMax].Vertices1D[1] = newVertexPropa.Id0D; //GUARDATO FINO A QUI



        unsigned int lontano = 0;
        for (unsigned int i = 0;i<3;i++) {
            if (Triangolo.Vertices2D[i] != vects[idLatoTagliatoNuovo].Vertices1D[1] && Triangolo.Vertices2D[i] != vects[idLatoTagliatoVecchio].Vertices1D[0]) {
                lontano = Triangolo.Vertices2D[i];
                vectpPenu.push_back(vectp[lontano]);
            }
        }

        // vertici effettivi nuovo propa
        for (unsigned int i = 0;i<3;i++) {
            if (vertTriNuovoPropa[i] == lontano) {
                vertTriNuovoPropa[i] = newVertexPropa.Id0D;
                break;
            }
        }

        // vertici aggiornati
        for (unsigned int i = 0;i<3;i++) {
            if (Triangolo.Vertices2D[i] != oppositePropa && Triangolo.Vertices2D[i] != lontano) {
                Triangolo.Vertices2D[i] = newVertexPropa.Id0D;
                break;
            };
        }

        // lati aggiornati
        if (vects[latoMax].Vertices1D[0] == vects[idLatoTagliatoVecchio].Vertices1D[0]) {
            for (unsigned int i=0; i < 3; i++) {
                if (Triangolo.Edges[i] == latoMax){
                    Triangolo.Edges[i] = newSegmentPropa.Vertices1D[1];
                }
                if (Triangolo.Edges[i] == idLatoTagliatoVecchio){
                    Triangolo.Edges[i] = MedianaPropa.Id1D;
                }
            }

        }
        else if (vects[latoMax].Vertices1D[1] == vects[idLatoTagliatoNuovo].Vertices1D[1]) {
            for (unsigned int i=0; i < 3; i++) {
                if (Triangolo.Edges[i] == idLatoTagliatoVecchio) {
                    Triangolo.Edges[i] = MedianaPropa.Id1D;
                }
            }
        }

        // lati effettivi nuovo propa
        if (vects[latoMax].Vertices1D[0] == vects[idLatoTagliatoVecchio].Vertices1D[0]) {
            for (unsigned int i=0; i < 3; i++) {
                if (latiTriNuovoPropa[i] != latoMax && latiTriNuovoPropa[i] != idLatoTagliatoVecchio){
                    latiTriNuovoPropa[i] = MedianaPropa.Id1D;
                }
            }
        }
        else if (vects[latoMax].Vertices1D[1] == vects[idLatoTagliatoNuovo].Vertices1D[1]) {
            for (unsigned int i=0; i < 3; i++) {
                if (latiTriNuovoPropa[i] == latoMax){
                    latiTriNuovoPropa[i] = newSegmentPropa.Id1D;
                }
                if ((vects[latiTriNuovoPropa[i]].Vertices1D[0] == oppositePropa && vects[latiTriNuovoPropa[i]].Vertices1D[1] == lontano) || (vects[latiTriNuovoPropa[i]].Vertices1D[1] == oppositePropa && vects[latiTriNuovoPropa[i]].Vertices1D[0] == lontano)) {
                    latiTriNuovoPropa[i] = MedianaPropa.Id1D;
                }
            }
        }

        unsigned int sizePenultimo = vectt.size();

        Cell2D Penultimo = Cell2D(sizePenultimo, vertTriNuovoPropa, latiTriNuovoPropa, vectpPenu);
        vectt.push_back(Penultimo);

        //aggiorno PARZIALMENTE matrice di adiac

        if (vects[latoMax].Vertices1D[0] == vects[idLatoTagliatoVecchio].Vertices1D[0]) {
            Matr.push_back({Triangolo.Id2D, Penultimo.Id2D});
            Matr.push_back({Triangolo.Id2D, idAltroTriPropa});
            for (unsigned int i = 0; i<2; i++) {
                if (Matr[idLatoTagliatoNuovo][i] == Triangolo.Id2D) {
                    Matr[idLatoTagliatoNuovo][i] = Penultimo.Id2D;
                }
            }
        }
        else if (vects[latoMax].Vertices1D[1] == vects[idLatoTagliatoNuovo].Vertices1D[1]) {
            Matr.push_back({Triangolo.Id2D, Penultimo.Id2D});
            for (unsigned int i = 0; i<2; i++) {
                if (Matr[idLatoTagliatoVecchio][i] == Triangolo.Id2D) {
                    Matr[idLatoTagliatoVecchio][i] = Penultimo.Id2D;
                }

            }
        }


        // collego i due punti medi

        array<unsigned int, 3> latiTriResiduoPropa = Penultimo.Edges;
        array<unsigned int, 3> vertTriResiduoPropa = Penultimo.Vertices2D;


        unsigned int sizeUnione = vects.size();
        unsigned int markerUnione = 0;
        vector<unsigned int> vettoreUnione = {vects[idLatoTagliatoVecchio].Vertices1D[1], newVertexPropa.Id0D};

        Cell1D Unione = Cell1D(sizeUnione, markerUnione, vettoreUnione);
        vects.push_back(Unione);

        if (vects[latoMax].Vertices1D[0] == vects[idLatoTagliatoVecchio].Vertices1D[0]) {

            for (unsigned int i = 0; i<3; i++) {
                // aggiorno vertici
                if (Penultimo.Vertices2D[i] == vects[idLatoTagliatoVecchio].Vertices1D[0]) {
                    Penultimo.Vertices2D[i] = vects[idLatoTagliatoVecchio].Vertices1D[1];

                    // CONTROLLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA E AGGIUNGIIIIIIIIIIIIIII AGGIORNAMENTO
                }
                if (vertTriResiduoPropa[i] == vects[idLatoTagliatoNuovo].Vertices1D[1]) {
                    vertTriResiduoPropa[i] = vects[idLatoTagliatoNuovo].Vertices1D[0];
                    vectpResi.push_back(vectp[vertTriNuovoPropa[i]]);
                }
                // aggiorno lati
                if (Penultimo.Edges[i] == idLatoTagliatoVecchio) {
                    Penultimo.Edges[i] = idLatoTagliatoNuovo;
                }
                if (Penultimo.Edges[i] == latoMax) {
                    Penultimo.Edges[i] = Unione.Id1D;
                }
                if (latiTriResiduoPropa[i] == MedianaPropa.Id1D) {
                    latiTriResiduoPropa[i] = Unione.Id1D;
                }

            }

            unsigned int sizeResiduoT = vectt.size();

            Cell2D Residuo = Cell2D(sizeResiduoT, vertTriResiduoPropa, latiTriResiduoPropa, vectpResi);
            vectt.push_back(Residuo);

            // aggiorno matrice adiacenza
            Matr.push_back({Penultimo.Id2D, Residuo.Id2D});
            for (unsigned int i = 0; i<2; i++) {
                if (Matr[latoMax][i] == Triangolo.Id2D) {
                    Matr[latoMax][i] = Residuo.Id2D;
                }
                if (Matr[idLatoTagliatoVecchio][i] == Triangolo.Id2D) {
                    Matr[idLatoTagliatoVecchio][i] = Residuo.Id2D;
                }
            }
        }
        else if (vects[latoMax].Vertices1D[1] == vects[idLatoTagliatoNuovo].Vertices1D[1]) {
            for (unsigned int i = 0; i<3; i++) {
                if (Penultimo.Vertices2D[i] == newSegmentPropa.Vertices1D[1]) {
                    Penultimo.Vertices2D[i] = vects[idLatoTagliatoVecchio].Vertices1D[1];
                }
                if (vertTriResiduoPropa[i] == vects[idLatoTagliatoVecchio].Vertices1D[0]) {
                    vertTriResiduoPropa[i] = vects[idLatoTagliatoNuovo].Vertices1D[0];
                }
                // aggiorno lati
                if (Penultimo.Edges[i] == newSegmentPropa.Id1D) {
                    Penultimo.Edges[i] = Unione.Id1D;
                }
                if (latiTriResiduoPropa[i] == MedianaPropa.Id1D) {
                    latiTriResiduoPropa[i] = Unione.Id1D;
                }
                if (latiTriResiduoPropa[i] == idLatoTagliatoVecchio) {
                    latiTriResiduoPropa[i] = idLatoTagliatoNuovo;
                }
            }

            // CONTROLLAAAAAAAAAAAAAAA E AGGIORNAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

            unsigned int sizeResiduo = vectt.size();

            Cell2D Residuo = Cell2D(sizeResiduo, vertTriResiduoPropa, latiTriResiduoPropa, vectpResi);
            vectt.push_back(Residuo);

            // aggiorno matrice adiacenza
            Matr.push_back({Residuo.Id2D, idAltroTriPropa});
            Matr.push_back({Penultimo.Id2D, Residuo.Id2D});
            for (unsigned int i = 0; i<2; i++) {
                if (Matr[idLatoTagliatoNuovo][i] == Triangolo.Id2D) {
                    Matr[idLatoTagliatoNuovo][i] = Residuo.Id2D;
                }
            }
        }

        // void Propagazione(unsigned int idLatoTagliatoVecchio, unsigned int idLatoTagliatoNuovo, Cell2D Triangolo, unsigned int latoMax){
        // in questo caso è ricorsiva
        if (markerMaxEdgePropa == 0) {
            Cell2D& AltroTriPropa = vectt[idAltroTriPropa];
            //unsigned int recursionDepth1 = numberRecurs + 1;
            Project::Propagazione(latoMax, newSegmentPropa.Id1D, AltroTriPropa, idAltroMaxEdgePropa, vectp, vects, vectt, Matr); //, recursionDepth1);
        }
    } // fine else (lato lungo diverso dal precedente)
} // fine Propagazione

//void PropagazioneRicorsiva(unsigned int& idLatoTagliatoVecchio, unsigned int& idLatoTagliatoNuovo, Cell2D& Triangolo, unsigned int& latoMax, vector<Project::Cell0D>& vectp, vector<Project::Cell1D>& vects, vector<Project::Cell2D>& vectt, vector<vector<unsigned int>>& Matr, unsigned int& numberRecurs){
//    Propagazione(idLatoTagliatoVecchio, idLatoTagliatoNuovo, Triangolo, latoMax, vectp, vects, vectt, Matr, numberRecurs);

}


 // fine namespace Cells


//-----------------------------------------------------------------------