//
//  KnightTripController.hpp
//  knightTrip
//
//  Created by Pavel on 02.07.16.
//
//

#ifndef KnightTripController_hpp
#define KnightTripController_hpp

#include "KnightTripControllerStates.hpp"


namespace kt {
    class KnightTripModel;
    class KnightTripView;
    
    class KnightTripController : public std::enable_shared_from_this<KnightTripController> {
        
    public:
        //init
        KnightTripController();
        
        //service
        void start();
        void switchToNextState();
        
        //events
        void onCellTap(int cellAbsIndex);
        
        //properties
        inline std::shared_ptr<KnightTripModel> model() { return _model; }
        inline std::shared_ptr<KnightTripView> view() { return _view; }
    
    private:
        CC_DISALLOW_COPY_AND_ASSIGN(KnightTripController);
    private:
        std::shared_ptr<KnightTripModel> _model;
        std::shared_ptr<KnightTripView> _view;
        
        std::shared_ptr<KTControllerState> _currentState;
    };
}

#endif /* KnightTripController_hpp */
