//
//  KnightTripView.hpp
//  knightTrip
//
//  Created by Pavel on 03.07.16.
//
//

#ifndef KnightTripView_hpp
#define KnightTripView_hpp

#include "ChessBoardNode.hpp"
#include "BasicTypes.hpp"

namespace kt {
    
    class KnightTripModel;
    class KnightTripController;
    
    
    class KnightTripView : public IChessBoardNodePresenter {
    public:
        //init
        KnightTripView();
        
        //service
        void initialize();
        void rebuildChessBoard();
        void rebuildChessBoardCell(int cellAbsIndex);
        void setInvitationText(const std::string& invText);
        void clearTrails();
        void showKnightTrail(const std::vector<int>& wayPoints);
        
        //properties
        inline void setController(std::weak_ptr<KnightTripController> cntr) { _controller = cntr; }
        inline void setModel(std::weak_ptr<KnightTripModel> model) { _model = model; }
        
        //events
        void onTapNextBtn(cocos2d::Ref* pSender);
        
        //IChessBoardNodeController stuff
        int cellsPerRow() override;
        void onTapOnChessBoardCell(unsigned int cellAbsIndex) override;
        ChessBoardCellStatus statusForCell(unsigned int cellAbsIndex) override;
        
        
    private:
        CC_DISALLOW_COPY_AND_ASSIGN(KnightTripView);
        
        //service
        void buildContent();
    private:
        std::weak_ptr<KnightTripController> _controller;
        std::weak_ptr<KnightTripModel> _model;
        cocos2d::Node* _rootNode;
        ChessBoardNode* _board;
        cocos2d::Label* _invitationLbl;
    };
}



#endif /* KnightTripView_hpp */
