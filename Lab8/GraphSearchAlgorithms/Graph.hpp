//
//  Graphs.hpp
//  Lab8
//
//  Created by Melissa Hazlewood on 12/10/19.
//  Copyright © 2019 Melissa Hazlewood. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

#ifndef Graphs_hpp
#define Graphs_hpp

// NOTE : NO SELF-PARENT-POINTING NODES!! aka no loops

class Node
{
private:
    /* Member variables */
    int * index; //pointer because we want these indices to be unique for each node??
    vector<Node*> adjList; //idk how to chooose b/w ptrs and non-ptrs anymore ):
    Node * parent;
    
public:
    /* Member variables */
    int start; //start time for DFS_visit
    int end; //end time for DFS_visit
    char color;
    
    /* Member functions */
    // Special functions - constructors/destructors/operators
    
    Node() //default constructor
    {
//        cout << "Node Default Constructor ";
        
        index = new int;
        *index = -1; //-1 instead of garbage data to mean "undefined" or "unassigned"
        adjList = {};
        parent = nullptr;
        start = -1;
        end = -1;
        color = 'g';
    
//        cout << this << endl;
    }
    
    Node(int ind) //parameterized constructor; parameter - index
    {
//        cout << "Node Parameterized Constructor ";
        
        index = new int;
        *index = ind;
        adjList = {};
        parent = nullptr;
        start = -1;
        end = -1;
        color = 'g';
        
//        cout << this << endl;
    }
    
    Node(const Node &otherNode) //copy constructor
    {
//        cout << "Node Copy Constructor ";
        
        this->index = new int;
        *this->index = *otherNode.index;
        this->adjList = otherNode.adjList;
        if (otherNode.parent != nullptr)
        {
            this->parent = new Node;
            this->parent = &(*otherNode.parent);
        }
        this->start = otherNode.start;
        this->end = otherNode.end;
        this->color = otherNode.color;
        
//        cout << this << endl;
    }
    
    Node& operator=(const Node& otherNode) //copy assignment operator overload
    {
//        cout << "Node Copy Assignment" << endl;
        if (this != &otherNode)
        {
            delete this->index;
            this->index = new int;
            *this->index = *otherNode.index;
            
            //should I clear out adjList first?
            this->adjList = otherNode.adjList;
            
            if (otherNode.parent != nullptr)
            {
                this->parent = new Node;
                this->parent = &(*otherNode.parent);
            }
            this->start = otherNode.start;
            this->end = otherNode.end;
            this->color = otherNode.color;
        }
        return *this;
    }
    
    ~Node() //destructor
    {
//        cout << "Node Destructor " << this << endl;
        if (index != nullptr)
            delete index;
//        if (parent != nullptr) //hopefully no need to delete parent since it will be deleted by its own destructor
//            delete parent;
        parent = nullptr; //HOWEVER, we don't want any dangling ptrs, so we can set it to null
    }
    
    // Getters and Setters
    int getIndex()
    {
        return *this->index;
    }
    
    void setIndex(int ind)
    {
        if (index != nullptr)
            *index = ind;
    }
    
    vector<Node*>& getAdjList()
    {
//        cout << "Getting adjList..." << endl;
        return adjList;
    }
    
    void setAdjList(vector<Node*> adj)
    {
        //clear out adjList first?
        //adjList = adj; //doesn't check for duplicates
        for (int i = 0; i < adj.size(); i++) //does check
        {
            addAdjacentNode(*adj[0]);
        }
    }

    Node* getParentPointer()
    {
        return parent;
    }
    
    void setParentPointer(Node & node)
    {
        parent = new Node;
        parent = &node;
    }
    
    // Other Useful Functions
    int getAdjListSize()
    {
        return (int) this->adjList.size();
    }
    
    int indexOfNeighborinAdjList(Node & node) //returns -1 if not found
    {
        for (int i = 0; i < this->adjList.size(); i++)
        {
            if (&(*adjList[i]) == &node)
            {
                return i;
            }
        }
        return -1;
    }
    
