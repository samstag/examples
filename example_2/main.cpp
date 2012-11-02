#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

/* From example 1 we had this:
 */
typedef vector<int> repr_type;

std::size_t repr_size=10;

std::size_t all_ones(repr_type& r) {
    return count(r.begin(), r.end(), 1);
}

/* ... which define our representation and fitness function.  Now, we need to
 start on the evolutionary part.  Let's begin with a population:
 */
typedef vector<repr_type> population_type;

/* This is simply a vector of repr_types.  
 
 We also need to know how large this population should be, so let's define
 another constant for that purpose:
 */
size_t population_size=100;

/* Next, we need to decide what kind of generational model we'll use.  For this
 example, let's go with something simple.  We'll call it "random_selection":
 */
void random_selection(population_type& p) {
    random_shuffle(p.begin(), p.end());
    population_type offspring(p.begin(), p.begin()+static_cast<std::size_t>(0.05*population_size));
    
    p.insert(p.end(), offspring.begin(), offspring.end());
    random_shuffle(p.begin(), p.end());
    p.erase(p.begin()+population_size, p.end());
}

/* This generational model selects 5% random individuals for replication, copies them
 into an offspring population, merges the offspring into the original population,
 and then truncates the lot back down to a configured population size.

 Now, let's make sure it works:
 */
int main(int argc, const char * argv[]) {
    population_type p(population_size, repr_type(repr_size, 0));

    random_selection(p);
    return 0;
}

/* Great.  However, there's still no evolution here -- There's no mutation or 
 preferential survival for highly fit individuals.  On to example 3...
 */