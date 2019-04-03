#include <stdio.h>
#include <iostream>
#include "Case.h"

using namespace std;

class Generateur {
private:

    int **grid;
    Case **line;
    Case **col;
    int *lineValue;
    int *columnValue;
    int nbLines;
    int nbColumns;
    int *pValue;
    int values;


public:
    void generer(int nbL, int nbC);

    static int nbEssaies;
    static int nb_iteration;
    static int nbResults;
    static int MAX_SOLUTION;

    Generateur(int *lineValue, int *columnValue, int *pValue, int nbLines, int nbColumns,
           int values);

    Generateur(const Generateur &a);

    Generateur();

    void generateK(int nbL, int nbC, int values);

    void print_grid();

    int sumRow(int i);

    int sumCol(int i);

    bool check_val_row(int i);

    bool check_val_col(int i);

    bool end_of_game();

    void monte_carlo();

    void solve(int m);

    bool choose(int x, int y, bool opti);

    bool solve_monte_carlo(bool opti);

    void reinitialiser();

    int nb_val_possible(int i, int j);

    int *possible_choice(int cpt, int i, int j);

    void delete_choice(int x, int i, int j);

    void forward_checking(Generateur k, int i, int j);
};

