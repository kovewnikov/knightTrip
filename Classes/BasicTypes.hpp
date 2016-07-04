//
//  BasicTypes.hpp
//  knightTrip
//
//  Created by Pavel on 02.07.16.
//
//

#ifndef BasicTypes_hpp
#define BasicTypes_hpp

namespace kt {
    
    enum class ChessBoardCellStatus {
        NORMAL          = 0,
        CLOSED          = 1,
        START_IS_HERE   = 2,
        FINISH_IS_HERE  = 3,
    };
    

    class Graph {
    private:
        class GraphVertex {
        public:
            //init
            GraphVertex();
            CC_DISALLOW_COPY_AND_ASSIGN(GraphVertex);
            
            void connectWith(int bInd);
            inline void setIsActive(bool isActive);
            inline bool isActive();
            std::vector<int>::iterator adjacentBegin();
            std::vector<int>::iterator adjacentEnd();
        private:
            bool _isActive;
            std::vector<int> _adjacentVIndices;
        };
    public:
        //init
        Graph();
        Graph(int verticesQty);
        CC_DISALLOW_COPY_AND_ASSIGN(Graph);
        
        //service
        void expand(int newVertsQty);
        void clear();
        void insertEdge(unsigned int aInd, unsigned int bInd);
        void setVertexIsActive(unsigned int vInd, bool isActive);
        bool searchMinPathBFS(unsigned int originInd, unsigned int targetInd, std::vector<int>& path);
        
    private:
        std::vector<std::shared_ptr<GraphVertex>> _vertices;
    };
    
    
    
}

#endif /* BasicTypes_hpp */
