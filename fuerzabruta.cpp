#include <bits/stdc++.h>
#include <ctime>      
#include <sys/resource.h> 
using namespace std;

vector<int> costInsert(26), costDelete(26);
vector<vector<int>> costReplace(26, vector<int>(26));
vector<vector<int>> costTranspose(26, vector<int>(26));

// Función para cargar los costos desde los archivos
void loadCosts() {
    ifstream insertFile("cost_insert.txt"), deleteFile("cost_delete.txt"), 
              replaceFile("cost_replace.txt"), transposeFile("cost_transpose.txt");

    if (!insertFile || !deleteFile || !replaceFile || !transposeFile) {
        cerr << "No se pudo abrir uno de los archivos de costo." << endl;
        exit(1);
    }

    // Cargar costos de inserción y eliminación
    for (int i = 0; i < 26; i++) insertFile >> costInsert[i];
    for (int i = 0; i < 26; i++) deleteFile >> costDelete[i];

    // Cargar matriz de costos de sustitución
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            replaceFile >> costReplace[i][j];
        }
    }

    // Cargar matriz de costos de transposición
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            transposeFile >> costTranspose[i][j];
        }
    }

    insertFile.close();
    deleteFile.close();
    replaceFile.close();
    transposeFile.close();
}

// Función para calcular la distancia de Levenshtein usando fuerza bruta sin optimizaciones
int levenshteinDistance(const string &str1, const string &str2, int lenStr1, int lenStr2) {
    // Casos base: si uno de los strings es vacío
    if (lenStr1 == 0) return accumulate(costInsert.begin(), costInsert.begin() + lenStr2, 0);
    if (lenStr2 == 0) return accumulate(costDelete.begin(), costDelete.begin() + lenStr1, 0);

    // Índices para obtener los costos de sustitución
    int i1 = str1[lenStr1 - 1] - 'a';  // Asumiendo que las letras son minúsculas
    int i2 = str2[lenStr2 - 1] - 'a';  // Asumiendo que las letras son minúsculas

    // Costo de sustitución
    int substitution = levenshteinDistance(str1, str2, lenStr1 - 1, lenStr2 - 1) + costReplace[i1][i2];

    // Llamadas recursivas con los costos de inserción y eliminación
    int insertion = levenshteinDistance(str1, str2, lenStr1, lenStr2 - 1) + costInsert[i2];
    int deletion = levenshteinDistance(str1, str2, lenStr1 - 1, lenStr2) + costDelete[i1];

    // Cálculo del costo de transposición
    int transposition = INT_MAX;
    if (lenStr1 > 1 && lenStr2 > 1 &&
        str1[lenStr1 - 1] == str2[lenStr2 - 2] &&
        str1[lenStr1 - 2] == str2[lenStr2 - 1]) {
        
        int t1 = str1[lenStr1 - 2] - 'a';  // índice del segundo último carácter en str1
        int t2 = str2[lenStr2 - 1] - 'a';  // índice del segundo último carácter en str2
        transposition = levenshteinDistance(str1, str2, lenStr1 - 2, lenStr2 - 2) + costTranspose[t1][t2] + 1;
    }

    // Selección del costo mínimo entre todas las operaciones posibles
    return min({insertion, deletion, substitution, transposition});
}

// Función para obtener el uso máximo de memoria en kilobytes
long getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;  // Devuelve la memoria máxima usada en KB
}

int main() {
    loadCosts();  // Cargar los costos antes de comenzar el cálculo

    ifstream file("dataset.csv");
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo de entrada." << endl;
        return 1;
    }

    ofstream outfile("cadenas_costos_resultado.csv", ios::app);
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida." << endl;
        return 1;
    }

    string line;
    bool first_line = outfile.tellp() == 0;  // Comprueba si el archivo está vacío
    if (first_line) {
        outfile << "Tipo de Caso,Cadena 1,Cadena 2,Costo" << endl;
    }

    int totalCost = 0;  // Variable para almacenar la suma de todos los costos

    // Iniciar el conteo del tiempo de ejecución
    clock_t startTime = clock();

    while (getline(file, line)) {
        stringstream ss(line);
        string tipoCaso, str1, str2;
        getline(ss, tipoCaso, ',');  
        getline(ss, str1, ',');      
        getline(ss, str2, ',');      

        int distance = levenshteinDistance(str1, str2, str1.size(), str2.size());
        totalCost += distance;  // Sumar el costo al total
        outfile << tipoCaso << "," << str1 << "," << str2 << "," << distance << endl;
    }

    // Finalizar el conteo del tiempo de ejecución
    clock_t endTime = clock();
    double elapsedTime = double(endTime - startTime) / CLOCKS_PER_SEC;

    file.close();
    outfile.close();

    // Obtener la memoria máxima utilizada
    long memoryUsed = getMemoryUsage();

    // Mostrar resultados
    cout << "Resultados agregados en cadenas_costos_resultado.csv" << endl;
    cout << "Costo total de todas las distancias: " << totalCost << endl;
    cout << "Tiempo de ejecución: " << elapsedTime << " segundos" << endl;
    cout << "Memoria máxima utilizada: " << memoryUsed << " KB" << endl;

    return 0;
}
