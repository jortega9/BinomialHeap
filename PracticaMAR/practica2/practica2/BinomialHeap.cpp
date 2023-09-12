#include "BinomialHeap.h"
#include <time.h>

const int M = 4000;
const int N = 100;

BinomialHeap* insertarElementos() { // como rpuebas pero para inicializar los de unir
	BinomialHeap* m = new BinomialHeap;
	
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 100; j++) {
			int num = rand() % 100000 + 1;
			m->insertar(num);
		}
	}

	return m;
} 

double pruebasInsert(BinomialHeap*& m) {
	vector<double> v;
	double t = 0;

	clock_t start, end;
	
	for (int i = 0; i < M; i++) {
		start = clock();
		for (int j = 0; j < N; j++) {
			int num = rand() % M + 1;
			m->insertar(num);
		}
		end = clock();
		v.push_back(((double)(end - start) / N) / CLOCKS_PER_SEC);
	}

	for (int i = 0; i < v.size(); i++) {
		t += v[i];
	}

	t = t / v.size();

	return t;
}

double pruebasDecrecerClave(BinomialHeap*& m) {
	vector<double> v;
	vector<BinomialTree*> fs;
	double t = 0;

	clock_t start, end;

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			int num = rand() % M + 1;
			BinomialTree* f = m->findNodeinHeap(*m, num);
			fs.push_back(f);
		}
	}

	for (int i = 0; i < M; i++) {
		start = clock();
		for (int j = 0; j < N; j++) {
			int r = rand() % M + 1;
			m->decrecerClave(fs[i*N + j], r);
		}

		end = clock();

		v.push_back(((double)(end - start) / N) / CLOCKS_PER_SEC);
	}

	for (int i = 0; i < v.size(); i++) {
		t += v[i];
	}

	t = t / v.size();

	return t;

}

double pruebasMinimo(BinomialHeap*& m) {
	vector<double> v;

	clock_t start, end;
	double t = 0;

	for (int i = 0; i < M; i++) {
		start = clock();
		for (int j = 0; j < N; j++) {
			int num = rand() % M + 1;
			m->minimo();
		}
		end = clock();
		v.push_back(((double)(end - start) / N) / CLOCKS_PER_SEC);
		
	}

	for (int i = 0; i < v.size(); i++) {
		t += v[i];
	}

	t = t / v.size();

	return t;
}

double pruebasBorrarMin(BinomialHeap*& m) {
	vector<double> v;
	double t = 0;

	clock_t start, end;

	for (int i = 0; i < M; i++) {
		start = clock();
		for (int j = 0; j < N; j++) {
			m->borrarMin();
		}
		end = clock();

		v.push_back(((double)(end - start) / N) / CLOCKS_PER_SEC);
	}

	for (int i = 0; i < v.size(); i++) {
		t += v[i];
	}

	t = t / v.size();

	return t;
}

double pruebasUnir(const BinomialHeap* m1, const BinomialHeap* m2, BinomialHeap& m3) {
	vector<double> v;
	double t = 0;

	clock_t start, end;

	for (int i = 0; i < M; i++) { // hace todo el rato lo mismo
		start = clock();
		for (int j = 0; j < N; j++) {
			m3 = m3.unir(*m1, *m2);
		}
		end = clock();

		v.push_back(((double)(end - start) / N) / CLOCKS_PER_SEC);
	}

	for (int i = 0; i < v.size(); i++) {
		t += v[i];
	}

	t = t / v.size();

	return t;
}

int main() {
	BinomialHeap* m = new BinomialHeap;
	BinomialHeap* m2 = new BinomialHeap;
	BinomialHeap* m3 = new BinomialHeap;
	BinomialHeap* m5 = new BinomialHeap;
	BinomialHeap m4;

	double tIns = 0, tBM = 0, tUnir = 0, tMin = 0, tDecr = 0;

	// inicializo m2 y m3 para pruebas de unir
	m2 = insertarElementos();
	m3 = insertarElementos();
	//inicializo m5 para pruebas decrecer clave (para probarlo con monticulo completo en vez de uno Borrado
	m5 = insertarElementos();

	// comenzamos las pruebas de los metodos
	tIns = pruebasInsert(m);
	tBM = pruebasBorrarMin(m);
	tUnir = pruebasUnir(m2, m3, m4); // uno m2 y m3 y lo guardo en m4
	tMin = pruebasMinimo(m);
	tDecr = pruebasDecrecerClave(m5);

	//int n = N * M;

	//string nombreArchivo = "plot.txt";

	//fstream archivo;
	//archivo.open(nombreArchivo.c_str(), ios::out);

	//archivo << n << "," << tIns << "," << tBM << "," << tUnir << "," << tMin << "," << tDecr << endl;

	//archivo.close();
	

	return 0;
}