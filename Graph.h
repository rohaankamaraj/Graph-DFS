/*
 Name: Rohaan Kamaraj
 Email: rkamara1@asu.edu
 Description: This file contains a Graph implementation with the following functionality:
 DFS, strongly-connected-components, and floyd-warshall.
 */

#include <string>
#include <iostream>
#include <iomanip>
#include "LinkedList.h"

//GRAPH IMPLEMENTATION
class Graph {

private:
    //private instance data
    LinkedList* graph;
    int size;

public:
    //public methods
    Graph(int);
    void end_graph();
    void insertEdge(string, string);
    void insertNode(string, string, int);
    int find_vertices(string, string);
    int find_edges(string, string);
    void populate_graph(Graph*, int, int, string, string);
    void out_degree(int, int);
    void floyd_warshall();
    void DFS(int[]);
    void DFS_T(int[]); //DFS for transposed graph
    void DFS_VISIT(int, int&, string[], string[], int[], int[]);
    void DFS_VISIT(int, int&, string[], string[], int[], int[], int&); //DFS_VISIT for transposed graph
    void stronglyConnectedComponents();
    Node *getHead(int i);
    int getLength(int i);
    void transpose();
};

//Graph constructor
Graph::Graph(int userSize) {
    graph = new LinkedList[userSize];
    size = userSize;
}

//removes all information from the graph
void Graph::end_graph() {
    for (int i = 0; i < size; i++) {
        graph[i].removeAll();
    }
}

//Finds where the pair should be inserted in the Graph, and inserts it in that place
void Graph::insertEdge(string originalInfo, string newInfo) {
    string newPublicationYear = "";
    //find old and new publicationYear so i can insert a node
    for (int i = 0; i < size; i++) {
         if (graph[i].getHead()->getInformation() == newInfo) {
            newPublicationYear = graph[i].getHead()->getPubYear();
        }
    }

    //insert the edges
    for (int i = 0; i < size; i++) {
        if (graph[i].getHead()->getInformation() == originalInfo) {
            graph[i].insertEdge(newInfo, newPublicationYear);
        }
    }
}

//Used to initially populate the adjacency list with all the vertices.
void Graph::insertNode(string information, string pubYear, int placement) {
    graph[placement].insertEdge(information, pubYear);
}

//used to calculate how many vertices the new Graph should contain based on data from the initial graph
//and the years passed in
int Graph::find_vertices(string firstYear, string lastYear) {

    int intFirstYear, intLastYear;
    intFirstYear = stoi(firstYear), intLastYear = stoi(lastYear);

    int vertices = 0;

    //count how big the new Graph should be
    for (int i = 0; i < size; i++) {
        int year = stoi(graph[i].getHead()->getPubYear());
        if (year >= intFirstYear && year <= intLastYear) {
            vertices++;
        }
    }

    return vertices;

}

//used to calculate how many edges the new Graph should contain based on data from the initial graph
//and the years passed in
int Graph::find_edges(string firstYear, string lastYear) {
    int edges = 0;

    int intFirstYear, intLastYear;
    intFirstYear = stoi(firstYear), intLastYear = stoi(lastYear);

    //go into each vertice that is to be added in the new graph, inside the original graph, and see
    //if their edges should be in the new graph as well
    for (int i = 0; i < size; i++) {
        int year = stoi(graph[i].getHead()->getPubYear());
        if (graph[i].getLength() > 1 && (year >= intFirstYear && year <= intLastYear)) {
            Node* node = graph[i].getHead();
            for (int j = 1; j < graph[i].getLength(); j++) {
                node = node->getNext();
                int year2 = stoi(node->getPubYear());
                if (year2 >= intFirstYear && year2 <= intLastYear) {
                    edges++;
                }
            }
        }
    }

    return edges;

}

//returns the head of a LinkedList in the Graph based on the index passed in
Node* Graph::getHead(int i) {
    return graph[i].getHead();
}

//returns the count of a LinkedList in the Graph based on the index passed in
int Graph::getLength(int i) {
    return graph[i].getLength();
}

