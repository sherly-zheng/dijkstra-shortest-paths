#include <iostream>
#include <fstream>
using namespace std;

class DijkstraSSS{
	public:
		int numNodes;
		int sourceNode;
		int minNode;
		int currentNode;
		int newCost;
		int** costMatrix;
		int* fatherAry;
		int* markedAry;
		int* bestCostAry;
	
		void loadCostMatrix(ifstream& inFile){
			int ni, nj, c;
			while(inFile >> ni >> nj >> c){
				costMatrix[ni][nj] = c;
			}
		}
		
		void setBestCostAry(){
			for(int i = 1; i <= numNodes; i++)
				bestCostAry[i] = costMatrix[sourceNode][i];
		}
		
		void setFatherAry(){
			for(int i = 1; i <= numNodes; i++)
				fatherAry[i] = sourceNode;
		}
		
		void setMarkedAry(){
			for(int i = 1; i <= numNodes; i++){
				if(i == sourceNode) markedAry[i] = 1;
				else markedAry[i] = 0;
			}
		}
		
		int computeCost(){
			int cost = bestCostAry[minNode] + costMatrix[minNode][currentNode];
			return cost;
		}
		
		void markMinNode(){
			markedAry[minNode] = 1;
		}
		
		void changeFather(){
			fatherAry[currentNode] = minNode;
		}
		
		void changeCost(){
			bestCostAry[currentNode] = newCost;
		}
		
		void debugPrint(ofstream& outFile2){			
			outFile2 << "The sourceNode is: " << sourceNode << endl;		
 			outFile2 << "fatherAry: " << endl;
			for (int i = 1; i <= numNodes; i++)
				outFile2 << fatherAry[i] << " ";
			outFile2 << endl; 
			
			outFile2 << "bestCostAry: " << endl;
			for (int i = 1; i <= numNodes; i++)
				outFile2 << bestCostAry[i] << " ";
			outFile2 << endl; 
			
			outFile2 << "markedAry: " << endl;
			for (int i = 1; i <= numNodes; i++)
				outFile2 << markedAry[i] << " ";
			outFile2 << endl;  
			outFile2 << "-------------------------------------" << endl;
		}
		
		void printShortestPaths(ofstream& outFile1){
			outFile1 << "The path from " << sourceNode << " to " << currentNode << ": " << currentNode;
			int father = fatherAry[currentNode];
			while(father != sourceNode){
				outFile1 << " <- " << father;
				father = fatherAry[father];
			}
			outFile1 << " <- " << sourceNode << ": cost = " << bestCostAry[currentNode] << endl;
		}
		
		int findUnmarkedMinNode(){
			int minCost = 100000;
			minNode = -1;
			for(int i = 1; i <= numNodes; i++){
				if(bestCostAry[i] < minCost && markedAry[i] < 1){
					minNode = i;
					minCost = bestCostAry[i];
				}
			}
			return minNode;
		}
};

int main(int argc, char** argv){ 
	ifstream inFile;
	ofstream outFile1, outFile2;
	DijkstraSSS G;

	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	inFile >> G.numNodes; 

	G.costMatrix = new int*[G.numNodes + 1];
	for(int i = 0; i <= G.numNodes; i++)
		G.costMatrix[i] = new int[G.numNodes + 1 ]();
	
	for(int i = 1; i <= G.numNodes; i++){
		for(int j = 1; j <= G.numNodes; j++){
			if(i != j)
				G.costMatrix[i][j] = 99999;
		}
	}
	
	G.fatherAry = new int[G.numNodes + 1]();
	G.markedAry = new int[G.numNodes + 1]();
	G.bestCostAry = new int[G.numNodes + 1]();
	
	for(int i = 1; i <= G.numNodes; i++){
		G.fatherAry[i] = i;
		G.bestCostAry[i] = 99999;
	}
	
	G.loadCostMatrix(inFile);
	G.sourceNode = 1;
	
	while(G.sourceNode <= G.numNodes){
		G.setBestCostAry();
		G.setFatherAry();
		G.setMarkedAry();
		while(G.findUnmarkedMinNode() > 0){
			G.markMinNode();
			G.debugPrint(outFile2);			
			for(int i = 1; i <= G.numNodes; i++){
				if(G.markedAry[i] < 1){
					G.currentNode = i;
					G.newCost = G.computeCost();
					if(G.newCost < G.bestCostAry[G.currentNode]){
						G.changeFather();
						G.changeCost();
					}
				}
				G.debugPrint(outFile2);
			}
		}
		G.currentNode = 1;
		while(G.currentNode <= G.numNodes){
			G.printShortestPaths(outFile1);
			G.currentNode++;
		}
		outFile1 << endl;		
		G.sourceNode++;
	}
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}