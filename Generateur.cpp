

#include "Generateur.h"
#include <iomanip>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int Generateur::nbEssaies = 1;
int Generateur::nbIter = 0;
int Generateur::nbResult = 0;
int Generateur::MAX_SOLUTION = 1;


Generateur::Generateur() {}

Generateur::Generateur(int *lineValue, int *columnValue, int *pValues, int nbLines, int nbColumns,
               int values) {

    this->lineValue = lineValue;
    this->columnValue = columnValue;
    this->nbLines = nbLines;
    this->nbColumns = nbColumns;
    this->pValues = pValues;
    this->values = values;
    casesGrid = new int *[nbLines];
    for (int i = 0; i < nbLines; i++) {
        casesGrid[i] = new int[nbColumns];
        for (int j = 0; j < nbColumns; j++) {
            casesGrid[i][j] = 0;
        }
    }
    line = new Case *[nbLines];
    col = new Case *[nbColumns];
    for (int i = 0; i < nbLines; i++) {
        line[i] = new Case[values];
        for (int j = 0; j < values; j++) {
            line[i][j] = *new Case(false, pValues[j]);
        }
    }
    for (int i = 0; i < nbColumns; i++) {
        col[i] = new Case[values];
        for (int j = 0; j < values; j++) {
            col[i][j] = *new Case(false, pValues[j]);
        }
    }
}

Generateur::Generateur(const Generateur &a) {

    this->nbLines = a.nbLines;
    this->nbColumns = a.nbColumns;
    this->pValues = a.pValues;
    this->values = a.values;
    this->lineValue = new int[nbLines];

    for (int i = 0; i < nbLines; i++) {
        this->lineValue[i] = a.lineValue[i];
    }
    this->columnValue = new int[nbColumns];
    for (int i = 0; i < nbColumns; i++) {
        this->columnValue[i] = a.columnValue[i];
    }
    this->casesGrid = new int *[this->nbLines];
    for (int i = 0; i < nbLines; i++) {
        this->casesGrid[i] = new int[nbColumns];
        for (int j = 0; j < nbColumns; j++) {
            this->casesGrid[i][j] = a.casesGrid[i][j];
        }

    }
    this->line = new Case *[nbLines];
    this->col = new Case *[nbColumns];
    for (int i = 0; i < nbLines; i++) {
        this->line[i] = new Case[values];
        for (int j = 0; j < values; j++) {
            this->line[i][j].val = a.line[i][j].val;
            this->line[i][j].used = a.line[i][j].used;

        }

    }
    for (int i = 0; i < nbColumns; i++) {
        this->col[i] = new Case[values];
        for (int j = 0; j < values; j++) {
            this->col[i][j].val = a.col[i][j].val;
            this->col[i][j].used = a.col[i][j].used;
        }
    }

}

void Generateur::generateK(int nbL, int nbC, int num_pValues) {

    this->pValues = new int[num_pValues];
    this->values = num_pValues;
    this->nbLines = nbL;
    this->nbColumns = nbC;

    for (int i = 1; i <= num_pValues; i++) {
        this->pValues[i - 1] = i;
    }

    line = new Case *[nbLines];
    col = new Case *[nbColumns];
    for (int i = 0; i < nbLines; i++) {
        line[i] = new Case[values];
        for (int j = 0; j < values; j++) {
            line[i][j] = *new Case(false, pValues[j]);
        }
    }
    for (int i = 0; i < nbColumns; i++) {
        col[i] = new Case[values];
        for (int j = 0; j < values; j++) {
            col[i][j] = *new Case(false, pValues[j]);
        }
    }

    casesGrid = new int *[nbLines];
    for (int i = 0; i < nbLines; i++) {
        casesGrid[i] = new int[nbColumns];
        for (int j = 0; j < nbColumns; j++) {
            casesGrid[i][j] = 0;
        }
    }
    bool x;
    do {
        reinitialiser();   // reinitialiser les cases a  false avec la valeur 0, a chaque fois que la méthode solve_monte_carlo renvoie false
        x = solve_monte_carlo(false);
    } while (!x);

    this->lineValue = new int[nbLines];
    this->columnValue = new int[nbColumns];

    for (int i = 0; i < nbLines; i++) {
        this->lineValue[i] = sumRow(i);
    }

    for (int i = 0; i < nbColumns; i++) {
        this->columnValue[i] = sumCol(i);
    }

    reinitialiser();
    print_grid();
    cout << "Valeurs possibles : [ ";
    for (int i = 0; i < values; i++) {
        if (i != (values - 1)) cout << pValues[i] << " , ";
        else cout << pValues[i] << " ]" << endl << endl;
    }
}

