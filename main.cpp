//
//  main.cpp
//  CrossTunnel
//
//  Created by Orkun Krand on 9/16/16.
//  Copyright © 2016 Orkun Krand. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;
//Struct to represent a state
struct Node{
	vector<int> leftPeeps; //People on the left side of the tunnel
	vector<int> rightPeeps; //People on the right side of the tunnel
	int cost; //cost associated with that state
	bool leftToRight; //boolean that represents the torch
	Node(); //default constructor
};
Node::Node(){
	leftToRight = true; //torch begins on the left
	cost = 0; //initial cost is 0
	leftPeeps.clear(); //clears everybody on the left
	rightPeeps.clear(); //clears everybody on the left
}
//operator overriding for the == operator. Required to compare 2 states(Nodes)
bool operator==(Node n1, Node n2){
	if (n1.leftPeeps == n2.leftPeeps && n1.rightPeeps == n2.rightPeeps && n1.leftToRight == n2.leftToRight)
		return true;
	else
		return false;
}
//boolean that compares costs of two nodes. Required to sort fringe based on cost
bool costComp(Node n1, Node n2){
	return n1.cost < n2.cost;
}
//Function that represents the search algorithm. Takes a character that shows what kind of search, start state, and goal state
int searchAlgo(char breadthOrDepthOrUnif, Node startNode, Node goalNode){
	int cost = 999999999; //start with a very high cost
	vector<Node> visited;
	vector<Node> fringe;
	vector<Node>::iterator it;
	fringe.push_back(startNode);
	while (fringe.size() > 0){
		if (breadthOrDepthOrUnif == 'U')
			sort(fringe.begin(), fringe.end(), costComp);
		
		Node n = fringe[0]; //Node n is the first item in the fringe
		visited.push_back(n); //add n to visited vector
		fringe.erase(fringe.begin()); //delete the first item from the fringe
		
		//Goal state checker
		if (n.rightPeeps.size() == startNode.leftPeeps.size()){
			cost = min(cost, n.cost);
			return cost;
		}
		
		if (n.leftToRight == true){ //if torch is going from left to right
			//double for loop because 2 people go from left to right
			for (int i = 0; i < n.leftPeeps.size(); ++i){
				for (int j = 1; j < n.leftPeeps.size(); ++j){
					Node newN = n; //the possible new Node
					if (i < j){
						//int1 and int2 are the people crossing the tunnel
						int int1 = *(newN.leftPeeps.begin() + i);
						int int2 = *(newN.leftPeeps.begin() + j);
						newN.cost += max(int1, int2); //add the cost to the current cost of that state
						newN.rightPeeps.push_back(int1);
						newN.rightPeeps.push_back(int2);
						sort(newN.rightPeeps.begin(), newN.rightPeeps.end()); //sort the new people so the vector is always organized so it prevents duplicates
						//erase the people from the left after moving them
						newN.leftPeeps.erase(newN.leftPeeps.begin() + j);
						newN.leftPeeps.erase(newN.leftPeeps.begin() + i);
						newN.leftToRight = false; //move the torch to the right
						if (find(visited.begin(), visited.end(), newN) == visited.end()){ //if newN is not in visited
							//if BFS, and the new state isn't in the fringe
							if (breadthOrDepthOrUnif == 'B' && find(fringe.begin(), fringe.end(), newN) == fringe.end())
								fringe.push_back(newN); //add to fringe
							//if DFS, and the new state isn't in the fringe
							else if (breadthOrDepthOrUnif == 'D' && find(fringe.begin(), fringe.end(), newN) == fringe.end())
								fringe.insert(fringe.begin(), newN); //add to fringe
							//if UCS, and the new state isn't in the fringe
							else if (breadthOrDepthOrUnif == 'U' && find(fringe.begin(), fringe.end(), newN) == fringe.end())
								fringe.push_back(newN); //add to fringe
							//if UCS, and the new state is in the fringe with a higher cost
							else if (breadthOrDepthOrUnif == 'U' && find(fringe.begin(), fringe.end(), newN)->cost > newN.cost)
								find(fringe.begin(), fringe.end(), newN)->cost = newN.cost; //change the cost of the one in the fringe to this state's cost
						}
					}
				}
			}
		}
		else{ //if torch is going from right to left
			for (int i = 0; i < n.rightPeeps.size(); ++i){ //single for loop because only 1 person will return
				Node newN = n; //the possible new Node
				int int1 = *(newN.rightPeeps.begin() + i); //int1 is the person crossing the tunnel
				newN.cost += int1;  //add the cost to the current cost of that state
				newN.leftPeeps.push_back(int1);
				sort(newN.leftPeeps.begin(), newN.leftPeeps.end()); //sort the people on the left so the vector is always organized
				newN.rightPeeps.erase(newN.rightPeeps.begin() + i); //erase the person from right after moving them
				newN.leftToRight = true; //move the torch
				if (find(visited.begin(), visited.end(), newN) == visited.end()){ //if newN is not in visited
					//if BFS, and the new state isn't in the fringe
					if (breadthOrDepthOrUnif == 'B' && find(fringe.begin(), fringe.end(), newN) == fringe.end())
						fringe.push_back(newN);
					//if DFS, and the new state isn't in the fringe
					else if (breadthOrDepthOrUnif == 'D' && find(fringe.begin(), fringe.end(), newN) == fringe.end())
						fringe.insert(fringe.begin(), newN); //add to fringe
					//if UCS, and the new state isn't in the fringe
					else if (breadthOrDepthOrUnif == 'U' && find(fringe.begin(), fringe.end(), newN) == fringe.end())
						fringe.push_back(newN); //add to fringe
					//if UCS, and the new state is in the fringe with a higher cost
					else if (breadthOrDepthOrUnif == 'U' && find(fringe.begin(), fringe.end(), newN)->cost > newN.cost)
						find(fringe.begin(), fringe.end(), newN)->cost = newN.cost; //change the cost of the one in the fringe to this state's cost
				}
			}
		}
	}
	return -1; //if the algorithm can't find a result, return -1
}


int main(int argc, const char * argv[]) {
	int numPeeps, dummyint, breadthCost, depthCost, unifCost;
	cout << "How many people are there?" << endl; //get the number of people
	cin >> numPeeps;
	Node startNode;
	Node goalNode;
	//get information about the people
	for (int i = 0; i < numPeeps; i++)
	{
		cout << "How long does person " << i+1 << " take to cross the tunnel?" << endl;
		cin >> dummyint;
		startNode.leftPeeps.push_back(dummyint);
		goalNode.rightPeeps.push_back(dummyint);
	}
	cout << endl;
	clock_t begin = clock(); //start timer
	
	sort(startNode.leftPeeps.begin(), startNode.leftPeeps.end());
	breadthCost = searchAlgo('D', startNode, goalNode);
	cout << "cost of depth first search is " << breadthCost << endl << endl;
	depthCost = searchAlgo('B', startNode, goalNode);
	cout << "cost of breadth first search is " << depthCost << endl << endl;
	unifCost = searchAlgo('U', startNode, goalNode);
	cout << "cost of uniform cost search is " << unifCost << endl << endl;
	clock_t end = clock(); //stop timer
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "execution time was " << elapsed_secs << endl; //print execution time
	
    return 0;
}
