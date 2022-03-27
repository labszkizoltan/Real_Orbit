#include "RandomGeneration.h"

#include <iostream> // somehow this bring in the rand() function

namespace RORNG
{
    float runif()
    {
        return (float)(rand()%2048)/2048.0f;
    }
    
    float rnorm()
    {
        return 0.0f;
    }

}