//this method actually populates the new Graph using all the data from find_vertices and find_edges
void Graph::populate_graph(Graph* userGraph, int vertices, int edges, string firstYear, string lastYear) {
    int intFirstYear, intLastYear;
    intFirstYear = stoi(firstYear), intLastYear = stoi(lastYear);
    int gSize = 0;
    //populate the graph with vertices
    for (int i = 0; i < userGraph->size; i++) {
        int year = stoi(userGraph->getHead(i)->getPubYear());
        if (year >= intFirstYear && year <= intLastYear) {
            graph[gSize].insertEdge(userGraph->getHead(i)->getInformation(),userGraph->getHead(i)->getPubYear());
            gSize++;
        }
    }

    gSize = 0;
    //add the edges in
    for (int i = 0; i < userGraph->size; i++) {
        int year = stoi(userGraph->getHead(i)->getPubYear());
        if (userGraph->getLength(i) > 1 && year >= intFirstYear && year <= intLastYear) {
            Node* node = userGraph->getHead(i);
            for (int j = 1; j < userGraph->getLength(i); j++) {
                node = node->getNext();
                int year2 = stoi(node->getPubYear());
                if (year2 >= intFirstYear && year2 <= intLastYear) {
                    graph[gSize].insertEdge(node->getInformation(),node->getPubYear());
                }
            }
            gSize++;
        } else if (userGraph->getLength(i) == 1 && year >= intFirstYear && year <= intLastYear) {
            gSize++;
        }
    } // used similar logic as find_vertices & find_edges to populate the vertices and edges respectively

}

//computes the out-degree distribution of the graph
void Graph::out_degree(int vertices, int edges) {

    double average = (double) edges / (double) vertices;
    cout << "The graph G has average out-degree " + to_string(edges) + "/" + to_string(vertices) +
    " = " << setprecision(4) << average << "." << endl;

    cout << "The out-degree distribution is:" << endl;
    cout << "Out-degree: Number of vertices" << endl;

    // determine how many out-degree options there are
    int max = 0;
    for (int i = 0; i < size; i++) {
        int length = graph[i].getLength() - 1;
        if (length > max) {
            max = length;
        }
    }

    //creates an integer array for storing the number of vertices per out-degree
    int out[max + 1];

    //makes every slot of the array 0 initially.
    for (int i  = 0; i < max + 1; i++) {
        out[i] = 0;
    }

    //finds the number of vertices for each out-degree
    for (int i = 0; i <= max; i++) {
        for (int j = 0; j < size; j++) {
            if ((graph[j].getLength() - 1) == i) {
                out[i]++;
            }
        }
    }

    //prints the out degree : number of vertices
    for (int i = 0; i <= max; i++) {
        cout << to_string(i)  + " : " + to_string(out[i]) << endl;
    }

}

//this method executes the floyd_warshall algorithm and finds the longest shortest path
void Graph::floyd_warshall() {

    //create the 2d array matrix
    int matrix[size][size];

    //populating the matrix initially with its values from the adjacency list
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = stoi(graph[j].getHead()->getInformation());
        }
    }

    //this calculates what values should be 1 using the info of the adjacency list
    for (int i = 0; i < size; i++) {
        Node* node = graph[i].getHead();
        if (graph[i].getLength() > 1) {
            node = node->getNext();
        }
        for (int j = 0; j < size; j++) {
            int y = stoi(node->getInformation());
            if (y == matrix[i][j]) {
                matrix[i][j] = 1;
                node = node->getNext();
                if (node == NULL) {
                    break;
                }
                //for if the next element in the adjacency list is smaller than the current one
                if (y > stoi(node->getInformation())) {
                    for (int c = 0; c < size; c++) {
                        if (stoi(node->getInformation()) == matrix[i][c]) {
                            matrix[i][c] = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    //makes every diagonal index of the matrix 0
    for (int i = 0; i < size; i++) {
        matrix[i][i] = 0;
    }

    //makes every element of the matrix that isn't 0 or 1, 999.
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != 1) {
                matrix[i][j] = 999;
            }
        }
    }

    //this is the meat of the floyd-warshall algorithm
    for (int k = 0; k < size; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (matrix[i][j] > matrix[i][k] + matrix[k][j]) {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
            }
        }
    }

    int diameter = 0;

    //finds the longest shortest path as the diameter
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] > diameter && matrix[i][j] != 999) {
                diameter = matrix[i][j];
            }
        }
    }

    cout << "The Graph g has diameter " + to_string(diameter) + "." << endl;

}

