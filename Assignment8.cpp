/*
 Name: Rohaan Kamaraj
 Email: rkamara1@asu.edu
 Description: This file takes information from 3 files: one for vertices, one for edges and one for
 commands. All of this information is processed and the necessary outputs are displayed on console.
 */

#include <iostream>
#include <string>
#include "Graph.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    //creating an infile for the vertices
    ifstream infile;
    infile.open(argv[2]);
    string line = "";
    int count = 0;

    //finding how many vertices are there in order to create the initial Graph
    while (getline(infile, line)) {
        count+= 1;
    }

    Graph* graph = new Graph(count);

    count = 0;
    string info, year;

    //resets the infile
    infile.clear();
    infile.seekg(0, ios::beg);

    //populates the graph with the vertices according to the file
    while (getline(infile, line)) {
        info = line.substr(0, line.find_first_of(" "));
        year = line.substr(line .find_first_of(" ") + 1);

        graph->insertNode(info, year, count);
        count++;

    }

    //creating an infile for the edges
    ifstream infile2;
    infile2.open(argv[1]);
    string originalInfo, newInfo;

    //populates the initial graph with the edges
    while (getline(infile2, line)) {
        originalInfo = line.substr(0, line.find_first_of(" "));
        newInfo = line.substr(line.find_first_of(" ") + 1);

        graph->insertEdge(originalInfo, newInfo);

    }

    //infile for the commands textfile
    ifstream infile3;
    infile3.open(argv[3]);
    string placeholder;

    //Declaring the graph for start-graph
    Graph* g;
    int vertices, edges;

    //while loop runs as long as there are commands left to fulfill
    while (getline(infile3, line)) {
        placeholder = line.substr(0,line.find_first_of(" "));

        if (placeholder == "start-graph") {

            cout << "next command: " + line << endl << endl;

            //this is all to split the "start-graph year1 year2" command and convert the year1
            //and year2 into integers.
            string year1, year2;
            placeholder = line.substr(line.find_first_of(" ") + 1);
            year1 = placeholder.substr(0, placeholder.find_first_of(" "));
            year2 = placeholder.substr(placeholder.find_first_of(" ") + 1);

            //this is to find the vertices, edges and create the new graph
            vertices = graph->find_vertices(year1, year2);
            edges = graph->find_edges(year1, year2);
            g = new Graph(vertices);

            cout << "The graph G for the years " + year1 + "-" + year2 + " has: " << endl;
            cout << "|V| = " + to_string(vertices) + " vertices" << endl;
            cout << "|E| = " + to_string(edges) + " edges" << endl;

            //this populates the new graph using the data found in find_vertices & find_edges
            g->populate_graph(graph, vertices, edges, year1, year2);

        } else if (placeholder == "out-degree") {
            //calls out_degree on the created graph
            cout << "next command: " + line << endl << endl;
            g->out_degree(vertices, edges);
        } else if (placeholder == "diameter") {
            //calls floyd_warshall on the created graph
            cout << "next command: " + line << endl << endl;
            g->floyd_warshall();
        } else if (placeholder == "strongly-connected-components") {
            //calls stronglyConnectedComponents on the created graph
            cout << "next command: " + line << endl << endl;
            g->stronglyConnectedComponents();
        } else if (placeholder == "end-graph") {
            //calls end_graph on the created graph
            cout << "next command: " + line << endl << endl;
            g->end_graph();
        } else if (placeholder == "exit") {
            //exits the loop
            cout << "next command: " + line << endl << endl;
            break;
        }
        cout << endl;
    }

    //closes the infiles
    infile.close();
    infile2.close();
    infile3.close();

    return 0;
}

