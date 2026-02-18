#include <iostream>
#include "src/include.hpp"

#define _cst(val) SC (size_t, val)

size_t binSearch (const std::vector<int>& src, int target)
{
    LET  (size_t, lp, 0);
    LET  (size_t, rp, src.size () - 1);
    LETA (size_t, mid);
    while (lp <= rp)
    {
        mid = (lp + rp) / _cst (2);

        if (src[mid] == target) return mid;
        
        if (src[mid] < target)  lp = mid + _cst (1);
        else                    rp = mid - _cst (1);
    }

    return SIZE_MAX;
}

int main ()
{
    std::vector<int> vec = {2, 3, 4, 20, 30};
    std::cout << "result: " << binSearch (vec, 20) << std::endl;

    return 0;
}