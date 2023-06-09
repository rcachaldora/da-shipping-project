#include <cmath>
#include "Graph.h"
#include "VertexEdge.h"
#include "MutablePriorityQueue.h"
#include "File_Reader.h"

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}


Vertex * Graph::findVertex(const int &id) const {
    for (auto v : vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}


int Graph::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}

bool Graph::addVertex(const int &id) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id));
    return true;
}


bool Graph::addEdge(const int &sourc, const int &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

bool Graph::addBidirectionalEdge(const int &sourc, const int &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

double Graph::Dijkstra(Vertex *v1, Vertex *v2) {

    for (Vertex *v: this->vertexSet) {
        v->setVisited(false);
        v->setDist(INF);
    }

    std::priority_queue<Vertex *> pq;
    v1->setDist(0);
    pq.push(v1);

    while (!pq.empty()) {

        Vertex *v = pq.top();
        pq.pop();

        if(v == v2) return v2->getDist();

        for (Edge *e: v->getAdj()) {
            if (e->getOrig()->getDist() + e->getWeight() < e->getDest()->getDist()) {
                e->getDest()->setDist(e->getOrig()->getDist() + e->getWeight());
                pq.push(e->getDest());
            }
        }

    }
    return v2->getDist();

}



void Graph::createAdjMatrix(){
    adjMatrix.clear();
    adjMatrix = std::vector<std::vector<double>>(vertexSet.size(),std::vector<double>(vertexSet.size(),0));
    for(unsigned long i = 0; i < adjMatrix.size(); i++){
        for(unsigned long j = i+1; j < adjMatrix.size(); j++){
            double dist = Dijkstra(vertexSet[i],vertexSet[j]);
            adjMatrix[i][j] = dist;
            adjMatrix[j][i] = dist;
        }
    }
}

void Graph::backtracking_algorithm_aux(int currPos, int count, double cost, double &ans, std::vector<Vertex*> &path)
{
    if (count == vertexSet.size())
    {
        for (auto e : vertexSet[currPos]->getAdj())
        {
            if (e->getDest()->getId() == 0)
            {
                double new_cost = cost + e->getWeight();
                if (new_cost < ans)
                {
                    ans = new_cost;
                    path = paths[currPos];
                    return;
                }
            }
        }
    }

    for (auto e : vertexSet[currPos]->getAdj())
    {
        if (!vertexSet[e->getDest()->getId()]->isVisited())
        {
            vertexSet[e->getDest()->getId()]->setVisited(true);

            paths[e->getDest()->getId()] = paths[currPos];
            paths[e->getDest()->getId()].push_back(e->getDest());

            backtracking_algorithm_aux(e->getDest()->getId(), count + 1, cost + e->getWeight(), ans, path);
            vertexSet[e->getDest()->getId()]->setVisited(false);
        }
    }
}

double Graph::exercise1()
{
    for (int i = 0; i < vertexSet.size(); i++)
    {
        vertexSet[i]->setVisited(false);
    }

    vertexSet[0]->setVisited(true);
    double ans = INT_MAX;
    std::vector<Vertex*> path;

    paths.resize(vertexSet.size());
    paths[0].push_back(vertexSet[0]);

    backtracking_algorithm_aux( 0, 1, 0, ans, path);


    return ans;
}




void Graph::Prims(std::vector<std::vector<Edge*>>& mst){
    for(auto v : vertexSet) {
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(false);
    }

    Vertex* s = vertexSet[0];
    s->setDist(0);

    MutablePriorityQueue<Vertex> q;
    q.insert(s);

    while( ! q.empty() ) {
        auto v = q.extractMin();
        v->setVisited(true);
        for(auto &e : v->getAdj()) {
            Vertex* w = e->getDest();
            if (!w->isVisited()) {
                auto oldDist = w->getDist();
                if(e->getWeight() < oldDist) {
                    w->setDist(e->getWeight());
                    w->setPath(e);
                    if (oldDist == INF) {
                        q.insert(w);
                    }
                    else {
                        q.decreaseKey(w);
                    }
                }
            }
        }
    }

    for (auto v : vertexSet) {
        v->setVisited(false);
    }

    for (auto &i : mst) {
        i.clear();
    }

    for (auto &v : vertexSet) {
        if (!v->isVisited()) {
            v->setVisited(true);
            auto e = v->getPath();
            if (e != nullptr) {
                auto src = e->getOrig();
                auto dest = e->getDest();
                mst[src->getId()].push_back(e);
                mst[dest->getId()].push_back(e);

            }
        }
    }


}



void Graph::DFS(int id, std::vector<Vertex*>& path, std::vector<std::vector<Edge*>>& mst, std::vector<bool>& visited) {

    Vertex* vertex = vertexSet[id];
    visited[id] = true;
    path.push_back(vertex);

    for (Edge* e : (mst)[id]) {
        if (!visited[e->getDest()->getId()]) {
            DFS(e->getDest()->getId(), path, mst, visited);
        }
    }
}

double Graph::get_distance(Vertex* v1, Vertex* v2){

        double dist = 0;
        bool coordinates = true;

    for( Edge* e : v1->getAdj()){

        if(e->getDest() == v2){
            coordinates = false;
            dist += e->getWeight();

        }

    }
    if(coordinates){
        double lat1 = 1,lat2 = 2,lon1 = 3,lon2 = 4;
        lat1 = v1->vertexCoordInfo.latitude;
        lat2 = v2->vertexCoordInfo.latitude;
        lon1 = v1->vertexCoordInfo.longitude;
        lon2 = v2->vertexCoordInfo.longitude;
        dist += haversine(lat1,lon1,lat2,lon2);

    }

    return dist;
}

double Graph::distance_calc(std::vector<Vertex*>& path){
    double dist = 0;
    bool coordinates = true;


    for(int i = 0 ; i < path.size() - 1; i++ ){

        dist += get_distance(path[i],path[i+1]);

    }
    return dist;
}

double Graph::haversine(double lat1, double lon1, double lat2, double lon2){
    // distance between latitudes
    // and longitudes
    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}

void Graph::complete_matrix() {

    int size = vertexSet.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                distMatrix[i][j] = 0.0;
            } else {
                double dist = get_distance(vertexSet[i], vertexSet[j]);
                distMatrix[i][j] = dist;
                distMatrix[j][i] = dist;
            }
        }
    }
}


