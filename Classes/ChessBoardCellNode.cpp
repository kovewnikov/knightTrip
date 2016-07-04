//
//  ChessBoardCellNode.cpp
//  knightTrip
//
//  Created by Pavel on 03.07.16.
//
//

#include "ChessBoardCellNode.hpp"
#include "ChessBoardNode.hpp"
#include "KnightTripModel.hpp"

using namespace kt;
USING_NS_CC;

ChessBoardCellNode::ChessBoardCellNode() {}

ChessBoardCellNode* ChessBoardCellNode::create() {
    ChessBoardCellNode* cell = new (std::nothrow) ChessBoardCellNode();
    if(cell && cell->init()) {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return nullptr;
}

bool ChessBoardCellNode::init() {
    if(LayerColor::init()) {
        setColor(Color3B::WHITE);
        setOpacity(0);
        _statusSpr = Sprite::create();
        _statusSpr->setAnchorPoint(Vec2(0, 0));
        _statusSpr->setPosition(0, 0);
        addChild(_statusSpr);
        
        return true;
    }
    
    return false;
}

void ChessBoardCellNode::updateStatus(ChessBoardCellStatus status) {
    
    Texture2D* statusTexture = requestTextureForCurrentStatus(status);
    if(statusTexture) {
        _statusSpr->setVisible(true);
        _statusSpr->setTexture(statusTexture);
        _statusSpr->setTextureRect(Rect(Vec2(0, 0), statusTexture->getContentSize()));
        _statusSpr->setScale(getContentSize().width / statusTexture->getContentSize().width);
    } else {
        _statusSpr->setVisible(false);
    }
    
}

void ChessBoardCellNode::fillBackground(const Color3B& color) {
    stopAllActions();
    setOpacity(180);
    auto tintAct = TintTo::create(0.25, color);

    runAction(tintAct);
}
void ChessBoardCellNode::clearBackground() {
    stopAllActions();
    setOpacity(0);
    setColor(Color3B::WHITE);
}


Texture2D* ChessBoardCellNode::requestTextureForCurrentStatus(ChessBoardCellStatus status) {
    std::string iconName;
    
    switch (status) {
        case ChessBoardCellStatus::CLOSED:
            iconName = "close_icon.png";
            break;
        case ChessBoardCellStatus::START_IS_HERE:
            iconName = "knight_icon.png";
            break;
        case ChessBoardCellStatus::FINISH_IS_HERE:
            iconName = "finish_icon.png";
            break;
        default:
            break;
    }
    
    return Director::getInstance()->getTextureCache()->addImage(iconName);
}