    void printAdjListIndices()
    {
        cout << "Printing " << adjList.size() << " elems in Adj List for node " << *index << "..." << endl;// << this << endl;
        cout << "v_" << *index << ":\t[ ";
        for (int i = 0; i < adjList.size() - 1; i++)
        {
            cout << *(*adjList[i]).index << ", ";
        }
        cout << *(*adjList[adjList.size()-1]).index << " ]" << endl;
    }
    
    void addAdjacentNode(Node &node)
    {
        if (indexOfNeighborinAdjList(node) == -1) //if the node is NOT already in the adj list, add it to the end
        {
            adjList.push_back(&node);
        }
        else
        {
//            cout << "Node [" << &node << "] is already a neighbor of this node [" << this << "]" << endl;
        }
    }
    
};



class Graph
{
private:
    
    /* ~Private~ Member functions */
    void makeNodes(int numV)
    {
//        cout << "Making nodes..." << endl;
        for (int i = 0; i < numV; i++)
        {
            Node * node_i = new Node{i};
            addNode(*node_i);
        }
//        cout << "Done making nodes." << endl;
    }
    
public:
    
    /* Member variables */
    
    vector<Node*> nodeList; //pointer so that when we change a node's contents, it will change in the nodeList as well (hopefully)
    int V; //number of vertices/nodes that should be in nodeList/graph
    int E; //number of edges that should be in the graph
    int E_added;
    bool directed;

    
    /* Member functions */
    // Special functions - constructors/destructors/operators

    Graph() //default constructor
    {
//        cout << "Graph Default Constructor ";

        nodeList = {};
        V = 0;
        E = 0;
        directed = true;
        
//        cout << this << endl;
    }

    Graph(vector<Node*> nodes, bool directed = true) //parameterized constructor; parameter - nodeList
    {
//        cout << "Graph Parameterized Constructor - nodeList" << endl;
        
        //nodeList = nodes; //doesn't check for duplicates yet
        setNodeList(nodes); //checks for duplicates via addNode()
        V = (int) nodes.size();
        E = 0;
        this->directed = directed;
        
//        cout << "End of Graph Parameterized Constructor - nodeList " << this << endl;
    }
    
    Graph(int V, bool directed = true) //parameterized constructor; parameter - V
    {
//        cout << "Graph Parameterized Constructor - V" << endl;
        
        //nodeList = nodes; //doesn't check for duplicates
        makeNodes(V); //does check for duplicates via addNode()
        this->V = V;
        E = 0;
        this->directed = directed;
        
//        cout << "End of Graph Parameterized Constructor - V " << this << endl;
    }
    
    Graph(int V, int E, bool directed = true) //parameterized constructor; parameter - V, E
    {
//        cout << "Graph Parameterized Constructor - V, E" << endl;
        makeNodes(V);
        this->V = V;
        this->E = E;
        while (E_added < E)
        {
            addEdge();
        }
        this->directed = directed;
    }

    Graph(const Graph& otherGraph) //copy constructor
    {
//        cout << "Graph Copy Constructor" << endl;
        
        // nodeList = otherGraph.nodeList; //doesn't check for duplicates
        setNodeList(otherGraph.nodeList); //checks for duplicates via addNode()
        V = otherGraph.V;
        E = otherGraph.E;
        directed = otherGraph.directed;
//        cout << "End of Graph Copy Constructor " << this << endl;
    }

    Graph& operator=(const Graph& otherGraph) //copy assignment operator overload
    {
//        cout << "Graph Copy Assignment" << endl;
        if (this != &otherGraph)
        {
            //clear prev nodeList contents (happens in setNodeList)
            // nodeList = otherGraph.nodeList; //doesn't check for duplicates
            setNodeList(otherGraph.nodeList); //checks for duplicates via addNode()
            V = otherGraph.V;
            E = otherGraph.E;
            directed = otherGraph.directed;
        }
//        cout << "End of Graph Copy Assignment" << endl;
        return *this;
    }

