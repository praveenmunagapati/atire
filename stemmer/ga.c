#include <stdio.h>
#include <stdlib.h>
#include "ga.h"
#include "ga_individual.h"

inline double GA::get_fitness(GA_individual *individual) {
    if (individual->is_evaluated) 
        return individual->fitness;
    individual->is_evaluated = TRUE;
    fitness_function->stemmer->set_stemmer(individual);
    individual->fitness = fitness_function->call();
    printf("fitness: %f\n", individual->fitness);
    return individual->fitness;
}

/* note that the pointer returned is not to a newly made object */
GA_individual *GA::tournament_select() {
    GA_individual *pick = population + (rand() % population_size);
    GA_individual *tmp;
    unsigned int i;

    for(i = 0; i < tournament_size - 1; i++) {
        tmp = population + (rand() % population_size);

        if (get_fitness(tmp) > get_fitness(pick))
            pick = tmp;
    }

    return pick;
}

void GA::next_generation() {
    unsigned int i = 0, op;

    if (elitism == USE_ELITISM) {
        i = 1;
        get_best()->reproduce(next_population);
    }

    for (; i < population_size; i++) {
        op = random_from(0, 100);
        if (op < mutation_rate)
            tournament_select()->mutate(next_population + i, str_gen);
        else if (op < mutation_rate + crossover_rate) 
            tournament_select()->crossover(tournament_select(),
                                         next_population + i);
        else 
            tournament_select()->reproduce(next_population + i);
    }

    {
        GA_individual *tmp = population;
        population = next_population;
        next_population = tmp;
    }
}

GA_individual *GA::get_best() {
    unsigned int i;
    GA_individual *best = population;

    for (i = 1; i < population_size; i++) {
        if (get_fitness(population + i) > get_fitness(best))
            best = population + i;
    }
    return best;
}

void GA::run(unsigned int generations) {
    unsigned int i;
    for (i = 0; i < generations; i++) {
        {
            GA_individual *best = get_best();
            best->print_raw();
            printf("Generation: %ud Fitness: %f\n", i, get_fitness(best));
        }
        this->next_generation();
    }
    {
        GA_individual *best = get_best();
        best->print_raw();
        printf("Fitness: %f\n", get_fitness(best));
    }
}

GA::GA(unsigned int population_size, GA_function *fitness_function, char *(*str_gen)()) {
    unsigned int i;
    this->population_size = population_size;
    this->fitness_function = fitness_function;
    this->str_gen = str_gen;

    population = new GA_individual[population_size];
    for (i = 0; i < population_size; i++) {
        population[i].generate(str_gen);
    }
    next_population = new GA_individual[population_size];

    elitism = USE_ELITISM;

    mutation_rate = DEFAULT_MUTATION_RATE;
    crossover_rate = DEFAULT_CROSSOVER_RATE;
    tournament_size = DEFAULT_TOURNAMENT_SIZE;
}

GA::~GA() {
    free(population);
}