#include <iostream>
#include "src/include.hpp"

#define _cst(val) SC (size_t, val)
#define _cb(val)  SC (bool, val)

size_t binSearch (const std::vector<int>& src, int target)
Beg ("binSearch")
    LET  (size_t, lp, 0);
    LET  (size_t, rp, src.size () - 1);
    LETA (size_t, mid);
    while (lp <= rp)
    {
        mid = (lp + rp) / _cst (2);

        if (src[mid] == target) Ret (mid);
        
        if (src[mid] < target)  lp = mid + _cst (1);
        else                    rp = mid - _cst (1);
    }

End (SIZE_MAX)

int main ()
{
    std::vector<int> vec = {2, 3, 4, 20, 30, 60, 76, 344, 32321, 2334};
    std::cout << "result: " << binSearch (vec, 20) << std::endl;

    LET (bool, test, true);
    if (test)
    {
        std::cout << "ggs\n";
    }
    return 0;
}