    ~Graph() //destructor
    {
//        cout << "Graph Destructor" << endl;
//        clearNodes(); //deletes nodeList's contents (since they are dynamically allocated)
//        cout << "End of Graph Destructor" << endl;
    }

    // Getters and Setters
    int getV() { return V; }
    
    void setV(int numV) { V = numV; }
    
    int getE() {  return E; }
    
    void setE(int numE) { E = numE; }
    
    vector<Node*> getNodeList()
    {
//        cout << "Getting nodeList..." << endl;
        return this->nodeList;
    }
    
    void setNodeList(vector<Node*> nodelist)
    {
//        cout << "Setting node list..." << endl;
        //delete any existing nodes
        if (nodeList.size() > 0)
        {
//            clearNodes();
        }
        //nodeList = nodelist; //doesn't check for duplicates
        for (int i = 0; i < nodelist.size(); i++) //checks for duplicates via addNode()
        {
            addNode(*nodelist[i]);
        }
//        cout << "Done setting node list." << endl;
    }
    
    // Other Useful Functions
    int getNodeListSize()
    {
        return (int) this->nodeList.size();
    }
    
    int indexOfNodeinNodeList(Node & node) //returns -1 if not found
    {
        for (int i = 0; i < this->nodeList.size(); i++)
        {
            if (&(*nodeList[i]) == &node)
            {
                return i;
            }
        }
        return -1;
    }
    
    void printAdjLists()
    {
        for (int i = 0; i < nodeList.size(); i++)
        {
            if (nodeList[i]->getAdjList().size() > 0)
            nodeList[i]->printAdjListIndices();
        }
    }
    
    void addNode(Node &node)
    {
//        cout << "Adding node " << &node << " to node list..." << endl;
        if (indexOfNodeinNodeList(node) == -1) //check that node is not already in list; ensures no duplicates
        {
            if (nodeList.size() == V)
            {
                V++; //update num vertices V if size of nodeList == V (meaning it has finished iitializing the first set of nodes yet, if applicable)
            }
            else if (nodeList.size() > V)
            {
//                cout << "Something went wrong while adding the node. There are more nodes in the nodeList than the graph is supposed to have (which is V = " << V << " nodes)." << endl;
                return; //exit so the node doesn't get added ...or should it still add?
            }
            nodeList.push_back(&node); //add the node
        }
        else
        {
//            cout << "Node [" << &node << "] is already in the nodeList of Graph " << this << endl;
        }
//        cout << "Done adding node to node list." << endl;
    }
    
    void makeEdge(Node &start, Node &end)
    {
//        cout << "Making edge between " << &start << " and " << &end << "..." << endl;
        if (&start == &end)
        {
//            cout << "Cannot make an edge between a node and itself (a loop) in this implementation." << endl;
        }
        else if (start.indexOfNeighborinAdjList(end) == -1) //check if end node is already a neighbor of start node; I think I double check this on accident (I also check in addAdjacentNode() in the node class
        {
            if (E_added == E)
            {
                E++;
            }
            else if (E_added > E)
            {
//                cout << "Something went wrong while making an edge. There have been more edges added (E_added = " << E_added << ") than kept track of (E = " << E << ")." << endl;
                return;
            }
            start.addAdjacentNode(end);
            if (!directed)
                end.addAdjacentNode(start);
            E_added++;
        }
        else
        {
//            cout << "Node [" << &end << "] is already a neighbor of " << &start << endl;
        }
//        cout << "Done making edge." << endl;
    }
    
    void addEdge()
    {
        int startInd = rand() % V;
        int endInd = rand() % V;
        
        makeEdge(*nodeList[startInd], *nodeList[endInd]);
    }
    
    void clearNodes() //deletes vertices in nodeList; don't think I need it??
    {
//        cout << "Clearing nodes..." << endl;
        for (int i = 0; i < nodeList.size(); i++)
        {
            if (nodeList[i] != nullptr)
            {
//                cout << "deleting node " << &nodeList[i] << endl;
                delete nodeList[i];
            }
        }
        //to update V or not to... it's probably going to be called right before adding new stuff anyway??
    }
    
