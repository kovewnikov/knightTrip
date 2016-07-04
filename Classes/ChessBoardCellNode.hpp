//
//  ChessBoardCellNode.hpp
//  knightTrip
//
//  Created by Pavel on 03.07.16.
//
//

#ifndef ChessBoardCellNode_hpp
#define ChessBoardCellNode_hpp

#include "BasicTypes.hpp"

namespace kt {
    class ChessBoardNode;
    
    class ChessBoardCellNode : public cocos2d::LayerColor {
        
    public:
        //init
        static ChessBoardCellNode* create();

        //service
        void updateStatus(ChessBoardCellStatus status);
        void fillBackground(const cocos2d::Color3B& color);
        void clearBackground();
    private:
        //init
        ChessBoardCellNode();
        bool init() override;
        //service
        cocos2d::Texture2D* requestTextureForCurrentStatus(ChessBoardCellStatus status);
    private:
        CC_DISALLOW_COPY_AND_ASSIGN(ChessBoardCellNode);
        cocos2d::Sprite* _statusSpr;
    };
}



#endif /* ChessBoardCellNode_hpp */
