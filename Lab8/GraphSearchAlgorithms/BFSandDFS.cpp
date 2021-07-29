//
//  CECS328Lab8.cpp
//  Lab8
//
//  Created by Melissa Hazlewood on 12/8/19.
//  Copyright © 2019 Melissa Hazlewood. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <algorithm> //for find()
#include "Graph.hpp"

using namespace std;


vector<Node*> BFS(Graph &g, Node &s);
vector<Node*> DFS(Graph &g);
vector<Node*> DFS_visit(Graph &g, Node &s, vector<Node*> &visited);
int timer = 0;
bool partA = true;
vector<Node*> sorted;

void partA_BFS();
void partA_DFS();
void partB();

int main()
{
    srand((unsigned) time(NULL));
    
//    partA_BFS();
    partA_DFS();
//    partB();
    
    return 0;
}

void partA_BFS()
{
    int v = 5;
    int e = 10;

    cout << "\nBFS" << endl;
    cout << "1.)" << endl;
    cout << "Please enter the number of vertices you would like in your graph: ";
    cin >> v;
    cout << "\nPlease enter desired number of edges: ";
    cin >> e;

    cout << "\n2.)" << endl;
    cout << "Generating edges and making graph..." << endl;
    Graph g{v,e};
    cout << "Done." << endl;

    cout << "\n3.)" << endl;
    cout << "Adjacency lists for each vertex in the resulting graph:" << endl;
    g.printAdjLists();

    int u_ind = 0;
    Node u = *g.getNodeList()[u_ind];
    vector<Node*> visited = BFS(g, u);

    cout << "\nVisited:\n\t";
    for (int i = 0; i < visited.size() - 1; i++)
    {
        cout << "v_" << visited[i]->getIndex() << ", ";
    }
    cout << "v_" << visited[visited.size()-1]->getIndex() << endl;

    cout << "\nShortest paths:" << endl;
    for (int i = 0; i < g.getNodeListSize(); i++)
    {
        cout << "v_" << g.getNodeList()[i]->getIndex() << ": <- ";
        Node * next{g.getNodeList()[i]->getParentPointer()};
        while (next != nullptr && next->getIndex() != -1)
        {
            if (next->getIndex() != u_ind)
                cout << next->getIndex() << " <- ";
            else
                cout << next->getIndex() << endl;
            next = next->getParentPointer();
        }
        cout << endl;
    }
}

void partA_DFS()
{
    int v = 5;
    int e = 5;
    cout << "\nDFS" << endl;
    cout << "1.)" << endl;
    cout << "Please enter the number of vertices you would like in your graph: ";
    cin >> v;
    cout << "\nPlease enter desired number of edges: ";
    cin >> e;

    cout << "\n2.)" << endl;
    cout << "Generating edges and making graph..." << endl;
    Graph g{v,e};
    cout << "Done." << endl;

    cout << "\n3.)" << endl;
    cout << "Adjacency lists for each vertex in the resulting graph:" << endl;
    g.printAdjLists();
    vector<Node*> visited = DFS(g);

    cout << "\nVisited:\n\t";
//    cout << "Size of visited[]: " << visited.size() << endl;
    for (int i = 0; i < visited.size() - 1; i++)
    {
        cout << "v_" << visited[i]->getIndex() << "(" << visited[i]->start << "/" << visited[i]->end << "), ";
    }
    int last = static_cast<int>(visited.size()-1);
    cout << "v_" << visited[last]->getIndex() << "(" << visited[last]->start << "/" << visited[last]->end << "), " << endl;
}

void partB()
{
//    int v = 5;
//    int e = 10;
    cout << "\nTopological Sorting" << endl;
//    cout << "New array with V = 5, E = 10:" << endl;
    partA = false;
    
//    Graph g2{v,e};
    
    //for easier testing, I'll make the graph manually instead of randomly
    Graph g2{5}; //five nodes 0-4
    //adding 4 non-cyclic edges
    g2.makeEdge(*g2.getNodeList()[0], *g2.getNodeList()[1]);
    g2.makeEdge(*g2.getNodeList()[1], *g2.getNodeList()[2]);
    g2.makeEdge(*g2.getNodeList()[2], *g2.getNodeList()[3]);
    g2.makeEdge(*g2.getNodeList()[3], *g2.getNodeList()[4]);
    
    g2.printAdjLists();
    vector<Node*> visited2 = DFS(g2);
    
    cout << "Topologically sorted nodes:" << endl;
    int last = static_cast<int>(sorted.size()-1);
    for (int i = 0; i < sorted.size()-1; i++)
    {
        cout << sorted[i]->getIndex() << "(" << sorted[i]->start << "/" << sorted[i]->end << "), ";
    }
    cout << sorted[sorted.size()-1]->getIndex() << "(" << sorted[last]->start << "/" << sorted[last]->end << "), " << endl;
}


