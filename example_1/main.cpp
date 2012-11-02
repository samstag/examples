#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

/* Ok, let's say we want to build an EA.
 
 First, we need to know what fitness function we're trying to solve, and we need
 to pick a representation (i.e., a genome) that we'll try to evolve to solve that
 fitness function.
 
 For these examples, let's assume that we want to solve "all-ones," a standard
 EA test problem where the fitness of a given individual is simply the number of
 ones in its genome.
 
 So, we could define the representation like this:
 */
typedef vector<int> repr_type;

/* That's just shorthand that tells the compiler "wherever you see repr_type, treat
 it like a vector<int>.
 
 We'll also need to know how many sites are in each representation.  Let's define
 a constant for this purpose:
 */
size_t repr_size=10;

/*
 size_t is a typedef defined by the STL - It's usually an unsigned int.
 
 Now we need a fitness function that operates on repr_types:
 */
size_t all_ones(repr_type& r) {
    return count(r.begin(), r.end(), 1);
}

/* Now, let's make sure it works:
 */
int main(int argc, const char * argv[]) {
    repr_type r1(repr_size,1);
    cout << "10 ones: " << all_ones(r1) << endl;
    
    repr_type r2(repr_size,0);
    cout << "0 ones: " << all_ones(r2) << endl;
    return 0;
}

/* Great.  However, there's no evolution here -- Indeed, we don't even have a
population.  On to example 2.
*/