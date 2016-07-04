//
//  KnightTripControllerStates.hpp
//  knightTrip
//
//  Created by Pavel on 04.07.16.
//
//

#ifndef KnightTripControllerStates_hpp
#define KnightTripControllerStates_hpp

namespace kt {
    
    class KnightTripController;

    class KTControllerState {
    public:
        virtual void toggleStatusForCell(unsigned int cellIndex) = 0;
        virtual std::shared_ptr<KTControllerState> requestNextState() = 0;
        virtual bool canSwitchState() = 0;
        virtual void onStateEnter() = 0;
        virtual void onStateExit() = 0;
        
    protected:
        std::weak_ptr<KnightTripController> _stateMachine;
    };
    
    
    
    
    
    class KTControllerStatePickClosedCells : public KTControllerState {
    public:
        KTControllerStatePickClosedCells(std::weak_ptr<KnightTripController> sMachine);
        
        void toggleStatusForCell(unsigned int cellIndex) override;
        std::shared_ptr<KTControllerState> requestNextState() override;
        bool canSwitchState() override;
        void onStateEnter() override;
        void onStateExit() override;
    };
    
    
    
    
    
    class KTControllerStatePickStartPosition : public KTControllerState {
    public:
        KTControllerStatePickStartPosition(std::weak_ptr<KnightTripController> sMachine);
        
        void toggleStatusForCell(unsigned int cellIndex) override;
        std::shared_ptr<KTControllerState> requestNextState() override;
        bool canSwitchState() override;
        void onStateEnter() override;
        void onStateExit() override;
    };
    
    
    
    
    
    class KTControllerStatePickFinishPosition : public KTControllerState {
    public:
        KTControllerStatePickFinishPosition(std::weak_ptr<KnightTripController> sMachine);
        
        void toggleStatusForCell(unsigned int cellIndex) override;
        std::shared_ptr<KTControllerState> requestNextState() override;
        bool canSwitchState() override;
        void onStateEnter() override;
        void onStateExit() override;
    };
    
    
    
    
    
    class KTControllerStateVisualizePath : public KTControllerState {
    public:    
        KTControllerStateVisualizePath(std::weak_ptr<KnightTripController> sMachine);
        
        void toggleStatusForCell(unsigned int cellIndex) override;
        std::shared_ptr<KTControllerState> requestNextState() override;
        bool canSwitchState() override;
        void onStateEnter() override;
        void onStateExit() override;
    };
}

#endif /* KnightTripControllerStates_hpp */
