//
//  KnightTripModel.hpp
//  knightTrip
//
//  Created by Pavel on 02.07.16.
//
//

#ifndef KnightTripModel_hpp
#define KnightTripModel_hpp

#include "BasicTypes.hpp"

#define CELLS_DIM 8

namespace kt {
    
    class KnightTripView;
    
    struct ChessCellData {
        ChessBoardCellStatus status;
    };
    
    class KnightTripModel {
    public:
        //init
        KnightTripModel();
        void initialize();
        
        //properties
        inline void setView(std::weak_ptr<KnightTripView> view) { _view = view; }
        inline unsigned int cellsDimension() { return CELLS_DIM; }
        inline bool isStartCellPicked() { return _startCellIndex >= 0; };
        inline bool isFinishCellPicked() { return _finishCellIndex >= 0; };
        
        //service
        void reloadModel();
        void setStatusForCell(unsigned int cellIndex, ChessBoardCellStatus newStatus);
        ChessBoardCellStatus statusForCell(unsigned int cellIndex);
        bool findWayFromStartToFinish(std::vector<int>& wayPoints);
        
    private:
        CC_DISALLOW_COPY_AND_ASSIGN(KnightTripModel);
        //service
        void createKnightPathGraph(unsigned int cellsPerRow);
        
    private:
        std::weak_ptr<KnightTripView> _view;
        std::vector<ChessCellData> _cellsInfo;
        int _startCellIndex;
        int _finishCellIndex;
        Graph _graph;
    };
}



#endif /* KnightTripModel_hpp */