    void printNodeList()
    {
        cout << "Node List:\n\t[ ";
        for (int i = 0; i < nodeList.size() - 1; i++)
        {
            cout << "v_" << nodeList[i]->getIndex() << ", ";
        }
        cout << "v_" << nodeList[nodeList.size() - 1]->getIndex() << " ]" << endl;
    }
};


//int main()
//{
//    srand((unsigned) time(NULL));
//    Node node1{1};
//    Node node2{2};
//    Node node3{3};
//    
//    cout << endl;
//
//    vector<Node*> nodes = {&node1, &node2, &node3};
//    cout << endl;
//    
//    cout << endl;
//    Graph empty{}; //default cotr
//    cout << endl;
//    Graph g{nodes}; //param cotr - nodelist
//    cout << endl;
//    Graph gcopy{g}; //copy cotr
//    cout << endl;
//    Graph gV{10, 10}; //param cotr - V
//    cout << endl;
//    
//    cout << endl;
//    
//    cout << gV.getNodeListSize() << endl;
//    gV.printNodeList();
//    gV.printAdjLists();
//
//}




#endif /* Graphs_hpp */





/* GRAPH CLASS STUFF */
/* Member variables */
    // V - number of vertices; int
    // E - number of edges; int
    //list of nodes/vertices - vector of nodes? pointer to vector of nodes? vector of pointers to nodes? pointer to vector of pointers to nodes?
//list of edges - vector of ordered pairs? pointer to vector or ordered pairs? vector of edge objects (if I make an edge class)?

/* Member functions */
    //default constructor
    //constructor with V param
//constructor with V and E params
    //constructor with node list param
//constructor with node list and edge list params
    //copy graph constructor
    //destructor

    //get V
    //get E
    //get node list
//get edge list

    //set V
    //set E
    //set node list ?
//set edge list ?

//get node v in node list
//set node v in node list
//add node to node list/graph
//copy node list
//is node list empty?
//get size of node list/graph; should be same as get V
//node in graph? aka if node in node list
//index of node in node list/graph

//clear node list/graph/edge list
//remove node v from node list/graph/edge list

//get edge i in edge list
//set edge i in edge list
//make edge (between two existing nodes), if not already made); add to edge list
//copy edge list
//is edge list empty?
//get size of edge list; should be the same as get E
//edge exists? aka if edge is in edge list
//index of edge in edge list

    //print nodes/node list
//print edges/edge list
    //print neighborhoods/adj lists (the associated node)
//print paths??




/* NODE CLASS STUFF */
/* Member variables */
    //index - int, pointer to int?
    //adjacency list - vector of nodes? vector of indices/ints? pointer to vector of nodes? pointer to vecor of indices/ints? linked list? BST?
    //parent - pointer to parent node

/* Member functions */
    //default constructor
    //constructor with index parameter
    //copy constructor
    //destructor
        //iterator over nodes ?

    //get index
    //get adjacency list
    //get parent - return parent node or pointer to node?

    //set index
    //set adj list
    //set parent (pointer to) a node

//get elem in adj list
//set elem in adj list
//copy adj list
//clear adj list
//is adj list empty
//add elem to adj list
//remove elem i from adj list
    //get size of adj list
        //is neighbor? aka is the node in adj list
    //indexof neighbor node in adj list, -1 if not found

    //print adj list





    /* NodeList as non-pointer */ //goes under public in graph class
