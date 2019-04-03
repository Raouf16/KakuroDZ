

#include "Solver.h"
#include <iomanip>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int Solver::nb_tentatives = 1;
int Solver::nb_iteration = 0;
int Solver::nb_solution = 0;
int Solver::MAX_SOLUTION = 1;


Solver::Solver() {}

Solver::Solver(int *val_lignes, int *val_colonnes, int *val_possibles, int nb_lignes, int nb_colonnes,
               int nb_val_possibles) {

    this->val_lignes = val_lignes;
    this->val_colonnes = val_colonnes;
    this->nb_lignes = nb_lignes;
    this->nb_colonnes = nb_colonnes;
    this->val_possibles = val_possibles;
    this->nb_val_possibles = nb_val_possibles;
    grille = new int *[nb_lignes];
    for (int i = 0; i < nb_lignes; i++) {
        grille[i] = new int[nb_colonnes];
        for (int j = 0; j < nb_colonnes; j++) {
            grille[i][j] = 0;
        }
    }
    ligne = new Cellule *[nb_lignes];
    col = new Cellule *[nb_colonnes];
    for (int i = 0; i < nb_lignes; i++) {
        ligne[i] = new Cellule[nb_val_possibles];
        for (int j = 0; j < nb_val_possibles; j++) {
            ligne[i][j] = *new Cellule(false, val_possibles[j]);
        }
    }
    for (int i = 0; i < nb_colonnes; i++) {
        col[i] = new Cellule[nb_val_possibles];
        for (int j = 0; j < nb_val_possibles; j++) {
            col[i][j] = *new Cellule(false, val_possibles[j]);
        }
    }
}

Solver::Solver(const Solver &a) {

    this->nb_lignes = a.nb_lignes;
    this->nb_colonnes = a.nb_colonnes;
    this->val_possibles = a.val_possibles;
    this->nb_val_possibles = a.nb_val_possibles;
    this->val_lignes = new int[nb_lignes];

    for (int i = 0; i < nb_lignes; i++) {
        this->val_lignes[i] = a.val_lignes[i];
    }
    this->val_colonnes = new int[nb_colonnes];
    for (int i = 0; i < nb_colonnes; i++) {
        this->val_colonnes[i] = a.val_colonnes[i];
    }
    this->grille = new int *[this->nb_lignes];
    for (int i = 0; i < nb_lignes; i++) {
        this->grille[i] = new int[nb_colonnes];
        for (int j = 0; j < nb_colonnes; j++) {
            this->grille[i][j] = a.grille[i][j];
        }

    }
    this->ligne = new Cellule *[nb_lignes];
    this->col = new Cellule *[nb_colonnes];
    for (int i = 0; i < nb_lignes; i++) {
        this->ligne[i] = new Cellule[nb_val_possibles];
        for (int j = 0; j < nb_val_possibles; j++) {
            this->ligne[i][j].val = a.ligne[i][j].val;
            this->ligne[i][j].used = a.ligne[i][j].used;

        }

    }
    for (int i = 0; i < nb_colonnes; i++) {
        this->col[i] = new Cellule[nb_val_possibles];
        for (int j = 0; j < nb_val_possibles; j++) {
            this->col[i][j].val = a.col[i][j].val;
            this->col[i][j].used = a.col[i][j].used;
        }
    }

}

void Solver::creerKakuro(int nbL, int nbC, int num_val_possibles) {

    this->val_possibles = new int[num_val_possibles];
    this->nb_val_possibles = num_val_possibles;
    this->nb_lignes = nbL;
    this->nb_colonnes = nbC;

    for (int i = 1; i <= num_val_possibles; i++) {
        this->val_possibles[i - 1] = i;
    }

    ligne = new Cellule *[nb_lignes];
    col = new Cellule *[nb_colonnes];
    for (int i = 0; i < nb_lignes; i++) {
        ligne[i] = new Cellule[nb_val_possibles];
        for (int j = 0; j < nb_val_possibles; j++) {
            ligne[i][j] = *new Cellule(false, val_possibles[j]);
        }
    }
    for (int i = 0; i < nb_colonnes; i++) {
        col[i] = new Cellule[nb_val_possibles];
        for (int j = 0; j < nb_val_possibles; j++) {
            col[i][j] = *new Cellule(false, val_possibles[j]);
        }
    }

    grille = new int *[nb_lignes];
    for (int i = 0; i < nb_lignes; i++) {
        grille[i] = new int[nb_colonnes];
        for (int j = 0; j < nb_colonnes; j++) {
            grille[i][j] = 0;
        }
    }
    bool x;
    do {
        reinitialiser();   // reinitialiser les cases a  false avec la valeur 0, a chaque fois que la méthode solve_monte_carlo renvoie false
        x = solve_monte_carlo(false);
    } while (!x);

    this->val_lignes = new int[nb_lignes];
    this->val_colonnes = new int[nb_colonnes];

    for (int i = 0; i < nb_lignes; i++) {
        this->val_lignes[i] = sumRow(i);
    }

    for (int i = 0; i < nb_colonnes; i++) {
        this->val_colonnes[i] = sumCol(i);
    }

    reinitialiser();
    print_grid();
    cout << "Valeurs possibles : [ ";
    for (int i = 0; i < nb_val_possibles; i++) {
        if (i != (nb_val_possibles - 1)) cout << val_possibles[i] << " , ";
        else cout << val_possibles[i] << " ]" << endl << endl;
    }
}

