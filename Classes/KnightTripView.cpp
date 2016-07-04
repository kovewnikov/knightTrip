//
//  KnightTripView.cpp
//  knightTrip
//
//  Created by Pavel on 03.07.16.
//
//

#include "KnightTripView.hpp"
#include "ChessBoardNode.hpp"

#include "KnightTripController.hpp"
#include "KnightTripModel.hpp"

#define SIDEBAR_WIDTH_PERCENT 25.
#define SIDEBAR_CHESS_BOARD_MIN_MARGIN_POINTS 30.

#define SIDEBAR_BACGROUND_COLOR Color4B(160, 128, 128, 255)

#define PERCENT_TO_COEF(percent) (percent / 100.)

using namespace kt;
USING_NS_CC;

#pragma mark - Lifecycle

KnightTripView::KnightTripView() {

}


void KnightTripView::initialize() {
    assert(!_controller.expired() &&
           !_model.expired()); //ожидаем, что перед инициализацией MVC компоненты уже связаны

    
    _rootNode = Node::create();
    _rootNode->setAnchorPoint(Vec2(0,0));
    _rootNode->setPosition(Director::getInstance()->getVisibleOrigin());
    _rootNode->setContentSize(Director::getInstance()->getVisibleSize());
    
    auto scene = Scene::create();
    scene->addChild(_rootNode);
    Director::getInstance()->runWithScene(scene);
    

    buildContent();
}

#pragma mark - Service

void KnightTripView::buildContent() {
    
    //sidebar
    auto sidebar = LayerColor::create(SIDEBAR_BACGROUND_COLOR,
                                             (_rootNode->getContentSize().width / 100.) * SIDEBAR_WIDTH_PERCENT,
                                             _rootNode->getContentSize().height);
    sidebar->setIgnoreAnchorPointForPosition(false);
    sidebar->setAnchorPoint(Vec2(1, 0));
    sidebar->setPosition(_rootNode->getContentSize().width, 0);
    
   _invitationLbl = Label::createWithTTF("some text",
                                      "fonts/Marker Felt.ttf",
                                      18,
                                      Size(sidebar->getContentSize().width, 64), TextHAlignment::CENTER);
    _invitationLbl->setOverflow(Label::Overflow::SHRINK);
    _invitationLbl->setAnchorPoint(Vec2(0, 0.5));
    _invitationLbl->setPosition(0, sidebar->getContentSize().height / 2);
    
    sidebar->addChild(_invitationLbl);

    auto nextButton = MenuItemSprite::create(Sprite::create("greenButton.png"),
                                             Sprite::create("greenButtonHighlight.png"),
                                             CC_CALLBACK_1(KnightTripView::onTapNextBtn, this));
    nextButton->setAnchorPoint(Vec2(0.5, 0));
    nextButton->setPosition(sidebar->getContentSize().width / 2, 10);
    
    auto menu = Menu::create(nextButton, NULL);
    menu->setPosition(Vec2::ZERO);
    sidebar->addChild(menu, 1);
    
    
    
    _rootNode->addChild(sidebar);
    
    //chessboard
    Size freeArea = Size(_rootNode->getContentSize().width - sidebar->getContentSize().width,
                         _rootNode->getContentSize().height);
    float chessBoardSide = std::min(freeArea.width, freeArea.height) - SIDEBAR_CHESS_BOARD_MIN_MARGIN_POINTS;
    
    _board = ChessBoardNode::create(chessBoardSide, this);
    _board->setIgnoreAnchorPointForPosition(false);
    _board->setAnchorPoint(Vec2(0.5, 0.5));
    _board->setPosition(freeArea.width / 2, freeArea.height / 2);
    _rootNode->addChild(_board);
}

void KnightTripView::rebuildChessBoard() {
    _board->reload();
}
void KnightTripView::rebuildChessBoardCell(int cellAbsIndex) {
    _board->reloadCell(cellAbsIndex);
}

void KnightTripView::setInvitationText(const std::string& invText) {
    _invitationLbl->setString(invText);
}

void KnightTripView::clearTrails() {
    _board->clearTrails();
}
void KnightTripView::showKnightTrail(const std::vector<int>& wayPoints) {
    _board->showKnightTrail(wayPoints, true);
}

#pragma mark - Events
void KnightTripView::onTapNextBtn(Ref* pSender) {
    _controller.lock()->switchToNextState();
}

#pragma mark - IChessBoardNodeController stuff

int KnightTripView::cellsPerRow() {
    return _model.lock()->cellsDimension();
}

void KnightTripView::onTapOnChessBoardCell(unsigned int cellAbsIndex) {
    _controller.lock()->onCellTap(cellAbsIndex);
}

ChessBoardCellStatus KnightTripView::statusForCell(unsigned int cellAbsIndex) {
    return _model.lock()->statusForCell(cellAbsIndex);
}

