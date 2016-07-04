//
//  KnightTripControllerStates.cpp
//  knightTrip
//
//  Created by Pavel on 04.07.16.
//
//

#include "KnightTripControllerStates.hpp"

#include "KnightTripController.hpp"
#include "KnightTripModel.hpp"
#include "KnightTripView.hpp"

using namespace kt;
USING_NS_CC;

//Состояние выбора закрытых ячеек
#pragma mark - KTControllerStatePickClosedCells

KTControllerStatePickClosedCells::KTControllerStatePickClosedCells(std::weak_ptr<KnightTripController> sMachine) {
    _stateMachine= sMachine;
}

void KTControllerStatePickClosedCells::toggleStatusForCell(unsigned int cellIndex) {
    ChessBoardCellStatus status = _stateMachine.lock()->model()->statusForCell(cellIndex);
    if(status != ChessBoardCellStatus::CLOSED) {
        _stateMachine.lock()->model()->setStatusForCell(cellIndex, ChessBoardCellStatus::CLOSED);
    } else {
        _stateMachine.lock()->model()->setStatusForCell(cellIndex, ChessBoardCellStatus::NORMAL);
    }
}

std::shared_ptr<KTControllerState> KTControllerStatePickClosedCells::requestNextState() {
    return std::make_shared<KTControllerStatePickStartPosition>(_stateMachine);
}

bool KTControllerStatePickClosedCells::canSwitchState() {
    return true;
}

void KTControllerStatePickClosedCells::onStateEnter() {
    _stateMachine.lock()->view()->setInvitationText("Pick closed cells and press \'Next\'");
}

void KTControllerStatePickClosedCells::onStateExit() {}









//Состояние выбора ячейки старта
#pragma mark - KTControllerStatePickStartPosition

KTControllerStatePickStartPosition::KTControllerStatePickStartPosition(std::weak_ptr<KnightTripController> sMachine) {
    _stateMachine= sMachine;
}

void KTControllerStatePickStartPosition::toggleStatusForCell(unsigned int cellIndex) {
    ChessBoardCellStatus status = _stateMachine.lock()->model()->statusForCell(cellIndex);
    if(status != ChessBoardCellStatus::CLOSED) {
        _stateMachine.lock()->model()->setStatusForCell(cellIndex, ChessBoardCellStatus::START_IS_HERE);
    }
}

std::shared_ptr<KTControllerState> KTControllerStatePickStartPosition::requestNextState() {
    return std::make_shared<KTControllerStatePickFinishPosition>(_stateMachine);
}

bool KTControllerStatePickStartPosition::canSwitchState() {
    if(_stateMachine.lock()->model()->isStartCellPicked()) {
        return true;
    }
    return false;
}

void KTControllerStatePickStartPosition::onStateEnter() {
    _stateMachine.lock()->view()->setInvitationText("Pick start-cell and press \'Next\'");
}

void KTControllerStatePickStartPosition::onStateExit() {}









//Состояние выбора ячейки финиша
#pragma mark - KTControllerStatePickFinishPosition

KTControllerStatePickFinishPosition::KTControllerStatePickFinishPosition(std::weak_ptr<KnightTripController> sMachine) {
    _stateMachine= sMachine;
}

void KTControllerStatePickFinishPosition::toggleStatusForCell(unsigned int cellIndex) {
    ChessBoardCellStatus status = _stateMachine.lock()->model()->statusForCell(cellIndex);
    if(status != ChessBoardCellStatus::CLOSED &&
       status != ChessBoardCellStatus::START_IS_HERE) {
        _stateMachine.lock()->model()->setStatusForCell(cellIndex, ChessBoardCellStatus::FINISH_IS_HERE);
    }
}

std::shared_ptr<KTControllerState> KTControllerStatePickFinishPosition::requestNextState() {
    return std::make_shared<KTControllerStateVisualizePath>(_stateMachine);
}

bool KTControllerStatePickFinishPosition::canSwitchState() {
    if(_stateMachine.lock()->model()->isFinishCellPicked()) {
        return true;
    }
    return false;
}


void KTControllerStatePickFinishPosition::onStateEnter() {
    _stateMachine.lock()->view()->setInvitationText("Pick finish-cell and press \'Next\'");
}
void KTControllerStatePickFinishPosition::onStateExit() {}









//Состояние визуализации
#pragma mark - KTControllerStateVisualizePath

KTControllerStateVisualizePath::KTControllerStateVisualizePath(std::weak_ptr<KnightTripController> sMachine) {
    _stateMachine= sMachine;
}

void KTControllerStateVisualizePath::toggleStatusForCell(unsigned int cellIndex) {}
std::shared_ptr<KTControllerState> KTControllerStateVisualizePath::requestNextState() {
    return std::make_shared<KTControllerStatePickClosedCells>(_stateMachine);
}

bool KTControllerStateVisualizePath::canSwitchState() {
    return true;
}

void KTControllerStateVisualizePath::onStateEnter() {

    std::vector<int>wayPoints;
    if(_stateMachine.lock()->model()->findWayFromStartToFinish(wayPoints)) {
        _stateMachine.lock()->view()->setInvitationText("For restart press \'Next\'");
        _stateMachine.lock()->view()->clearTrails();
        _stateMachine.lock()->view()->showKnightTrail(wayPoints);
    } else {
        _stateMachine.lock()->view()->setInvitationText("There is no way=( for restart press \'Next\'");
    }
}

void KTControllerStateVisualizePath::onStateExit() {
    _stateMachine.lock()->view()->clearTrails();
}