//    vector<Node> nodeList;
//
//    /* Member functions */
//    // Special functions - constructors/destructors/operators
//
//    Graph() //default constructor
//    {
//        cout << "Graph Default Constructor ";
//        nodeList = {};
//        cout << this << endl;
//    }
//
//    Graph(vector<Node> nodes) //parameterized constructor; parameter - nodeList
//    {
//        cout << "Graph Parameterized Constructor" << endl;
//        nodeList = nodes;
//        cout << "End of Graph Parameterized Constructor " << this << endl;
//    }
//
//    Graph(const Graph& otherGraph) //copy constructor
//    {
//        cout << "Graph Copy Constructor" << endl;
//        nodeList = otherGraph.nodeList;
//        cout << "End of Graph Copy Constructor " << this << endl;
//    }
//
//    Graph& operator=(const Graph& otherGraph) //copy assignment operator overload
//    {
//        cout << "Graph Copy Assignment" << endl;
//        if (this != &otherGraph)
//        {
//            nodeList = otherGraph.nodeList;
//        }
//        cout << "End of Graph Copy Assignment" << endl;
//        return *this;
//    }
//
//    Graph(Graph &&otherGraph) noexcept //move constructor
//    {
//        cout << "Graph Move Constructor" << endl;
//        nodeList = otherGraph.nodeList;
//        otherGraph.nodeList = {};
//        cout << "End of Graph Move Constructor" << this << endl;
//    }
//
//    Graph& operator=(Graph&& otherGraph) //move assignment operator overload
//    {
//        cout << "Graph Copy Assignment" << endl;
//        if (this != &otherGraph)
//        {
//            nodeList = otherGraph.nodeList;
//        }
//        cout << "End of Graph Copy Assignment" << endl;
//        return *this;
//    }
//
//    ~Graph() //destructor
//    {
//        cout << "Graph Destructor" << endl;
//    }
//
//    // Getters and Setters
//    vector<Node>& getNodeList()
//    {
//        cout << "Getting nodeList..." << endl;
//        return nodeList;
//    }




// Node move stuff

/*
     Node(Node &&otherNode) noexcept //move constructor
     {
         cout << "Node Move Constructor ";
         this->index = otherNode.index;
         otherNode.index = nullptr;
         
 //        this->adjList = otherNode.adjList;
 //        otherNode.adjList = nullptr;
         this->adjList = otherNode.adjList;
         otherNode.adjList = {};
         
         if (otherNode.parent != nullptr)
         {
             this->parent = new Node;
             this->parent = &(*otherNode.parent);
             otherNode.parent = nullptr;
         }
         
         cout << this << endl;
     }
     
     Node& operator=(Node&& otherNode) //move assignment operator overload
     {
         cout << "Node Move Assignment " << endl;
         if (this != &otherNode)
         {
             delete index;
             index = move(otherNode.index);
             otherNode.index = nullptr;
             
 //            delete adjList;
 //            adjList = move(otherNode.adjList);
 //            otherNode.adjList = nullptr;
             adjList = move(otherNode.adjList);
             otherNode.adjList = {};
             
 //            if (parent != nullptr) //don't want to delete prev pointed obj bc it's still in use
 //                delete parent;
             if (otherNode.parent != nullptr)
             {
                 parent = move(otherNode.parent);
                 otherNode.parent = nullptr;
             }
         }
         return *this;
     }
 */




// Graph move stuff

/*
 Graph(Graph &&otherGraph) noexcept //move constructor
 {
     cout << "Graph Move Constructor" << endl;
     this->nodeList = otherGraph.nodeList;
     otherGraph.nodeList = nullptr;
     cout << "End of Graph Move Constructor" << this << endl;
 }

 Graph& operator=(Graph&& otherGraph) //move assignment operator overload
 {
     cout << "Graph Move Assignment" << endl;
     if (this != &otherGraph)
     {
         delete this->nodeList;
         this->nodeList = move(otherGraph.nodeList);
         otherGraph.nodeList = nullptr;
     }
     cout << "End of Graph Move Assignment" << endl;
     return *this;
 }
 */


