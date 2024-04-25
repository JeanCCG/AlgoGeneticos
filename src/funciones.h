#include <time.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;
int min_x = 10;
int max_x = 70;
int min_y = 20;
int max_y = 85;
int min_z = 15;
int max_z = 94;
vector<int> minimos = {min_x, min_y, min_z}, maximos = {max_x, max_y, max_z};
vector<double> suma_fin, media_fin, max_min_fin;
int gen = 50;
int sw = 0;
int sep = 10;
int cantidad = 10;

void mix(string& padre, string& madre, int j) {
  int punto_cruce;
  string hijo1, hijo2;
  do {
    punto_cruce = rand() % padre.length();
    hijo1 = padre, hijo2 = madre;
    while (punto_cruce == 0) {
      punto_cruce = rand() % padre.length();
    }
    for (int i = punto_cruce; i < padre.length(); ++i) {
      hijo1[i] = madre[i];
    }
    for (int i = punto_cruce; i < madre.length(); ++i) {
      hijo2[i] = padre[i];
    }
  } while (stoi(hijo1, nullptr, 2) < minimos[j] || stoi(hijo1, nullptr, 2) > maximos[j] ||
           stoi(hijo2, nullptr, 2) < minimos[j] || stoi(hijo2, nullptr, 2) > maximos[j]);
  padre = hijo1;
  madre = hijo2;
}
string toBinary(int n) {
  string r;
  if (n == 0) {
    return "0";
  }
  while (n != 0) {
    r = (n % 2 == 0 ? "0" : "1") + r;
    n /= 2;
  }
  return r;
}
struct individuo {
  int x, y, z;
  string x_bin, y_bin, z_bin;
  double funcion, pselec, valor_esp, valor_actual;
  individuo(int x = 0, int y = 0, int z = 0, double f = 0, double p = 0, double v = 0,
            double a = 0) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->x_bin = toBinary(x);
    this->y_bin = toBinary(y);
    this->z_bin = toBinary(z);
    this->funcion = f;
    this->pselec = p;
    this->valor_esp = v;
    this->valor_actual = a;
  }
};
double funcion(int x, int y, int z) { return x * y * z; }
double round1(double var) { return round(var * 100) / 100; }
void cruzar(vector<individuo>& poblacion) {
  for (int i = 0; i <= poblacion.size() / 2; i += 2) {
    mix(poblacion[i].x_bin, poblacion[i + 1].x_bin, 0);
    poblacion[i].x = stoi(poblacion[i].x_bin, nullptr, 2);
    poblacion[i + 1].x = stoi(poblacion[i + 1].x_bin, nullptr, 2);
    mix(poblacion[i].y_bin, poblacion[i + 1].y_bin, 1);
    poblacion[i].y = stoi(poblacion[i].y_bin, nullptr, 2);
    poblacion[i + 1].y = stoi(poblacion[i + 1].y_bin, nullptr, 2);
    mix(poblacion[i].z_bin, poblacion[i + 1].z_bin, 2);
    poblacion[i].z = stoi(poblacion[i].z_bin, nullptr, 2);
    poblacion[i + 1].z = stoi(poblacion[i + 1].z_bin, nullptr, 2);
  }
}
void eliminar(vector<individuo> poblacion, int& max_index, int& min_index) {
  int max = 0;
  int max_f = 0;
  int min = 1000000;
  int min_f = 1000000;
  for (int i = 0; i < poblacion.size(); i++) {
    if (poblacion[i].valor_actual > max ||
        (poblacion[i].valor_actual == max && poblacion[i].funcion > max_f)) {
      max = poblacion[i].valor_actual;
      max_index = i;
      max_f = poblacion[i].funcion;
    }
    if (poblacion[i].valor_actual < min ||
        (poblacion[i].valor_actual == min && poblacion[i].funcion < min_f)) {
      min = poblacion[i].valor_actual;
      min_index = i;
      min_f = poblacion[i].funcion;
    }
  }
}
void muta_ini(vector<individuo>& poblacion) {
  double suma, media;
  for (int i = 0; i < 5; i++) {
    double total = 0;
    double extr;
    if (sw == 1) {
      extr = 0;
    } else {
      extr = 1000000;
    }
    for (int j = 0; j < poblacion.size(); j++) {
      if (i == 0) {
        double tmp5 = round1(funcion(poblacion[j].x, poblacion[j].y, poblacion[j].z));
        poblacion[j].funcion = tmp5;
        total += tmp5;
        if (sw == 1) {
          extr = max(extr, tmp5);
        } else {
          extr = min(extr, tmp5);
        }
      }
      if (i == 1) {
        double tmp5 = round1(poblacion[j].funcion / suma);
        poblacion[j].pselec = tmp5;
        total += tmp5;
      }
      if (i == 2) {
        double tmp5 = round1(poblacion[j].funcion / media);
        poblacion[j].valor_esp = tmp5;
        total += tmp5;
      }
      if (i == 3) {
        double tmp5 = round1(poblacion[j].valor_esp);
        poblacion[j].valor_actual = tmp5;
        total += tmp5;
      }
      if (i == 4) {
        double tmp5 = round(poblacion[j].valor_actual);
        poblacion[j].valor_actual = tmp5;
        total += tmp5;
      }
    }
    if (i == 0) {
      suma = total;
      media = total / poblacion.size();
      suma_fin.push_back(suma);
      media_fin.push_back(media);
      max_min_fin.push_back(extr);
    }
  }
  int max_index = 0;
  int min_index = 0;
  eliminar(poblacion, max_index, min_index);
  individuo tmp;
  if (sw == 1) {
    tmp = poblacion[max_index];
    poblacion.erase(poblacion.begin() + min_index);
    eliminar(poblacion, max_index, min_index);
    poblacion.erase(poblacion.begin() + max_index);
  } else {
    tmp = poblacion[min_index];
    poblacion.erase(poblacion.begin() + max_index);
    eliminar(poblacion, max_index, min_index);
    poblacion.erase(poblacion.begin() + min_index);
  }
  poblacion.insert(poblacion.begin() + poblacion.size() / 2, tmp);
  poblacion.insert(poblacion.begin() + poblacion.size() / 2, tmp);
}
void ordenar(vector<individuo>& poblacion) {
  int xx = 0, yy = 0, zz = 0;
  for (int i = 0; i < poblacion.size(); i++) {
    if (poblacion[i].x_bin.size() > xx) {
      xx = poblacion[i].x_bin.size();
    }
    if (poblacion[i].y_bin.size() > yy) {
      yy = poblacion[i].y_bin.size();
    }
    if (poblacion[i].z_bin.size() > zz) {
      zz = poblacion[i].z_bin.size();
    }
  }
  for (int i = 0; i < poblacion.size(); i++) {
    while (poblacion[i].x_bin.size() < xx) {
      poblacion[i].x_bin = "0" + poblacion[i].x_bin;
    }
    while (poblacion[i].y_bin.size() < yy) {
      poblacion[i].y_bin = "0" + poblacion[i].y_bin;
    }
    while (poblacion[i].z_bin.size() < zz) {
      poblacion[i].z_bin = "0" + poblacion[i].z_bin;
    }
  }
}

bool iguales(vector<individuo> poblacion) {
  for (int i = 0; i <= poblacion.size() / 2; i += 2) {
    if (poblacion[i].x == poblacion[i + 1].x && poblacion[i].y == poblacion[i + 1].y &&
        poblacion[i].z == poblacion[i + 1].z) {
      return true;
    }
  }
  return false;
}
bool iguales2(vector<individuo> poblacion) {
  for (int i = 0; i <= poblacion.size() / 2; i += 2) {
    if (poblacion[i].x_bin != poblacion[i + 1].x_bin &&
        poblacion[i].y_bin != poblacion[i + 1].y_bin &&
        poblacion[i].z_bin != poblacion[i + 1].z_bin) {
      return false;
    }
  }
  return true;
}