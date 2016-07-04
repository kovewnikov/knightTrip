//
//  ChessBoardNode.cpp
//  knightTrip
//
//  Created by Pavel on 03.07.16.
//
//

#include "ChessBoardNode.hpp"
#include "ChessBoardCellNode.hpp"

#define TRAIL_ACTION_TAG 99
#define TRAIL_PRIMARY_COLOR Color3B(255, 0, 0)
#define TRAIL_SECOND_COLOR Color3B(190, 190, 190)


using namespace kt;
USING_NS_CC;

#pragma mark - Lifecycle

ChessBoardNode* ChessBoardNode::create(float sideSize,
                                       IChessBoardNodePresenter* presenter) {
    ChessBoardNode* bNode = new (std::nothrow) ChessBoardNode();
    if(bNode && bNode->initChessBoard(sideSize, presenter)) {
        bNode->autorelease();
        return bNode;
    }
    CC_SAFE_DELETE(bNode);
    return nullptr;
}

ChessBoardNode::ChessBoardNode() {}

bool ChessBoardNode::initChessBoard(float sideSize,
                                    IChessBoardNodePresenter* presenter) {
    if(Node::init()) {
        _presenter = presenter;
        
        Size size(sideSize, sideSize);
        setContentSize(size);
        
        _backgroundSpr = Sprite::create("chess_board2.png");
        _backgroundSpr->setAnchorPoint(Vec2(0,0));
        Texture2D::TexParams tRepeatParams;
        tRepeatParams.magFilter = GL_NEAREST;
        tRepeatParams.minFilter = GL_NEAREST;
        tRepeatParams.wrapS = GL_REPEAT;
        tRepeatParams.wrapT = GL_REPEAT;
        _backgroundSpr->getTexture()->setTexParameters(tRepeatParams);
        addChild(_backgroundSpr);
        
        //touches
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan = CC_CALLBACK_2(ChessBoardNode::onTouchBegan, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(ChessBoardNode::onTouchEnded, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

        //other
        _pickedCellIndex = -1;

        return true;
    }
    
    return false;
}

#pragma mark - Service

void ChessBoardNode::reload() {
    unsigned int cellsPerRow = _presenter->cellsPerRow();
    
    //build board
    Size cellInitialSize = _backgroundSpr->getTexture()->getContentSize() / 2.;
    float nonScaledSideSize = cellInitialSize.width * cellsPerRow;
    _backgroundSpr->setTextureRect(Rect(0, 0, nonScaledSideSize, nonScaledSideSize));
    
    float cellScaleCoef = getContentSize().width / nonScaledSideSize;
    _backgroundSpr->setScale(cellScaleCoef);
    
    _cellSize = cellInitialSize * cellScaleCoef;
    
    
    //build cells
    int cellsNewCount = cellsPerRow * cellsPerRow;
    _cells.reserve(cellsNewCount);
    
    int cellIterIndex = -1;
    for(int i=0; i < cellsPerRow; i++) {
        for(int j=0; j < cellsPerRow; j++) {
            cellIterIndex++;
            ChessBoardCellNode* cell = nullptr;
            if(cellIterIndex < _cells.size()) {
                cell = _cells[cellIterIndex];
            } else {
                cell = ChessBoardCellNode::create();
                _cells.push_back(cell);
                addChild(cell);
            }

            cell->setContentSize(getCellSize());
            cell->setAnchorPoint(Vec2(0, 1.));
            cell->setIgnoreAnchorPointForPosition(false);
            cell->setPosition(cell->getContentSize().width * j,
                              getContentSize().height - (cell->getContentSize().height * i));
            cell->updateStatus(_presenter->statusForCell(cellIterIndex));
        }
    }
    
    
    for(int i = cellsNewCount; i < _cells.size(); i++) {
        ChessBoardCellNode* cell = _cells[i];
        cell->removeFromParentAndCleanup(true);
    }
    
    _cells.resize(cellsNewCount);
}

void ChessBoardNode::reloadCell(unsigned int cellAbsIndex) {
    assert(cellAbsIndex < _cells.size());
    _cells[cellAbsIndex]->updateStatus(_presenter->statusForCell(cellAbsIndex));
}

void ChessBoardNode::clearTrails() {
    stopActionByTag(TRAIL_ACTION_TAG);
    for(auto cellsIt = _cells.begin(); cellsIt != _cells.end(); cellsIt++) {
        (*cellsIt)->clearBackground();
    }
}

void ChessBoardNode::showKnightTrail(const std::vector<int>& wayPoints, bool animate) {
    std::vector<int> trail;
    Vector<FiniteTimeAction*> sequenceData;
    float delayBeforeStep = 0.9;
    
    auto originIt = wayPoints.begin();
    int i = 0;
    for(auto endIt = wayPoints.begin() + 1; endIt != wayPoints.end(); endIt++) {
        
        trail.clear();
        findCellsBetweenKnightWaypoints(*originIt, *endIt, trail);
        
        
        for(auto it = trail.begin(); it != trail.end(); it++) {
            auto delayBefore = DelayTime::create(i * delayBeforeStep);
            
            int cellIndex = *it;
            auto colorCallback1 = CallFunc::create([this, cellIndex](){
                _cells[cellIndex]->fillBackground(TRAIL_PRIMARY_COLOR);
                
            });
            
            auto delayBetween = DelayTime::create(0.95);
            
            auto colorCallback2 = CallFunc::create([this, cellIndex](){
                _cells[cellIndex]->fillBackground(TRAIL_SECOND_COLOR);
                
            });
            
            
            
            
            sequenceData.pushBack(Sequence::create(delayBefore,
                                                   colorCallback1,
                                                   delayBetween,
                                                   colorCallback2, NULL));
            
        }
        
        
        i++;
        originIt = endIt;
    }
    
    
    
    
    
    
    auto trailAnimation = Spawn::create(sequenceData);
    trailAnimation->setTag(TRAIL_ACTION_TAG);
    runAction(trailAnimation);
}



void ChessBoardNode::findCellsBetweenKnightWaypoints(unsigned int originInd,
                                            unsigned int targetInd,
                                            std::vector<int>& result) {
    assert(originInd < _cells.size() && targetInd < _cells.size());
    
    Vec2 originCoords = convertIndexToCellCoordinates(originInd);
    Vec2 targetCoords = convertIndexToCellCoordinates(targetInd);
    
    Vec2 offset = targetCoords - originCoords;

    int xSign = offset.x > 0 ? 1 : -1;
    int ySign = offset.y > 0 ? 1 : -1;
    
    if(std::abs(offset.x) > std::abs(offset.y)) {
        int curCellInd = originInd + 1 * xSign;
        result.push_back(curCellInd);
        curCellInd += 1 * xSign;
        result.push_back(curCellInd);
        curCellInd += _presenter->cellsPerRow() * ySign;
        result.push_back(curCellInd);
    } else {
        int curCellInd = originInd + _presenter->cellsPerRow() * ySign;
        result.push_back(curCellInd);
        curCellInd += _presenter->cellsPerRow() * ySign;
        result.push_back(curCellInd);
        curCellInd += 1 * xSign;
        result.push_back(curCellInd);
    }
}

Vec2 ChessBoardNode::convertIndexToCellCoordinates(int cellAbsIndex) {
    int y = 0;
    while(cellAbsIndex - _presenter->cellsPerRow() >= 0) {
        y++;
        cellAbsIndex -= _presenter->cellsPerRow();
    }
    int x = cellAbsIndex;
    
    return Vec2(x, y);
}

int ChessBoardNode::convertPositionToCellIndex(const Vec2& pos) const {
    int xCoord = floorf(pos.x / getCellSize().width);
    int yCoordFromTop = floorf( (getContentSize().height - pos.y) / getCellSize().height );
    
    unsigned int cellIndex = yCoordFromTop * _presenter->cellsPerRow() + xCoord;
    
    return (cellIndex < _cells.size() ? cellIndex : -1);
}

#pragma mark - Events

bool ChessBoardNode::onTouchBegan(Touch* touch, Event* event) {
    Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
    
    if(_pickedCellIndex >= 0 ||
       !Rect(0, 0, getContentSize().width, getContentSize().height).containsPoint(touchLocation)) {
        return false;
    }
    
    _pickedCellIndex = convertPositionToCellIndex(touchLocation);
    
    return true;
}

void ChessBoardNode::onTouchEnded(Touch *touch, Event *event) {
    Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
    
    if(_pickedCellIndex < 0 ||
       !Rect(0, 0, getContentSize().width, getContentSize().height).containsPoint(touchLocation)) {
        return;
    }
    
    if(_pickedCellIndex == convertPositionToCellIndex(touchLocation)) {
        _presenter->onTapOnChessBoardCell(_pickedCellIndex);
    }
    
    _pickedCellIndex = -1;
}

