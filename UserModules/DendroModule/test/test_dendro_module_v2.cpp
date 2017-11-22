/*
 * Smil
 * Copyright (c) 2011-2016 Matthieu Faessel
 *
 * This file is part of Smil.
 *
 * Smil is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Smil is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Smil.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * last modification by Amin Fehri
 * 
 */


#include <map>
#include <iostream>
#include "../../../Core/include/DBinary.h"
#include "DendroModule.hpp"

#include "Morpho/include/private/DMorphoGraph.hpp"
#include <include/private/DMorphoLabel.hpp>

using namespace smil;

class Test_DendrogramConstructor : public TestCase
{
  virtual void run()
  {
    // Construction of the desired dendrogram 
//     Dendrogram<UINT32,UINT32,UINT32> dendroTest(11);	
//     DendroNode<UINT32> dendroNodeA,dendroNodeB,dendroNodeC,dendroNodeD,dendroNodeE,
// 			dendroNodeF,dendroNodeG,dendroNodeH,dendroNodeI,dendroNodeJ,dendroNodeK;
//     // Nodes A, B, C, D, E, F are leafs
//     // NodeA
//     dendroNodeA.setValuation(3);
//     dendroNodeA.setMarker(0); //already default value, just to make things clearer
//     dendroNodeA.setFather(&dendroNodeG);
//     // NodeB
//     dendroNodeB.setValuation(1);
//     dendroNodeB.setMarker(1);
//     dendroNodeB.setFather(&dendroNodeG);
//     // NodeC
//     dendroNodeC.setValuation(4);
//     dendroNodeC.setMarker(0);
//     dendroNodeC.setFather(&dendroNodeH);
//     // NodeD
//     dendroNodeD.setValuation(2);
//     dendroNodeD.setMarker(2);
//     dendroNodeD.setFather(&dendroNodeH);
//     // NodeE
//     dendroNodeE.setValuation(5);
//     dendroNodeE.setMarker(3);
//     dendroNodeE.setFather(&dendroNodeI);
//     // NodeF
//     dendroNodeF.setValuation(2);
//     dendroNodeF.setMarker(0);
//     dendroNodeF.setFather(&dendroNodeI);
//     
//     // Nodes G, H, I, J, K are internal nodes
//     // NodeG	
//     dendroNodeG.setChildLeft(&dendroNodeA);
//     dendroNodeG.setChildRight(&dendroNodeB);
//     dendroNodeG.setNeighborLeft(&dendroNodeA);
//     dendroNodeG.setNeighborRight(&dendroNodeB);
//     dendroNodeG.setInternalNodeValuationInitial(1);
//     dendroNodeG.setFather(&dendroNodeK);
//     // NodeH	
//     dendroNodeH.setChildLeft(&dendroNodeC);
//     dendroNodeH.setChildRight(&dendroNodeD);
//     dendroNodeH.setNeighborLeft(&dendroNodeC);
//     dendroNodeH.setNeighborRight(&dendroNodeD);
//     dendroNodeH.setInternalNodeValuationInitial(2);
//     dendroNodeH.setFather(&dendroNodeJ);
//     // NodeI	
//     dendroNodeI.setChildLeft(&dendroNodeE);
//     dendroNodeI.setChildRight(&dendroNodeF);
//     dendroNodeI.setNeighborLeft(&dendroNodeE);
//     dendroNodeI.setNeighborRight(&dendroNodeF);
//     dendroNodeI.setInternalNodeValuationInitial(1);
//     dendroNodeI.setFather(&dendroNodeJ);
//     // NodeJ	
//     dendroNodeJ.setChildLeft(&dendroNodeH);
//     dendroNodeJ.setChildRight(&dendroNodeI);
//     dendroNodeJ.setNeighborLeft(&dendroNodeD);
//     dendroNodeJ.setNeighborRight(&dendroNodeE);
//     dendroNodeJ.setInternalNodeValuationInitial(7);
//     dendroNodeJ.setFather(&dendroNodeK);
//     // NodeK	
//     dendroNodeK.setChildLeft(&dendroNodeG);
//     dendroNodeK.setChildRight(&dendroNodeJ);
//     dendroNodeK.setNeighborLeft(&dendroNodeB);
//     dendroNodeK.setNeighborRight(&dendroNodeC);
//     dendroNodeK.setInternalNodeValuationInitial(8);
//     dendroNodeK.setFather(&dendroNodeK);


    // remplissage de dendroTest avec les noeuds créés
//     dendroTest.addDendroNodes(&dendroNodeA);
//     dendroTest.addDendroNodes(&dendroNodeB);
//     dendroTest.addDendroNodes(&dendroNodeC);
//     dendroTest.addDendroNodes(&dendroNodeD);
//     dendroTest.addDendroNodes(&dendroNodeE);
//     dendroTest.addDendroNodes(&dendroNodeF);
//     dendroTest.addDendroNodes(&dendroNodeG);
//     dendroTest.addDendroNodes(&dendroNodeH);
//     dendroTest.addDendroNodes(&dendroNodeI);
//     dendroTest.addDendroNodes(&dendroNodeJ);
//     dendroTest.addDendroNodes(&dendroNodeK);
    
    //------------------------------------------------
    // Construction of the graph from which the dendrogram is computed
    
//     smil::Graph<UINT32,float> g1;
//     g1.addNode(0,3);
//     g1.addNode(1,1);
//     g1.addNode(2,4);
//     g1.addNode(3,2);
//     g1.addNode(4,5);
//     g1.addNode(5,2);
//     
//     g1.addEdge(0,1, 1);
//     g1.addEdge(1,2, 8);
//     g1.addEdge(2,3, 2);
//     g1.addEdge(3,4, 7);
//     g1.addEdge(4,5, 1);
    
//     Dendrogram<UINT32,UINT32,UINT32> dendroFromG1 = Dendrogram<UINT32,UINT32,UINT32>(g1);
//     int nbNodes = dendroFromG1.getNbrNodes();
//     std::vector<DendroNode<UINT32>*>& dendroNodesG1 = dendroFromG1.getDendroNodes();
    
    
//     for (int i = 0; i<dendroNodesG1.size() ; i++){
//       DendroNode<UINT32> &curNode = *dendroNodesG1[i];
//       std::vector<UINT32> curLookup = curNode.getLookupProgeny();
//       cout << "****************** NOEUD NUMERO " << i << "*******" << endl;
//       for (int i = 0 ; i<curLookup.size() ; i++){
// 	cout << "noeud numéro " << i << " : " << curLookup[i] << endl;
//       }
//     }
    
//     Dendrogram<UINT32,UINT32,UINT32>::HierarchicalDendrogramConstruction(dendroFromG1,"surfacicImageReturn",50);
//     
//     long int lambda = 1;
//     
//     Dendrogram<UINT32,UINT32,UINT32>::removeMSTEdgesDendrogram(dendroFromG1,g1,lambda);
//     
//     g1.printSelf();

    
//     for (int i = 0 ; i<nbNodes; i++){
//       DendroNode<UINT32> &curNode = *dendroNodesG1[i];
//       cout << "Noeud numero " << i << 
//      " ; getValuation = " << curNode.getValuation() << 
//      " ; getInternalNodeValuationInitial = " << curNode.getInternalNodeValuationInitial() << 
//      " ; getInternalNodeValuationFinal = " << curNode.getInternalNodeValuationFinal()<< 
//      " ; getlabel = " << curNode.getLabel() << endl;
//      if (i>5){
//        cout << "labelFilsGauche = " << curNode.getChildLeft()->getLabel() <<
//        " ; labelFilsDroit = " << curNode.getChildRight()->getLabel() << 
//        " ; neighborGauche = " << curNode.getNeighborLeft()->getLabel() << 
//        " ; neighborDroit = " << curNode.getNeighborRight()->getLabel() << endl;
//      }
//     } 
    
    Image<UINT8> imOriginal8 = smil::Image<UINT8>("/home/afehri/Dropbox/Images/imTestPerso/cameramanMosa.png"); 
    Image<UINT8>  imIn = smil::Image<UINT8>("/home/afehri/Dropbox/Images/imTestPerso/cameramanMosa.png");
    Image<UINT8>  imGrad(imIn);
    
    Image<UINT32> imMosaic(imIn);
    Image<UINT32> imMosaic2(imIn);
    Image<UINT32> imOriginal(imIn);
    Image<UINT32> imEdgeValues(imIn);
    Image<UINT32> imNodeValues(imIn);
    Image<UINT32> imMosaicEnd(imIn);
    
    unsigned int un = 1;
    copy(imOriginal8,imOriginal);
    
    
    label(imIn, imMosaic2);
    sub(imMosaic2,un,imMosaic2);
    imMosaic2.show();
    Gui::execLoop();
    labelWithArea(imIn,imNodeValues);
    
    gradient(imOriginal8, imGrad);
    copy(imGrad,imEdgeValues);
    
    Graph<UINT32,UINT32> g2;
    mosaicToGraph(imMosaic2, imEdgeValues,imNodeValues, g2);
//     g2.printSelf();
    Graph<UINT32,UINT32> mst2 = g2.computeMST();
//     mst2.printSelf();

    Dendrogram<UINT32,UINT32,UINT32> dendroFromG2 = Dendrogram<UINT32,UINT32,UINT32>(mst2);
//     Dendrogram<UINT32,UINT32,UINT32>::HierarchicalDendrogramConstruction(dendroFromG2,"stochasticSurfacicImageReturn",100,imMosaic2,"erode");
    dendroFromG2.HierarchicalDendrogramConstruction("stochasticSurfacic",100,imMosaic2,"erode",HexSE());

//     
    dendroFromG2.sortNodes(true);
    int dendroFromG2NbrNodes = dendroFromG2.getNbrNodes();
    std::vector<DendroNode<UINT32>*> dendroFromG2Nodes = dendroFromG2.getDendroNodes();
    
    
//     TEST applyLookup
    int maxValMosa = smil::maxVal(imMosaic2);
    cout << "maxVal = " << maxValMosa << endl;
    std::map<UINT32,UINT32> lookupTest;
    for (int i = 0; i < maxValMosa + 1; i++){
      lookupTest[i] = 2*i;
    }
    Image<UINT32> imLookup = Image<UINT32>(imMosaic2);
    applyLookup<UINT32,UINT32>(imMosaic2,lookupTest,imLookup);
//     imMosaic2.show("imMosaic2");
//     imLookup.show("imLookup");
//     Gui::execLoop();
    
//     for (int i = 0; i<dendroFromG2NbrNodes; i++){
//       DendroNode<UINT32> &curNode = *dendroFromG2Nodes[i];
//       float lambda = curNode.getInternalNodeValuationInitial();  
//       if (lambda != 0){
// 	cout << "etape " << i+1 << "/" << dendroFromG2.getNbrNodes() << endl;
// 	cout << "lambda = " << lambda << endl; 
// 	Dendrogram<UINT32,UINT32,UINT32>::removeMSTEdgesDendrogram(dendroFromG2,mst2,lambda);
// 	graphToMosaic(imMosaic2, mst2, imMosaicEnd);
// 	cout <<"mst2.EdgeNbr = " <<  mst2.getEdgeNbr() << endl;
// 	imMosaicEnd.show("imMosaicEnd");
// 	imMosaicEnd.save(std::strcat("/home/afehri/Bureau/resultsCameraman/cameraman",str(i),".png"));
// 	Gui::execLoop();
//       }
//     }
    
//     float lambda = 0.5;
//     Dendrogram<UINT32,UINT32,UINT32>::removeMSTEdgesDendrogram(dendroFromG2,mst2,lambda);
//     graphToMosaic(imMosaic2, mst2, imMosaicEnd);
//     imMosaicEnd.show();
//     Gui::execLoop();
    
    
    
//     int nbNodesG2 = dendroFromG2.getNbrNodes();
//     std::vector<DendroNode<UINT32>*>& dendroNodesG2 = dendroFromG2.getDendroNodes();
//      for (int i = 0 ; i<nbNodesG2; i++){
//       DendroNode<UINT32> &curNode = *dendroNodesG2[i];
//       cout << "Noeud numero " << i << 
//      " ; getValuation = " << curNode.getValuation() << 
//      " ; getInternalNodeValuationInitial = " << curNode.getInternalNodeValuationInitial() << 
//      " ; getInternalNodeValuationFinal = " << curNode.getInternalNodeValuationFinal()<< 
//      " ; getlabel = " << curNode.getLabel() << endl;
//      if (curNode.getIsInternalNode()==1){
//        cout << "labelFilsGauche = " << curNode.getChildLeft()->getLabel() <<
//        " ; labelFilsDroit = " << curNode.getChildRight()->getLabel() << 
//        " ; neighborGauche = " << curNode.getNeighborLeft()->getLabel() << 
//        " ; neighborDroit = " << curNode.getNeighborRight()->getLabel() << endl;
//      }
//     } 
    
//     imMosaicEnd.show("imMosaicEnd");
//     Gui::execLoop();
    
//     *p1 == *p2

//     TEST_ASSERT(vals==valsTruth);
  }
};

int main(void)
{
      TestSuite ts;

      ADD_TEST(ts, Test_DendrogramConstructor);
      
      return ts.run();
}