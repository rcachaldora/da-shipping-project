//
// Created by ritac on 24/05/2023.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "File_Reader.h"
#include "../../da-shipping-project/src/VertexEdge.h"

File_Reader::File_Reader() {

}

void File_Reader::readEdges(const std::string& filename) {
    std::string origem, destino, distancia;
    int Capacity;
    std::string line;
    std::ifstream ifs("../data/" + filename);
    if (!ifs.is_open()) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return;
    }

    std::getline(ifs, line, '\n');
    if(!(line[0]>='a' && line[0]<='z'))
        goto label;


    while (std::getline(ifs, line, '\n')) {
        label:
        std::regex rgx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");
        std::sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);

        origem = *iter;
        iter++;
        destino = *iter;
        iter++;
        distancia = *iter;

        int sourceId = stoi(origem);
        int destId = stoi(destino);
        float weight = stof(distancia);

        // Create new vertices and edges
        Vertex* sourceVertex = graph.findVertex(sourceId);
        Vertex* destVertex = graph.findVertex(destId);
        if(sourceVertex== nullptr){
            sourceVertex = new Vertex(sourceId);
            graph.vertexSet.push_back(sourceVertex);
            allVertexes.insert(sourceVertex);
        }
        if(destVertex == nullptr){
            destVertex = new Vertex(destId);
            graph.vertexSet.push_back(destVertex);
            allVertexes.insert(destVertex);
        }

        Edge* newEdge = new Edge(sourceVertex, destVertex, weight);

        //talvez se tire isto daqui
        allEdges.insert(newEdge);


        // Add edge to the graph
        graph.addBidirectionalEdge(sourceId, destId, weight);
        //std::cout << origem << " " << destino << " " << distancia << " " << std::endl;
    }
    ifs.close();
}

void File_Reader::readTourism() {
    std::string origem, destino, distancia, labelOrigem, labelDestino;
    int Capacity;
    std::string line;
    std::ifstream ifs("../data/Toy-Graphs/tourism.csv");
    if (!ifs.is_open()) {
        std::cerr << "Error: Failed to open file tourism.csv" << std::endl;
        return;
    }
    std::getline(ifs, line, '\n');
    while (std::getline(ifs, line, '\n')) {
        std::regex rgx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");
        std::sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);

        origem = *iter;
        iter++;
        destino = *iter;
        iter++;
        distancia = *iter;
        iter++;
        labelOrigem = *iter;
        iter++;
        labelDestino = *iter;

        int sourceId = stoi(origem);
        int destId = stoi(destino);
        float weight = stof(distancia);

        // Create new vertices and edges
        Vertex* sourceVertex = graph.findVertex(sourceId);
        Vertex* destVertex = graph.findVertex(destId);
        if(sourceVertex== nullptr){
            sourceVertex = new Vertex(sourceId);
            graph.vertexSet.push_back(sourceVertex);
            allVertexes.insert(sourceVertex);
        }
        if(destVertex == nullptr){
            destVertex = new Vertex(destId);
            graph.vertexSet.push_back(destVertex);
            allVertexes.insert(destVertex);
        }

        Edge* newEdge = new Edge(sourceVertex, destVertex, weight);
        allEdges.insert(newEdge);
        vertexNames.emplace(labelOrigem, sourceVertex);

        // Add edge to the graph

        graph.addBidirectionalEdge(stoi(origem), stoi(destino), stoi(distancia));
        //std::cout << origem << " " << destino << " " << distancia << " " << std::endl;
    }
    ifs.close();
}

void File_Reader::readRealNodes(const std::string& filename) {
    std::string id, longitude, latitude;
    int Capacity;
    std::string line;
    std::ifstream ifs("../data/Real-world Graphs/" + filename);
    if (!ifs.is_open()) {
        std::cerr << "Error: Failed to open file tourism.csv" << std::endl;
        return;
    }
    std::getline(ifs, line, '\n');
    while (std::getline(ifs, line, '\n')) {
        std::regex rgx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");
        std::sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);

        id = *iter;
        iter++;
        longitude = *iter;
        iter++;
        latitude = *iter;


        int Id = stoi(id);
        double latId = stod(latitude);
        double longId = stod(longitude);
        Vertex::vertexCoord coord = {Id, latId, longId};

        Vertex* sourceVertex = graph.findVertex(Id);
        sourceVertex->vertexCoordInfo = coord;

        graph.vertexCoordMap.emplace(sourceVertex, coord);

        //std::cout << id << " " << latitude << " " << longitude << " " << std::endl;
    }
    ifs.close();
}