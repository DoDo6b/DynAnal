#include <iostream>
#include "src/include.hpp"

size_t binSearch (const std::vector<int>& src, int target)
{
    LET (size_t, lp, 0);
    LET (size_t, rp, src.size () - 1);
    Var<size_t> mid ("mid");
    while (lp <= rp)
    {
        mid = (lp + rp) / 2;

        if (src[mid] == target) return mid;
        
        if (src[mid] < target)  lp = mid + 1;
        else                    rp = mid - 1;
    }

    return SIZE_MAX;
}

int main ()
{
    std::vector<int> vec = {2, 3, 4, 20, 30};
    std::cout << "result: " << binSearch (vec, 20) << std::endl;

    return 0;
}