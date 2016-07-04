//
//  BasicTypes.cpp
//  knightTrip
//
//  Created by Pavel on 02.07.16.
//
//

#include "BasicTypes.hpp"

using namespace kt;

#pragma mark - GraphVertex definition

Graph::GraphVertex::GraphVertex() : _isActive(true) {};

void Graph::GraphVertex::connectWith(int bInd) {
    _adjacentVIndices.push_back(bInd);
}
inline void Graph::GraphVertex::setIsActive(bool isActive) {
    _isActive = isActive;
}
inline bool Graph::GraphVertex::isActive() {
    return _isActive;
}
std::vector<int>::iterator Graph::GraphVertex::adjacentBegin() {
    return _adjacentVIndices.begin();
}
std::vector<int>::iterator Graph::GraphVertex::adjacentEnd() {
    return _adjacentVIndices.end();
}




#pragma mark - Graph Definition

Graph::Graph() {}

Graph::Graph(int verticesQty) {
    expand(verticesQty);
}

void Graph::expand(int newVertsQty) {
    assert(_vertices.size() < newVertsQty); //можем только расширять
    
    int oldVertsQty = _vertices.size();
    _vertices.resize(newVertsQty);
    
    for(int i = oldVertsQty; i<_vertices.size(); i++) {
        _vertices[i] = std::make_shared<GraphVertex>();
    }
    
}

void Graph::clear() {
    _vertices.clear();
}

void Graph::insertEdge(unsigned int aInd, unsigned int bInd) {
    assert(aInd < _vertices.size() && bInd < _vertices.size());
    
    auto a = _vertices[aInd];
    auto b = _vertices[bInd];
    
    a->connectWith(bInd);
    b->connectWith(aInd);
    
}

void Graph::setVertexIsActive(unsigned int vInd, bool isActive) {
    assert(vInd < _vertices.size());
    _vertices[vInd]->setIsActive(isActive);
}

bool Graph::searchMinPathBFS(unsigned int originInd, unsigned int targetInd, std::vector<int>& path) {
    assert(originInd < _vertices.size() &&
           targetInd < _vertices.size() &&
           originInd != targetInd);
    
    std::vector<int> parents(_vertices.size(), -1);
    
    std::queue<int> processCandidates;
    processCandidates.push(originInd);
    bool pathWasFound = false;
    while(!processCandidates.empty() && !pathWasFound) {
        int vIndex = processCandidates.front();
        processCandidates.pop();
        auto v = _vertices[vIndex];
        if(!v->isActive()) {
            continue;
        }
        for(auto it = v->adjacentBegin(); it != v->adjacentEnd(); it++) {
            
            if(parents[*it] < 0) {
                parents[*it] = vIndex;
                if(*it == targetInd) {
                    pathWasFound = true;
                    break;
                }
                processCandidates.push(*it);
            }
        }
    }
    
    if(parents[targetInd] >= 0) {
        int vIndConductor = targetInd;
        while (vIndConductor != originInd) {
            assert(vIndConductor >= 0);
            path.push_back(vIndConductor);
            vIndConductor = parents[vIndConductor];
        }
        path.push_back(originInd);
        std::reverse(path.begin(), path.end());
        return true;
    } else {
        return false;
    }
}