void Solver::print_grid() {
    int max = 0;
    int min = 7;
    for (int i = 0; i < nb_lignes; i++) {
        if (val_lignes[i] > max) max = val_lignes[i];
    }
    for (int i = 0; i < nb_colonnes; i++) {
        if (val_colonnes[i] > max) max = val_colonnes[i];
    }
    int nb = (log10(max) + 1);
    if ((nb + 2) < min) nb = min;
    cout << setw(nb) << "\n" << setw(nb) << " ";
    for (int i = 0; i < nb_colonnes; i++) {
        cout << setw(nb) << val_colonnes[i];
        cout << setw(nb) << " ";
    }
    // cout << setw(nb) << "Somme";
    cout << endl;
    for (int i = 0; i < (nb * ((nb_colonnes * 2) + 1)) - 1; i++) {
        if (i < nb) cout << " ";
        else cout << "-";
    }
    cout << endl;
    for (int i = 0; i < nb_lignes; i++) {
        cout << setw(nb - 2) << val_lignes[i];
        cout << " |";
        for (int j = 0; j < nb_colonnes; j++) {
            cout << setw(nb) << grille[i][j];
            if (j != nb_colonnes - 1) cout << setw(nb) << "|";
            else cout << setw(nb) << "|";
        }
        // cout << setw(nb - 2) << sumRow(i);
        cout << '\n';
        for (int i = 0; i < (nb * ((nb_colonnes * 2) + 1)) - 1; i++) {
            if (i < nb) cout << " ";
            else cout << "-";
        }
        cout << endl;
    }

    // cout << setw(nb) << "Somme  ";
    for (int i = 0; i < nb_colonnes; i++) {
        //   cout << setw(nb) << sumCol(i);
        cout << setw(nb) << " ";
    }
    cout << "\n\n";

}

int Solver::sumCol(int i) {
    int somme = 0;
    for (int j = 0; j < nb_lignes; j++) {
        somme += grille[j][i];
    }
    return somme;
}

int Solver::sumRow(int i) {
    int somme = 0;
    for (int j = 0; j < nb_colonnes; j++) {
        somme += grille[i][j];
    }
    return somme;
}

bool Solver::check_val_row(int i) {
    bool resultat = true;
    for (int z = 0; z < nb_val_possibles; z++) {
        int val = val_possibles[z];
        bool b_val = false;
        for (int j = 0; j < nb_colonnes; j++) {
            if (grille[i][j] == val) {
                if (b_val == false) b_val = true;
                else {
                    return false;
                }
            }
        }
    }
    return resultat;
}

bool Solver::check_val_col(int i) {
    bool resultat = true;
    for (int z = 0; z < nb_val_possibles; z++) {
        int val = val_possibles[z];
        bool b_val = false;
        for (int j = 0; j < nb_lignes; j++) {
            if (grille[j][i] == val) {
                if (b_val == false) b_val = true;
                else {
                    return false;
                }
            }
        }
    }
    return resultat;
}

bool Solver::end_of_game() {
    for (int i = 0; i < nb_lignes; i++) {
        if (sumRow(i) != val_lignes[i] || !check_val_row(i)) return false;
    }
    for (int i = 0; i < nb_colonnes; i++) {
        if (sumCol(i) != val_colonnes[i] || !check_val_col(i)) return false;
    }
    return true;
}

bool Solver::solve_monte_carlo(bool opti) {
    for (int i = 0; i < nb_lignes; i++) {
        for (int j = 0; j < nb_colonnes; j++) {
            if (!choose(i, j, opti)) {
                nb_tentatives++;
                return false;
            }

        }
    }
    return true;
}

void Solver::monte_carlo() {
    clock_t start, end;
    start = clock();

    do {
        reinitialiser();
        solve_monte_carlo(true);
    } while (end_of_game() != true);
    end = clock();
    double texec = (float) (end - start) / CLOCKS_PER_SEC;
    print_grid();
    cout << endl << "Nombre de tentaive(s) : " << nb_tentatives << endl;
    cout << "Temps d'execution : " << setprecision(5) << texec * 1000 << "ms" << endl << endl;
}