void Generateur::print_grid() {
    int max = 0;
    int min = 7;
    for (int i = 0; i < nbLines; i++) {
        if (lineValue[i] > max) max = lineValue[i];
    }
    for (int i = 0; i < nbColumns; i++) {
        if (columnValue[i] > max) max = columnValue[i];
    }
    int nb = (log10(max) + 1);
    if ((nb + 2) < min) nb = min;
    cout << setw(nb) << "\n" << setw(nb) << " ";
    for (int i = 0; i < nbColumns; i++) {
        cout << setw(nb) << columnValue[i];
        cout << setw(nb) << " ";
    }
    // cout << setw(nb) << "Somme";
    cout << endl;
    for (int i = 0; i < (nb * ((nbColumns * 2) + 1)) - 1; i++) {
        if (i < nb) cout << " ";
        else cout << "-";
    }
    cout << endl;
    for (int i = 0; i < nbLines; i++) {
        cout << setw(nb - 2) << lineValue[i];
        cout << " |";
        for (int j = 0; j < nbColumns; j++) {
            cout << setw(nb) << casesGrid[i][j];
            if (j != nbColumns - 1) cout << setw(nb) << "|";
            else cout << setw(nb) << "|";
        }
        // cout << setw(nb - 2) << sumRow(i);
        cout << '\n';
        for (int i = 0; i < (nb * ((nbColumns * 2) + 1)) - 1; i++) {
            if (i < nb) cout << " ";
            else cout << "-";
        }
        cout << endl;
    }

    // cout << setw(nb) << "Somme  ";
    for (int i = 0; i < nbColumns; i++) {
        //   cout << setw(nb) << sumCol(i);
        cout << setw(nb) << " ";
    }
    cout << "\n\n";

}

int Generateur::sumCol(int i) {
    int somme = 0;
    for (int j = 0; j < nbLines; j++) {
        somme += casesGrid[j][i];
    }
    return somme;
}

int Generateur::sumRow(int i) {
    int somme = 0;
    for (int j = 0; j < nbColumns; j++) {
        somme += casesGrid[i][j];
    }
    return somme;
}

bool Generateur::check_val_row(int i) {
    bool resultat = true;
    for (int z = 0; z < values; z++) {
        int val = pValues[z];
        bool b_val = false;
        for (int j = 0; j < nbColumns; j++) {
            if (casesGrid[i][j] == val) {
                if (b_val == false) b_val = true;
                else {
                    return false;
                }
            }
        }
    }
    return resultat;
}

bool Generateur::check_val_col(int i) {
    bool resultat = true;
    for (int z = 0; z < values; z++) {
        int val = pValues[z];
        bool b_val = false;
        for (int j = 0; j < nbLines; j++) {
            if (casesGrid[j][i] == val) {
                if (b_val == false) b_val = true;
                else {
                    return false;
                }
            }
        }
    }
    return resultat;
}

bool Generateur::end_of_game() {
    for (int i = 0; i < nbLines; i++) {
        if (sumRow(i) != lineValue[i] || !check_val_row(i)) return false;
    }
    for (int i = 0; i < nbColumns; i++) {
        if (sumCol(i) != columnValue[i] || !check_val_col(i)) return false;
    }
    return true;
}

bool Generateur::solve_monte_carlo(bool opti) {
    for (int i = 0; i < nbLines; i++) {
        for (int j = 0; j < nbColumns; j++) {
            if (!choose(i, j, opti)) {
                nbEssaies++;
                return false;
            }

        }
    }
    return true;
}

void Generateur::monte_carlo() {
    clock_t start, end;
    start = clock();

    do {
        reinitialiser();
        solve_monte_carlo(true);
    } while (end_of_game() != true);
    end = clock();
    double texec = (float) (end - start) / CLOCKS_PER_SEC;
    print_grid();
    cout << endl << "Nombre de tentaive(s) : " << nbEssaies << endl;
    cout << "Temps d'execution : " << setprecision(5) << texec * 1000 << "ms" << endl << endl;
}

