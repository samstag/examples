/* qhfc.cpp
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
#include <ea/meta_population.h>
#include <ea/generational_models/qhfc.h>
#include <ea/representations/bitstring.h>
#include <ea/cmdline_interface.h>
using namespace ealib;

/*! Fitness function that rewards for the number of ones in the genome.
 */
struct all_ones : public fitness_function<unary_fitness<double> > {
    template <typename Individual, typename EA>
    double operator()(Individual& ind, EA& ea) {
        return static_cast<double>(std::count(ind.repr().begin(), ind.repr().end(), 1u));
    }
};


/*! User-defined configuration struct; called at various points during initialization
 of the EA.
 */
template <typename EA>
struct configuration : public abstract_configuration<EA> {
    /*! Called to generate the initial population, in this case, a population of
     random bitstrings.
     */
    void initial_population(EA& ea) {
        generate_ancestors(ancestors::random_bitstring(), get<POPULATION_SIZE>(ea), ea);
    }

    //! Called to fill a population to capacity.
    void fill_population(EA& ea) {
        generate_ancestors(ancestors::random_bitstring(), get<POPULATION_SIZE>(ea)-ea.size(), ea);
    }
};

typedef evolutionary_algorithm<
bitstring,
mutation::operators::per_site<mutation::site::bitflip>, // mutation operator
all_ones,
configuration,
recombination::two_point_crossover,
generational_models::deterministic_crowding< > > ea_type;

template <typename EA>
struct mp_configuration : public abstract_configuration<EA> {
};

typedef meta_population<ea_type, mp_configuration, generational_models::qhfc> mea_type;


/*! Define the EA's command-line interface.
 */
template <typename EA>
class cli : public cmdline_interface<EA> {
public:
    virtual void gather_options() {
        add_option<POPULATION_SIZE>(this);
        add_option<REPRESENTATION_SIZE>(this);
        add_option<META_POPULATION_SIZE>(this);
        add_option<MUTATION_PER_SITE_P>(this);
        add_option<ELITISM_N>(this);
        add_option<QHFC_BREED_TOP_FREQ>(this);
        add_option<QHFC_DETECT_EXPORT_NUM>(this);
        add_option<QHFC_PERCENT_REFILL>(this);
        add_option<QHFC_CATCHUP_GEN>(this);
        add_option<QHFC_NO_PROGRESS_GEN>(this);
        add_option<RUN_UPDATES>(this);
        add_option<RUN_EPOCHS>(this);
        add_option<CHECKPOINT_OFF>(this);
        add_option<CHECKPOINT_PREFIX>(this);
        add_option<RNG_SEED>(this);
        add_option<RECORDING_PERIOD>(this);
    }
    
    virtual void gather_events(EA& ea) {
        add_event<datafiles::qhfc>(this, ea);
    };
};
LIBEA_CMDLINE_INSTANCE(mea_type, cli);