int Graph::Nearest_unvisited_vertex(std::vector<bool>& visited,int cur){

    int size = vertexSet.size();

    int nearest_vertex = -1;

    double nearest_distance = INF;

    for( int i = 0; i < size; i++){

        if(!visited[i] &&  distMatrix[cur][i] < nearest_distance){

            nearest_vertex = i;

            nearest_distance = distMatrix[cur][i];
        }
    }


    return nearest_vertex;
}

void Graph::nearest_neighbor_tour(std::vector<bool>& visited,std::vector<Vertex*>& path){

    int size = vertexSet.size();
    int cur = 0;
    Vertex * inicial_vertex = vertexSet[0];
    visited[cur] = true;
    path.push_back(inicial_vertex);
    for(int i = 1 ; i < size ; i++ ){
        int next_vertex = Nearest_unvisited_vertex(visited, cur);
         visited[next_vertex] = true;
        path.push_back(vertexSet[next_vertex]);
        cur = next_vertex;
    }
    path.push_back(inicial_vertex);

}

double Graph::tour_length( std::vector<Vertex*>& tour) {
    double length = 0.0;
    int size = vertexSet.size();
    for (int i = 0; i < size - 1; ++i) {
        length += distMatrix[tour[i]->getId()][tour[i + 1]->getId()];
    }
    length += distMatrix[tour[size - 1]->getId()][tour[0]->getId()];
    return length;
}

void Graph::tsp_2opt( std::vector<Vertex*>& path) {
    std::vector<Vertex*> new_tour = path;
    const int n = path.size();
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 1; i < n - 2; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (j - i == 1) {
                    continue;
                }
                std::reverse(new_tour.begin() + i, new_tour.begin() + j);
                double new_length = tour_length(new_tour);
                if (new_length < tour_length(path)) {
                    path = new_tour;
                    improved = true;
                } else {
                    std::reverse(new_tour.begin() + i, new_tour.begin() + j);
                }
            }
        }
    }

}



double Graph::exercise3(){

    double dist;
    int size = vertexSet.size();
    std::vector<Vertex*> path;
    std::vector<bool> visited(size, false);

    distMatrix = std::vector<std::vector<double>>(size, std::vector<double>(size, 0.0));


    complete_matrix();
   nearest_neighbor_tour(visited,path);
    tsp_2opt(path);


    dist = distance_calc(path);

    /*for(int i = 0; i < path.size(); i++){
        std::cout << path[i]->getId() << " ";
    }*/

    return dist;
}


double Graph::exercise2(){

    double dist;
    int size = vertexSet.size();
    std::vector<bool> visited(size, false);
    std::vector<std::vector<Edge*>> mst(vertexSet.size());
    std::vector<Vertex*> path;
    Prims(mst);

    DFS(vertexSet[0]->getId(), path, mst, visited);
    path.push_back(vertexSet[0]);

    dist = distance_calc(path);



    return dist;
}

double Graph::fun(int i , int mask){

    int n = vertexSet.size();

    if(mask == ((1 << i) | 1))
        return adjMatrix[0][i];

    if(memo[i][mask] != 0)
        return memo[i][mask];

    double res = INF;

    for( int j = 0; j < n; j++){

        if((mask & (1 << j)) && j != i && j != 0)
            res = std::min(res, fun(j,mask & (~(1 << i))) + adjMatrix[j][i]);
    }
    memo[i][mask] = res;
    return res;
}

double Graph::exercise3_2(){

    int size = vertexSet.size();
    createAdjMatrix();
    double ans = std::numeric_limits<double>::max();

    std::vector<double> tmp(pow(2,size),0);
    memo = std::vector<std::vector<double>>(size,tmp);

    for(int i = 1 ; i < size; i++ ){

        ans = std::min(ans, fun(i, (1 << (size)) - 1)
                            + adjMatrix[i][0]);

    }

    return ans;
}

Graph::~Graph() {
    //deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}