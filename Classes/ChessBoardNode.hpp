//
//  ChessBoardNode.hpp
//  knightTrip
//
//  Created by Pavel on 03.07.16.
//
//

#ifndef ChessBoardNode_hpp
#define ChessBoardNode_hpp

#include "BasicTypes.hpp"

namespace kt {
    
    class ChessBoardCellNode;
    
    class IChessBoardNodePresenter {
    public:
        virtual int cellsPerRow() = 0;
        virtual ChessBoardCellStatus statusForCell(unsigned int cellAbsIndex) = 0;
        virtual void onTapOnChessBoardCell(unsigned int cellAbsIndex) = 0;
    };
    
    
    
    class ChessBoardNode : public cocos2d::Node {
    public:
        static ChessBoardNode* create(float sideSize,
                                      IChessBoardNodePresenter* presenter);
        ChessBoardNode();
        bool initChessBoard(float sideSize,
                            IChessBoardNodePresenter* presenter);
        
        //service
        void reload();
        void reloadCell(unsigned int cellAbsIndex);
        
        void clearTrails();
        void showKnightTrail(const std::vector<int>& wayPoints, bool animate);
        
        //properties
        inline cocos2d::Size getCellSize() const { return _cellSize; };
        
        //touch-events
        bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
        void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
        
    private:
        CC_DISALLOW_COPY_AND_ASSIGN(ChessBoardNode);
        //service
        void findCellsBetweenKnightWaypoints(unsigned int originInd,
                                             unsigned int targetInd,
                                             std::vector<int>& result);
        cocos2d::Vec2 convertIndexToCellCoordinates(int cellAbsIndex);
        int convertPositionToCellIndex(const cocos2d::Vec2& pos) const;
    private:
        IChessBoardNodePresenter* _presenter;
        cocos2d::Sprite* _backgroundSpr;
        std::vector<ChessBoardCellNode*> _cells;
        cocos2d::Size _cellSize;
        int _pickedCellIndex;
    };
}


#endif /* ChessBoardNode_hpp */
