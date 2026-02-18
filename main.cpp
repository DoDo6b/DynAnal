#include <iostream>
#include "src/include.hpp"

#define _cst(val) SC (size_t, val)
#define _cb(val)  SC (bool,   val)
#define _ci(val)  SC (int,    val)

size_t binSearch (const std::vector<int>& src, Var<int> target)
{
    GVZ.subgraphOpen (__func__);
    LET  (size_t, lp, 0);
    LET  (size_t, rp, src.size () - 1);
    LETA (size_t, mid);
    while (lp <= rp)
    {
        mid = (lp + rp) / _cst (2);

        if (_ci (src[mid]) == target)
        {
            GVZ.subgraphClose ();
            return mid;
        }
        
        if (_ci (src[mid]) < target)  lp = mid + _cst (1);
        else                          rp = mid - _cst (1);
    }

    GVZ.subgraphClose ();
    return SIZE_MAX;
}

int main ()
{
    GVZ.subgraphOpen (__func__);
    std::vector<int> vec = {2, 3, 4, 20, 30, 60, 76, 344, 32321, 2334};
    const LET (int, tar, 20);
    std::cout << "result: " << binSearch (vec, tar) << std::endl;

    GVZ.subgraphClose ();
    return 0;
}