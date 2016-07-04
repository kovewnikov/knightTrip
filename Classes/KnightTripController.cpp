//
//  KnightTripController.cpp
//  knightTrip
//
//  Created by Pavel on 02.07.16.
//
//

#include "KnightTripController.hpp"

#include "KnightTripModel.hpp"
#include "KnightTripView.hpp"

using namespace kt;
USING_NS_CC;

KnightTripController::KnightTripController() {}


#pragma mark - Service

void KnightTripController::start() {
    _model = std::make_shared<KnightTripModel>();
    _view = std::make_shared<KnightTripView>();
    
    _model->setView(_view);
    
    _view->setController(shared_from_this());
    _view->setModel(_model);
    
    _model->initialize();
    _view->initialize();
    
    _model->reloadModel();
    
    switchToNextState();
}

void KnightTripController::switchToNextState() {
    if(!_currentState) {
        _currentState = std::make_shared<KTControllerStatePickClosedCells>(shared_from_this());
        _currentState->onStateEnter();
    } else if(_currentState->canSwitchState()) {
        _currentState->onStateExit();
        _currentState = _currentState->requestNextState();
        _currentState->onStateEnter();
    }
}

#pragma mark - Events

void KnightTripController::onCellTap(int cellAbsIndex) {
    _currentState->toggleStatusForCell(cellAbsIndex);
}

