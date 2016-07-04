//
//  KnightTripModel.cpp
//  knightTrip
//
//  Created by Pavel on 02.07.16.
//
//

#include "KnightTripModel.hpp"
#include "KnightTripView.hpp"



using namespace kt;
USING_NS_CC;

#pragma mark - Init

KnightTripModel::KnightTripModel() {}

void KnightTripModel::initialize() {
    assert(!_view.expired()); //ожидаем, что перед инициализацией MVC компоненты уже связаны
}

#pragma mark - Service

void KnightTripModel::reloadModel() {
    
    int cellsQty = CELLS_DIM * CELLS_DIM;
    
    _cellsInfo.clear();
    _cellsInfo.resize(cellsQty);
    
    _startCellIndex = -1;
    _finishCellIndex = -1;
    
    createKnightPathGraph(CELLS_DIM);
    
    _view.lock()->rebuildChessBoard();
    

}

void KnightTripModel::setStatusForCell(unsigned int cellIndex, ChessBoardCellStatus newStatus) {
    assert(cellIndex < _cellsInfo.size());
    
    if(_cellsInfo[cellIndex].status == newStatus) {
        return;
    }
    
    if(newStatus == ChessBoardCellStatus::START_IS_HERE && _startCellIndex >= 0) {
        _cellsInfo[_startCellIndex].status = ChessBoardCellStatus::NORMAL;
        _view.lock()->rebuildChessBoardCell(_startCellIndex);
        _startCellIndex = -1;
    } else if(newStatus == ChessBoardCellStatus::FINISH_IS_HERE && _finishCellIndex >= 0) {
        _cellsInfo[_finishCellIndex].status = ChessBoardCellStatus::NORMAL;
        _view.lock()->rebuildChessBoardCell(_finishCellIndex);
        _finishCellIndex = -1;
    }
    
    
    
    if(cellIndex == _startCellIndex) {
        _startCellIndex = -1;
    } else if(cellIndex == _finishCellIndex) {
        _finishCellIndex = -1;
    }
    
    _cellsInfo[cellIndex].status = newStatus;
    _view.lock()->rebuildChessBoardCell(cellIndex);
    
    switch (newStatus) {
        case ChessBoardCellStatus::START_IS_HERE: {
            _startCellIndex = cellIndex;
            break;
        }
        case ChessBoardCellStatus::FINISH_IS_HERE:
            _finishCellIndex = cellIndex;
            break;
        default:
            break;
    }
    
    switch (newStatus) {
        case ChessBoardCellStatus::CLOSED :
            _graph.setVertexIsActive(cellIndex, false);
            break;
        default:
            _graph.setVertexIsActive(cellIndex, true);
            break;
    }
}

ChessBoardCellStatus KnightTripModel::statusForCell(unsigned int cellIndex) {
    assert(cellIndex < _cellsInfo.size());
    return _cellsInfo[cellIndex].status;
}

bool KnightTripModel::findWayFromStartToFinish(std::vector<int>& wayPoints) {
    assert(_startCellIndex >= 0 && _finishCellIndex >= 0);
    
    return _graph.searchMinPathBFS(_startCellIndex, _finishCellIndex, wayPoints);
}


void KnightTripModel::createKnightPathGraph(unsigned int cellsDim) {
    assert(cellsDim > 0);
    
    _graph.clear();
    _graph.expand(cellsDim * cellsDim);
    
    for(int rowInd=0; rowInd < cellsDim; rowInd++) {
        for(int colInd=0; colInd < cellsDim; colInd++) {
            
            for(int xOffset=-2; xOffset<=2; xOffset++) {
                if(xOffset == 0) { continue; }
                
                int yOffset1 = 3 - std::abs(xOffset);
                int yOffset2 = -yOffset1;
                
                unsigned int aIndex = rowInd * cellsDim + colInd;
                
                if(0 <= colInd + xOffset && colInd + xOffset < cellsDim &&
                   0 <= rowInd + yOffset1 && rowInd + yOffset1 < cellsDim) {
                    unsigned int bIndex1 = (rowInd + yOffset1) * cellsDim + colInd + xOffset;
                    _graph.insertEdge(aIndex, bIndex1);
                }
                if(0 <= colInd + xOffset && colInd + xOffset < cellsDim &&
                   0 <= rowInd + yOffset2 && rowInd + yOffset2 < cellsDim) {
                    unsigned int bIndex2 = (rowInd + yOffset2) * cellsDim + colInd + xOffset;
                    _graph.insertEdge(aIndex, bIndex2);
                }
                
            }
            
        }
    }
}