#ifndef _ENGINE_CONST_H_
#define _ENGINE_CONST_H_

namespace EngineConst
{
    const int WIN_SCORE = 1000000;
    const int LOSS_SCORE = -WIN_SCORE;
    
    const int INF = 1000000000;
    const int NEG_INF = -INF;
    
    const std::vector<int> LINE_SCORES = {
        0,      
        1,      
        10,     
        100,    
        1000,   
        10000   
    };
};

#endif
