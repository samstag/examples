/* meta_population.cpp
 * 
 * This file is part of EALib.
 * 
 * Copyright 2012 David B. Knoester.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <ea/evolutionary_algorithm.h>
#include <ea/generational_models/death_birth_process.h>
#include <ea/representations/circular_genome.h>
#include <ea/fitness_function.h>
#include <ea/cmdline_interface.h>
#include <ea/datafiles/fitness.h>
#include <ea/markov_network.h>
#include <ea/meta_population.h>
#include <ea/island_model.h>
#include <ea/selection/elitism.h>
using namespace ea;

/* This example defines an island model GA, where each individual represents a
 markov network.  The key to understanding meta-populations in EALib is that *each*
 island is its own, independent, instance of an evolutionary_algorithm.  Anything
 that crosses island boundaries is then defined at the meta-population level.
 */


/*! Sample fitness function for Markov networks.
 */
struct example_fitness : fitness_function<unary_fitness<double>, constantS, absoluteS, stochasticS> {
    
    /*! Initialize this fitness function -- load data, etc. */
    template <typename RNG, typename EA>
    void initialize(RNG& rng, EA& ea) {
    }
    
	template <typename Individual, typename RNG, typename EA>
	double operator()(Individual& ind, RNG& rng, EA& ea) {
        mkv::markov_network net(get<MKV_INPUT_N>(ea),
                                get<MKV_OUTPUT_N>(ea),
                                get<MKV_HIDDEN_N>(ea),
                                rng);
        
        // build a markov network from the individual's genome:
        mkv::build_markov_network(net, ind.repr().begin(), ind.repr().end(), ea);
        
        // allocate space for the inputs & outputs:
        std::vector<int> inputs(get<MKV_INPUT_N>(ea), 0);
        std::vector<int> outputs(get<MKV_OUTPUT_N>(ea));
        
        // now, set the values of the bits in the input vector:
        
        // update the network n times:
        update(net, get<MKV_UPDATE_N>(ea), inputs.begin(), outputs.begin());
        
        // calculate fitness based on the outputs...
        
        // and return some measure of fitness:
        return 1.0;
    }
};


/*! Configuration for an EA (one island).
 */
template <typename EA>
struct configuration : public abstract_configuration<EA> {
    //! Called to generate the initial EA population.
    void initial_population(EA& ea) {
        generate_ancestors(mkv_random_individual(), get<POPULATION_SIZE>(ea), ea);
    }
};


//! Evolutionary algorithm definition (one island).
typedef evolutionary_algorithm<
circular_genome<int>,
mkv_mutation,
example_fitness,
configuration,
recombination::asexual,
generational_models::death_birth_process<selection::proportionate< >, selection::elitism<selection::random> >
> ea_type;


/*! Configuration for the meta-population (all islands).  In this example, we don't
 actually need to do anything.
 */
template <typename EA>
struct mp_configuration : public abstract_configuration<EA> {
};


//! Meta-population definition - This is the "super" EA type.
typedef meta_population<ea_type, mp_configuration> mp_type;


/*! Define the EA's command-line interface.
 */
template <typename EA>
class cli : public cmdline_interface<EA> {
public:
    virtual void gather_options() {
        // markov network options
        add_option<MKV_INPUT_N>(this);
        add_option<MKV_OUTPUT_N>(this);
        add_option<MKV_HIDDEN_N>(this);
        add_option<MKV_UPDATE_N>(this);
        add_option<MKV_GATE_TYPES>(this);
        add_option<MKV_INITIAL_GATES>(this);
        add_option<MKV_REPR_INITIAL_SIZE>(this);
        add_option<MKV_REPR_MAX_SIZE>(this);
        add_option<MKV_REPR_MIN_SIZE>(this);
        add_option<GATE_INPUT_LIMIT>(this);
        add_option<GATE_INPUT_FLOOR>(this);
        add_option<GATE_OUTPUT_LIMIT>(this);
        add_option<GATE_OUTPUT_FLOOR>(this);
        
        // ea options
        add_option<REPRESENTATION_SIZE>(this);
        add_option<POPULATION_SIZE>(this);
        add_option<REPLACEMENT_RATE_P>(this);
        add_option<MUTATION_PER_SITE_P>(this);
        add_option<MUTATION_UNIFORM_INT_MAX>(this);
        add_option<MUTATION_DELETION_P>(this);
        add_option<MUTATION_DUPLICATION_P>(this);
        add_option<RUN_UPDATES>(this);
        add_option<RUN_EPOCHS>(this);
        add_option<CHECKPOINT_OFF>(this);
        add_option<CHECKPOINT_PREFIX>(this);
        add_option<RNG_SEED>(this);
        add_option<RECORDING_PERIOD>(this);
        add_option<META_POPULATION_SIZE>(this);
        add_option<ISLAND_MIGRATION_PERIOD>(this);
        add_option<ISLAND_MIGRATION_RATE>(this);
        add_option<ELITISM_N>(this);
    }
    
    virtual void gather_events(EA& ea) {
        add_event<island_model>(this, ea);
        add_event<datafiles::meta_population_fitness>(this, ea);
    };
};
LIBEA_CMDLINE_INSTANCE(mp_type, cli);