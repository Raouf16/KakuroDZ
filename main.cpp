#include "Generateur.h"
#include "windows.h"

int main(int argc, const char *argv[]) {

    cout << "||-----------------------------------------------------------------------------------||" << endl;
    cout << "||          ####### Kakuro by Naim & Raouf #######                                     " << endl;
    cout << "||-----------------------------------------------------------------------------------||" << endl;
    cout << endl;

    Generateur *s = new Generateur();
    int nbLines = 0, nbColumn = 0, nbValues = 0, valuesList;

    cout << "\t--> nombre de lignes : ";
    cin >> nbLines;
    cout << "\t--> nombre de colonnes : ";
    cin >> nbColumn;
    cout << "\t--> Nombre de valeurs possibles : ";
    cin >> nbValues;
    /*
            int* lineValue = new int[nbLines];
            int* columnValue = new int[nbColumn];
            int* pValue = new int[nbValues];
            cout << endl;
            cout << " Valeurs a atteindre sur les lignes : " << endl;

            for (int i = 0; i < nbLines; i++) {
                cout << "\tSomme ligne[" << i << "] = ";
                cin >> lineValue[i];
            }
            cout << endl;

            cout << "Valeurs a atteindre sur les colonnes : " << endl;
            for (int i = 0; i < nbColumn; i++) {
                cout << "\tSomme Colonne[" << i << "] = ";
                cin >> columnValue[i];
            }
            cout << endl;

           // cout << "Liste des  valeurs possibles : " << endl;
            for (int i = 1; i <= nbValues; i++) {
                pValue[i - 1] = i;
            }

            cout << "Liste des  valeurs possibles : [ ";
            for (int i = 0; i < nbValues; i++) {
                if (i != (nbValues - 1)) cout << pValue[i] << " , ";
                else cout << pValue[i] << " ]" << endl << endl;
            }


            s = Generateur(lineValue, columnValue, pValue, nbLines, nbColumn, nbValues);
            s.print_grid();
            cout << "Choisir le nombre solutions possibles : ";
            cin >> valuesList;
            s.solve(valuesList);
    */

    s->generateK(nbLines, nbColumn, nbValues);
    s->monte_carlo();

    Sleep(100000);

    return 0;

}