vector<Node*> DFS(Graph &g)
{
    vector<Node*> visited = {};
    for (int i = 0; i < g.getNodeListSize(); i++)
    {
        Node * u{g.getNodeList()[i]};
        if (u->getParentPointer() == nullptr)
        {
            //set parent pointer to -1 (the index will be -1)
            Node * uParent = new Node{};
            u->setParentPointer(*uParent);
            visited.push_back(u);
            DFS_visit(g, *u, visited);
        }
    }
    return visited;
}

vector<Node*> DFS_visit(Graph &g, Node &s, vector<Node*> &visited)
{
    timer++;
    s.start = timer;
    
    for (int i = 0; i < s.getAdjListSize(); i++)
    {
        Node * u{s.getAdjList()[i]};
        if (u->getParentPointer() == nullptr)
        {
            u->setParentPointer(s);
            DFS_visit(g, *u, visited);
            visited.push_back(u);
        }
        else if (u->start != -1 && u->end == -1)
        {
            if (partA)
                cout << "Cycle found." << endl;
            else
                cout << "Cycle detected. Topological sort is impossible." << endl;
        }
    }
    
    timer++;
    s.end = timer;
    
    if (!partA)
        sorted.push_back(&s);
//        sorted.insert(sorted.begin(), &s);
    return visited;
}



vector<Node*> BFS(Graph &g, Node &s)
{
    vector<Node*> visited = {};
    queue<Node*> q = {};
    q.push(&s);
    Node * sParent = new Node{};
    s.setParentPointer(*sParent);
//    cout << s.parent->index;
    
    while(!q.empty())
    {
        Node * curr(q.front());
        q.pop();
        for (int i = 0; i < curr->getAdjListSize(); i++)
        {
            if ((*curr->getAdjList()[i]).getParentPointer() == nullptr)
            {
                (*curr->getAdjList()[i]).setParentPointer(*curr);
                q.push(&(*curr->getAdjList()[i]));
                visited.push_back(curr->getAdjList()[i]);
            }
        }
    }
    return visited;
}



//was in main
//vector<node> * reachables(BFS(g, (*g.nodeList)[0])); //creates tree of paths from the input node
//vector<vector<int>> verticesReached(2); //row 0 will be (indices of) nodes reached, row 1 will be shortest distance
//
//
//
//for (int i = 0; i < reachables->size(); i++) //for getting the distinct vertices reached
//{
//    vector<int>::iterator it = find(verticesReached[0].begin(), verticesReached[0].end(), (*reachables)[i].index);
//    if (it == verticesReached[0].end())
//    {
//        verticesReached[0].push_back((*reachables)[i].index);
//        verticesReached[1].push_back(100000);
//    }
//}
//
//for (int i = 0; i < reachables->size(); i++) //for getting the shortest distance
//{
//    cout << (*reachables)[i].index << endl;
//    vector<int>::iterator it = find(verticesReached[0].begin(), verticesReached[0].end(), (*reachables)[i].index);
//    int ind = static_cast<int>(distance(verticesReached[0].begin(), it));
//    if (it != verticesReached[0].end() && i < verticesReached[1][ind])
//    {
//        verticesReached[1][ind] = i; //DISTANCES
//    }
//}
//
//node check((*g.nodeList)[verticesReached[0][2]]);
//int dist = 0;
//while (check.parent != nullptr) //ISSUE... check.parent is always null even when copying from node in graph WITH a non-null parent pointer
//{
//    dist++;
//    check = *check.parent;
//}
//
//cout << "dist: " << dist << endl;
//
//for (int i = 0; i < verticesReached[0].size(); i++) //for printing results
//{
//    cout << "v_" << verticesReached[0][i] << " reached in ";
//    cout << verticesReached[1][i] << " steps" << endl;
//}
//
//
//delete [] reachables;