//DepthFirstSearch to be used for original graph in stronglyConnectedComponents
void Graph::DFS(int finished[]) {

    //the arrays needed to collect data
    string colors[size];
    string pi[size];
    int discovered[size];

    //paints every node white, it's pi is none, and hasn't been discovered nor finished yet
    for (int i = 0; i < size; i++) {
        colors[i] = "white";
        pi[i] = "none";
        discovered[i] = 0;
        finished[i] = 0;
    }

    int time = 0;

    for (int i = 0; i < size; i++) {
        if (colors[i] == "white") {
            DFS_VISIT(i, time, colors, pi, discovered, finished); //call DFS_VISIT for every white node
        }
    }

}

//DFS_VISIT is part of DFS, which is used for the original graph in stronglyConnectedComponents
void Graph::DFS_VISIT(int i, int& time, string colors[], string pi[], int discovered[], int finished[]) {
    //paint the current node gray, as well as store it's discovered data
    colors[i] = "gray";
    time++;
    discovered[i] = time;
    Node* node = graph[i].getHead();

    //explore adjacent node(s)
    for (int r = 1; r < graph[i].getLength(); r++) {
        //get location of adjacent edge in the LL
        node = node->getNext();
        int loc = 0;
        string adjInfo = node->getInformation();
        for (int j = 0; j < size; j++) {
            if (graph[j].getHead()->getInformation() == adjInfo) {
                loc = j;
            }
        }
        if (colors[loc] == "white") {
            pi[loc] = graph[i].getHead()->getInformation();
            DFS_VISIT(loc, time, colors, pi, discovered, finished); //recursive call
        }
    }
    //finally, paints node black and stores it's finished time
    colors[i] = "black";
    time++;
    finished[i] = time;
}

//this method calls DFS on the original graph, transposes it, and calls the transposed DFS method on
//the transposed graph. In other words, it finds the strongly connected components of the graph and
//displays its information.
void Graph::stronglyConnectedComponents() {
    //call DFS on original graph
    int finished[size];
    DFS(finished);

    //get transpose of original graph & preserve the original graph
    LinkedList* original = graph;
    transpose();

    //write and then call new DFS method with the transpose of graph
    //DFS_T also outputs the information for strongly connected components
    DFS_T(finished);
    graph = original;
}

//this helper method transposes the graph for stronglyConnectedComponents()
void Graph::transpose() {

    //create transpose graph
    LinkedList* transpose = new LinkedList[size];

    //fill in indexes in transpose graph
    for (int i = 0; i < size; i++) {
        string info = graph[i].getHead()->getInformation();
        string year = graph[i].getHead()->getPubYear();
        transpose[i].insertEdge(info, year);
    }

    //fill in edges in transpose graph
    for (int i = 0; i < size; i++) {
        Node* original = graph[i].getHead();
        Node* node = graph[i].getHead();
        for (int j = 1; j < graph[i].getLength(); j++) {
            node = node->getNext();
            for (int k = 0; k < size; k++) {
                if (graph[k].getHead()->getInformation() == node->getInformation()) {
                    transpose[k].insertEdge(original->getInformation(), original->getPubYear());
                }
            }
        }
    }

    //make graph the transposed graph (original graph is preserved in stronglyConnectedComponents)
    graph = transpose;

}