/* Previous Try - trouble with pointers and stuff */
//class node
//{
//public:
//    int * index;
//    vector<node> * adjList; //(pointer to a) vector so we can add neighbors dynamically; pointer bc it wasn't saving the node elems outside of this scope w/o it
//    node * parent;
//
//    node()
//    {
//        index = new int;
//        adjList = new vector<node>();
//        parent = new node;
//    }
//
//    node(int ind) //constructor requires index
//    {
//        index = &ind;
//        adjList = new vector<node>();
//        parent = new node;
//    }
//
//    node(const node &other) //copy constructor
//    {
//        this->index = new int; //make room in memory for int pointer
//        *this->index = *other.index; //fill in that memory by having it point to same thing as other node
//
//        //not sure about here until..
//            this->adjList = new vector<node>();
//            *this->adjList = *other.adjList;
//
//            vector<node> * adjListcopy(other.adjList);
//            adjList = adjListcopy;
//        //here... check on vector member copying
//
//        this->parent = new node;
//        *this->parent = *other.parent;
//
//        //prob can delete stuff below but keeping just in case, til I get this stuff to work
////        if (other.parent != nullptr)
////            parent = new node();
//////        else
////            parent = other.parent;
//    }
//
//    node & operator= (const node &other) //override assignment operator just in case
//    {
//        index = other.index;
//        vector<node> * adjListcopy(other.adjList);
//        adjList = adjListcopy;
////        if (parent != nullptr)
////            delete parent;
//        parent = new node(*other.parent);
//        return *this;
//    }
//
////    ~node() //destructor for dynamically allocated elements
////    {
////        delete parent;
////        if (!(adjList->empty()))
////            delete adjList;
////    }
//
//    void addNeighbor(node &neighbor)
//    {
//        adjList->push_back(neighbor);
//    }
//
//    bool contains(node &n)
//    {
//        if (!adjList->empty())
//        {
//            for (int i = 0; i < adjList->size(); i++)
//            {
//                if (n.index == (*adjList)[i].index)
//                    return true;
//            }
//        }
//        return false;
//    }
//
//    void printAdjList()
//    {
//        cout << "[ ";
//        if (!adjList->empty()) //throws a bad access exception 4 lines down if I don't include this
//        {
//            for (int i = 0; i < adjList->size() - 1; i++)
//            {
//                cout << "v_" << (*adjList)[i].index << ", ";
//            }
//            cout << "v_" << (*adjList)[adjList->size() - 1].index;
//        }
//        cout << " ]";
//    }
//
//    int getAdjListSize()
//    {
//        return static_cast<int>(adjList->size());
//    }
//
//};
//
//class Graph
//{
//private:
//    int E_made;
//
//    void initializeNodes()
//    {
//        for (int i = 0; i < V; i++)
//        {
//            node * v = new node(i);
//            nodeList->push_back(*v);
//        }
//    }
//
//    void makeEdge(node &start, node &end)
//    {
//        if (!start.contains(end) && start.getAdjListSize() < V)
//        {
//            start.addNeighbor(end);
//            E_made++;
//        }
//    }
//
//    void generateEdges()
//    {
//        srand((unsigned) time(NULL)); //SHOULD ONLY BE CALLED ONCE IN A PROGRAM!!
//
//        int start;
//        int end;
//
//        while (E_made < E)
//        {
//            //pick a random vertex['s index] to be the source of the edge
//            start = rand() % V;
//            //pick another random vertex['s index] to be the sink of the edge (did I use that right?)
//            end = rand() % V; //can it be the same as start? like can we have loops? rn we can... ask her later just in case
//
//            makeEdge((*nodeList)[start], (*nodeList)[end]); //increments E_made in makeEdge()
//        }
//    }
//
//public:
//    int V; //number of vertices
//    int E; //number of edges
//    vector<node> * nodeList;
//
//    Graph() //default constructor; probably don't need to define explicitly but w/e
//    {
//        V = 0;
//        E = 0;
//        E_made = 0;
//        initializeNodes();
//        generateEdges();
//    }
//
//    Graph(int v, int e) //constructor of graph with chosen V/E, but random edges
//    {
//        V = v;
//        E = e;
//        E_made = 0;
//        initializeNodes();
//        generateEdges();
//    }
//
//    void printAdj()
//    {
//        for (int i = 0; i < V; i++)
//        {
//            cout << "\t  ";
//            cout << "v_" << i << ": ";
//            (*nodeList)[i].printAdjList();
//            cout << endl;
//        }
//    }
//};
