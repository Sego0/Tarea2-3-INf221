#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

// Tablas de costos
vector<vector<int>> costReplace(26, vector<int>(26, 0));
vector<int> costInsert(26, 0);
vector<int> costDelete(26, 0);
vector<vector<int>> costTranspose(26, vector<int>(26, 0));

// Función para cargar las tablas de costos desde archivos
void cargarCostos() {
    ifstream file;

    // Cargar costos de inserción
    file.open("cost_insert.txt");
    if (!file.is_open()) {
    cerr << "Error al abrir el archivo correspondiente. 1" << endl;
    exit(1);}
    for (int i = 0; i < 26; ++i) {
        file >> costInsert[i];
    }
    file.close();

    // Cargar costos de eliminación
    file.open("cost_delete.txt");
    if (!file.is_open()) {
    cerr << "Error al abrir el archivo correspondiente. 2" << endl;
    exit(1);}
    for (int i = 0; i < 26; ++i) {
        file >> costDelete[i];
    }
    file.close();

    // Cargar costos de sustitución
    file.open("cost_replace.txt");
    if (!file.is_open()) {
    cerr << "Error al abrir el archivo correspondiente. 3" << endl;
    exit(1);}
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            file >> costReplace[i][j];
        }
    }
    file.close();

    // Cargar costos de transposición
    file.open("cost_transpose.txt");
    if (!file.is_open()) {
    cerr << "Error al abrir el archivo correspondiente. 4" << endl;
    exit(1);}
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            file >> costTranspose[i][j];
        }
    }
    file.close();
}

// Calcula el costo de sustituir el carácter 'a' por 'b'
int costo_sub(char a, char b) {
    return costReplace[a - 'a'][b - 'a'];
}

// Calcula el costo de insertar el carácter 'b'
int costo_ins(char b) {
    return costInsert[b - 'a'];
}

// Calcula el costo de eliminar el carácter 'a'
int costo_del(char a) {
    return costDelete[a - 'a'];
}

// Calcula el costo de transponer los caracteres 'a' y 'b' adyacentes
int costo_trans(char a, char b) {
    return costTranspose[a - 'a'][b - 'a'];
}

// Función para encontrar la distancia de edición usando programación dinámica
int editDistanceDP(string str1, string str2, int m, int n) {
    // Crear un vector 2D para almacenar los valores de la distancia de edición
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // Casos base
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) {
                dp[i][j] = j * costo_ins(str2[j - 1]);
            } else if (j == 0) {
                dp[i][j] = i * costo_del(str1[i - 1]);
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                int costInsert = dp[i][j - 1] + costo_ins(str2[j - 1]);
                int costDelete = dp[i - 1][j] + costo_del(str1[i - 1]);
                int costReplace = dp[i - 1][j - 1] + costo_sub(str1[i - 1], str2[j - 1]);
                dp[i][j] = std:: min({costInsert, costDelete, costReplace});

                // Considera la transposición si los caracteres son adyacentes y se pueden intercambiar
                if (i > 1 && j > 1 && str1[i - 1] == str2[j - 2] && str1[i - 2] == str2[j - 1]) {
                    int costTranspose = dp[i - 2][j - 2] + costo_trans(str1[i - 2], str1[i - 1]);
                    dp[i][j] = min(dp[i][j], costTranspose);
                }
            }
        }
    }

    // Retornar la distancia de edición final
    return dp[m][n];
}

vector<pair<string, string>> leerCadenas(const string& filename) {
    vector<pair<string, string>> cadenas;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return cadenas;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string str1, str2;

        // Separar las cadenas por la coma
        if (getline(ss, str1, ',') && getline(ss, str2, ',')) {
            cadenas.emplace_back(str1, str2);
        }
    }

    file.close();
    return cadenas;
}

void limpiarCadena(string& str) {
    str.erase(remove_if(str.begin(), str.end(), [](char c) {
        return c < 'a' || c > 'z'; // Filtra caracteres fuera de rango
    }), str.end());
}

int main() {
    // Cargar los costos desde los archivos de texto
    cargarCostos();

    // Cargar los datasets desde los archivos de texto
    vector<pair<string, string>> similares = leerCadenas("similar_strings_1000.txt");
    vector<pair<string, string>> repetidas = leerCadenas("repeated_strings_1000.txt");
    vector<pair<string, string>> transposiciones = leerCadenas("transposable_strings_1000.txt");
    vector<pair<string, string>> distintas = leerCadenas("distinct_strings_1000.txt");
    
    // Definir las cadenas de entrada
    int n = 0;
    while (n < 10){
        string str1 = repetidas[n].first;
        string str2 = repetidas[n].second;
        limpiarCadena(str1);
        limpiarCadena(str2);

        auto inicio = chrono::high_resolution_clock::now();
        int result = editDistanceDP(str1, str2, str1.length(), str2.length());
        auto fin = chrono::high_resolution_clock::now();

        auto duracion = chrono::duration_cast<chrono::milliseconds >(fin - inicio).count();
        
        // Calcular y mostrar la distancia mínima de edición
        cout << result << " " << duracion << endl;
        n++;
    }

    return 0;
}