//This DepthFirstSearch is for the transposed graph in stronglyConnectedComponents
//also prints out all the information for strongly connected components
void Graph::DFS_T(int finished[]) {
    //the 4 arrays needed to collect data
    string colors[size];
    string pi[size];
    int discovered[size];
    int newFinished[size];

    //paints every node white, it's pi is none, and hasn't been discovered nor finished yet
    for (int i = 0; i < size; i++) {
        colors[i] = "white";
        pi[i] = "none";
        discovered[i] = 0;
        newFinished[i] = 0;
    }

    int time = 0;
    int components = 0;

    //placement is used to find the index of the finishing times from greatest to smallest
    int placement[size];

    //populate placement
    for (int i = 0; i < size; i++) {
        int max = 0;
        int place = 0;
        for (int j = 0; j < size; j++) {
            if (finished[j] > max) {
                max = finished[j];
                place = j;
            }
        }
        placement[i] = place;
        finished[place] = 0;
    }

    //this is to find number of components, I do this before finding the size of each component
    //so I can create an array of size number of components, for the component distribution
    for (int i = 0; i < size; i++) {
        if (colors[placement[i]] == "white") {
            DFS_VISIT(placement[i], time, colors, pi, discovered, newFinished); //call DFS_VISIT for every white node
            components++;
        }
    }

    cout << "The graph G has " + to_string(components) + " strongly connected component(s)" << endl << endl;
    int outDegree[components];
    int outDegree2[components];

    //reset all the arrays
    for (int i = 0; i < size; i++) {
        colors[i] = "white";
        pi[i] = "none";
        discovered[i] = 0;
        newFinished[i] = 0;
    }

    int count = 1;
    int tracker = 0;
    //this is to find the size of each component
    for (int i = 0; i < size; i++) {
        if (colors[placement[i]] == "white") {
            DFS_VISIT(placement[i], time, colors, pi, discovered, newFinished, count); //call DFS_VISIT
            outDegree[tracker] = count;
            outDegree2[tracker] = count;
            tracker++;
        }
        count = 1;
    }

    //print out the component and size
    for (int i = 0; i < components; i++) {
        cout << "Component " << i << " size: " << outDegree[i] << endl;
    }
    cout << endl;

    //print out the strongly connected component distribution
    cout << "The strongly connected component distribution is:" << endl;
    cout << "Size : Number of components" << endl;

    //determine how many degree options there are
    int options = outDegree[0];
    int numOptions = 1;
    for (int i = 1; i < components; i++) {
        if (outDegree[i] != options) {
            numOptions++;
        }
    }

    //create two arrays of number of options size
    int componentSize[numOptions];
    int numComponents[numOptions];

    //initially populate the arrays with 0
    for (int i = 0; i < numOptions; i++) {
        componentSize[i] = 0;
        numComponents[i] = 0;
    }

    //get the correct order of the componentSize array
    for (int i = 0; i < numOptions; i++) {
        int smallest = outDegree[0];
        int index = 0;
        for (int j = 1; j < components; j++) {
            if (smallest > outDegree[j]) {
                smallest = outDegree[j];
                index = j;
            } else if (smallest == outDegree[j]) {
                outDegree[j] = 999;
            }
        }
        componentSize[i] = smallest;
        outDegree[index] = 999;
    }

    //get the correct order of numComponents array
    for (int i = 0; i < numOptions; i++) {
        int count = 0;
        for (int j = 0; j < components; j++) {
            if (outDegree2[j] == componentSize[i]) {
                count++;
            }
        }
        numComponents[i] = count;
    }

    //prints out the distribution itself
    for (int i = 0; i < numOptions; i++) {
        cout << componentSize[i] << " : " << numComponents[i] << endl;
    }


}

//DFS_VISIT is part of DFS, which is used for stronglyConnectedComponents.
//This is particularly used for the transposed graph, I added an extra integer to
//get the size for each strongly connected component
void Graph::DFS_VISIT(int i, int& time, string colors[], string pi[], int discovered[], int finished[], int& count) {
    //paint the current node gray, as well as store it's discovered data
    colors[i] = "gray";
    time++;
    discovered[i] = time;
    Node* node = graph[i].getHead();

    //explore adjacent node(s)
    for (int r = 1; r < graph[i].getLength(); r++) {
        //get location of adjacent edge in the LL
        node = node->getNext();
        int loc = 0;
        string adjInfo = node->getInformation();
        for (int j = 0; j < size; j++) {
            if (graph[j].getHead()->getInformation() == adjInfo) {
                loc = j;
            }
        }

        if (colors[loc] == "white") {
            count++;
            pi[loc] = graph[i].getHead()->getInformation();
            DFS_VISIT(loc, time, colors, pi, discovered, finished, count); //recursive call
        }
    }
    //finally, paints node black and stores it's finished time
    colors[i] = "black";
    time++;
    finished[i] = time;
}