void Solver::reinitialiser() {

    for (int i = 0; i < nb_lignes; i++) {
        for (int j = 0; j < nb_val_possibles; j++) {
            ligne[i][j].used = false;
        }
    }
    for (int i = 0; i < nb_colonnes; i++) {
        for (int j = 0; j < nb_val_possibles; j++) {
            col[i][j].used = false;
        }
    }
    for (int i = 0; i < nb_lignes; i++) {
        for (int j = 0; j < nb_colonnes; j++) {
            grille[i][j] = 0;
        }

    }


}

bool Solver::choose(int x, int y, bool opti) {

    int taille = nb_val_possible(x, y);
    if (taille != 0) {
        int rnd = rand() % (taille);
        srand(time(NULL));
        int *tab = possible_choice(taille, x, y);
        grille[x][y] = tab[rnd];
        delete_choice(tab[rnd], x, y);
        if (opti) {
            if ((sumRow(x) > val_lignes[x] || sumCol(y) > val_colonnes[y]) || ((y == nb_colonnes - 1) &&
                                                                               sumRow(x) != val_lignes[x]) ||
                ((x == nb_lignes - 1) && sumCol(y) != val_colonnes[y]))
                return false;

        }
    } else return false;

    return true;
}

int Solver::nb_val_possible(int i, int j) {

    int cpt = 0;
    for (int x = 0; x < nb_val_possibles; x++) {
        if (!ligne[i][x].used) {
            for (int z = 0; z < nb_val_possibles; z++) {
                if (!col[j][z].used) {
                    if (ligne[i][x].val == col[j][z].val) {
                        cpt++;
                    }
                }
            }
        }
    }
    return cpt;
}

int *Solver::possible_choice(int cpt, int i, int j) {
    int *tab = NULL;

    if (cpt != 0) {

        tab = new int[cpt];
        cpt = 0;
        for (int x = 0; x < nb_val_possibles; x++) {
            if (!ligne[i][x].used) {
                for (int z = 0; z < nb_val_possibles; z++) {
                    if (!col[j][z].used) {
                        if (ligne[i][x].val == col[j][z].val) {
                            tab[cpt] = ligne[i][x].val;
                            cpt++;
                        }
                    }
                }
            }
        }
    }
    return tab;
}

void Solver::delete_choice(int x, int i, int j) {

    for (int z = 0; z < nb_val_possibles; z++) {
        if (ligne[i][z].val == x) { ligne[i][z].used = true; }
    }
    for (int z = 0; z < nb_val_possibles; z++) {
        if (col[j][z].val == x) { col[j][z].used = true; }
    }

}

void Solver::forward_checking(Solver k, int i, int j) {
    if (nb_solution != MAX_SOLUTION || MAX_SOLUTION == 0) {
        int taille = k.nb_val_possible(i, j);
        int *tab = k.possible_choice(taille, i, j);
        if (taille != 0) {
            for (int x = 0; x < taille; x++) {
                nb_iteration++;
                Solver g = k;
                g.grille[i][j] = tab[x];
                g.delete_choice(tab[x], i, j);
                if ((g.sumRow(i) > g.val_lignes[i] || g.sumCol(j) > g.val_colonnes[j])) break;
                else {
                    if (i == nb_lignes - 1 && j == nb_colonnes - 1) {
                        if (g.end_of_game()) {
                            g.print_grid();
                            nb_solution++;
                        }
                    } else {
                        if (j == (g.nb_colonnes - 1) && i < g.nb_lignes - 1) {
                            if (g.sumRow(i) != g.val_lignes[i]);
                            else forward_checking(g, i + 1, 0);
                        } else {
                            if (((i == g.nb_lignes - 1) && (g.sumCol(j) != g.val_colonnes[j])));
                            else forward_checking(g, i, j + 1);
                        }
                    }
                }
            }
        }
    }
}

void Solver::solve(int m) {
    MAX_SOLUTION = m;
    cout << m;
    clock_t begin, end;
    begin = clock();
    forward_checking(*this, 0, 0);
    end = clock();
    double temps_execution = (float) (end - begin) / CLOCKS_PER_SEC;
    cout << endl << "Nombre d'iteration(s) : " << nb_iteration << endl;
    (nb_solution > 0 ? cout << "Nombre de résultats : " << nb_solution << endl : cout
            << "Il n'existe aucune solution au Solver" << endl);
    cout << "Temps d'execution : " << setprecision(5) << temps_execution << "secondes" << endl << endl;
}