void Generateur::reinitialiser() {

    for (int i = 0; i < nbLines; i++) {
        for (int j = 0; j < values; j++) {
            line[i][j].used = false;
        }
    }
    for (int i = 0; i < nbColumns; i++) {
        for (int j = 0; j < values; j++) {
            col[i][j].used = false;
        }
    }
    for (int i = 0; i < nbLines; i++) {
        for (int j = 0; j < nbColumns; j++) {
            casesGrid[i][j] = 0;
        }

    }


}

bool Generateur::choose(int x, int y, bool opti) {

    int taille = nb_val_possible(x, y);
    if (taille != 0) {
        int rnd = rand() % (taille);
        srand(time(NULL));
        int *tab = possible_choice(taille, x, y);
        casesGrid[x][y] = tab[rnd];
        delete_choice(tab[rnd], x, y);
        if (opti) {
            if ((sumRow(x) > lineValue[x] || sumCol(y) > columnValue[y]) || ((y == nbColumns - 1) &&
                                                                               sumRow(x) != lineValue[x]) ||
                ((x == nbLines - 1) && sumCol(y) != columnValue[y]))
                return false;

        }
    } else return false;

    return true;
}

int Generateur::nb_val_possible(int i, int j) {

    int cpt = 0;
    for (int x = 0; x < values; x++) {
        if (!line[i][x].used) {
            for (int z = 0; z < values; z++) {
                if (!col[j][z].used) {
                    if (line[i][x].val == col[j][z].val) {
                        cpt++;
                    }
                }
            }
        }
    }
    return cpt;
}

int *Generateur::possible_choice(int cpt, int i, int j) {
    int *tab = NULL;

    if (cpt != 0) {

        tab = new int[cpt];
        cpt = 0;
        for (int x = 0; x < values; x++) {
            if (!line[i][x].used) {
                for (int z = 0; z < values; z++) {
                    if (!col[j][z].used) {
                        if (line[i][x].val == col[j][z].val) {
                            tab[cpt] = line[i][x].val;
                            cpt++;
                        }
                    }
                }
            }
        }
    }
    return tab;
}

void Generateur::delete_choice(int x, int i, int j) {

    for (int z = 0; z < values; z++) {
        if (line[i][z].val == x) { line[i][z].used = true; }
    }
    for (int z = 0; z < values; z++) {
        if (col[j][z].val == x) { col[j][z].used = true; }
    }

}

void Generateur::forward_checking(Generateur k, int i, int j) {
    if (nbResult != MAX_SOLUTION || MAX_SOLUTION == 0) {
        int taille = k.nb_val_possible(i, j);
        int *tab = k.possible_choice(taille, i, j);
        if (taille != 0) {
            for (int x = 0; x < taille; x++) {
                nbIter++;
                Generateur g = k;
                g.casesGrid[i][j] = tab[x];
                g.delete_choice(tab[x], i, j);
                if ((g.sumRow(i) > g.lineValue[i] || g.sumCol(j) > g.columnValue[j])) break;
                else {
                    if (i == nbLines - 1 && j == nbColumns - 1) {
                        if (g.end_of_game()) {
                            g.print_grid();
                            nbResult++;
                        }
                    } else {
                        if (j == (g.nbColumns - 1) && i < g.nbLines - 1) {
                            if (g.sumRow(i) != g.lineValue[i]);
                            else forward_checking(g, i + 1, 0);
                        } else {
                            if (((i == g.nbLines - 1) && (g.sumCol(j) != g.columnValue[j])));
                            else forward_checking(g, i, j + 1);
                        }
                    }
                }
            }
        }
    }
}

void Generateur::solve(int m) {
    MAX_SOLUTION = m;
    cout << m;
    clock_t begin, end;
    begin = clock();
    forward_checking(*this, 0, 0);
    end = clock();
    double temps_execution = (float) (end - begin) / CLOCKS_PER_SEC;
    cout << endl << "Nombre d'iteration(s) : " << nbIter << endl;
    (nbResult > 0 ? cout << "Nombre de résultats : " << nbResult << endl : cout
            << "Il n'existe aucune solution au Generateur" << endl);
    cout << "Temps d'execution : " << setprecision(5) << temps_execution << "secondes" << endl << endl;
}
