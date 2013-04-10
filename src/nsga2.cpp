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
#include <ea/fitness_functions/all_ones.h>
#include <ea/generational_models/nsga2.h>
using namespace ealib;

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
};


typedef evolutionary_algorithm<
bitstring,
mutation::per_site<mutation::bit>,
multi_all_ones,
configuration,
recombination::two_point_crossover,
generational_models::nsga2,
nsga2_attributes
> ea_type;


/*! Define the EA's command-line interface.
 */
template <typename EA>
class cli : public cmdline_interface<EA> {
public:
    virtual void gather_options() {
        add_option<POPULATION_SIZE>(this);
        add_option<REPRESENTATION_SIZE>(this);
        add_option<MUTATION_PER_SITE_P>(this);
        add_option<TOURNAMENT_SELECTION_N>(this);
        add_option<TOURNAMENT_SELECTION_K>(this);
        add_option<RUN_UPDATES>(this);
        add_option<RUN_EPOCHS>(this);
        add_option<CHECKPOINT_OFF>(this);
        add_option<CHECKPOINT_PREFIX>(this);
        add_option<RNG_SEED>(this);
        add_option<RECORDING_PERIOD>(this);
    }
    
    virtual void gather_events(EA& ea) {
    };
};
LIBEA_CMDLINE_INSTANCE(ea_type, cli);
