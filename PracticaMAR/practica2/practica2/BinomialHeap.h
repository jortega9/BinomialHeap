#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <cctype>
#include <list>

using namespace std;

// duda con sibling 

struct BinomialTree {
	int key;
	BinomialTree* child;
	BinomialTree* sibling;
	BinomialTree* parent;
	int degree;
};


class BinomialHeap {
private:
	list<BinomialTree*> trees; // lista de arboles binomiales almacenando sus raices
	int size; // tamano de monticulo
	BinomialTree* min; // puntero a la raiz del arbol binomial de menor valor

public:
	BinomialHeap() {
		size = 0;
		min = NULL;

	}

	BinomialTree* unirBinomialTrees(BinomialTree* n1, BinomialTree* n2) { // O(1) 
		if (n1->key > n2->key) { // asefuro que n1 sea menor que n2
			swap(n1, n2);
		}

		n2->parent = n1;
		n2->sibling = n1->child;
		n1->child = n2;
		n1->degree++;
		n2->degree++;

		return n1;
	}

	BinomialHeap unirMonticulos(BinomialHeap mont1, BinomialHeap mont2) { //log(n1) + log(n2) + 2 -> O(log(n))

		BinomialHeap* m = new BinomialHeap;
		m->trees = list<BinomialTree*>(m->size, NULL);
		m->size = mont1.size + mont2.size;

		// compruebo los minimos de ambos monticulos y selecciono el menor
		
		if (mont1.size != 0 && mont2.size != 0) {
			if (mont1.min->key < mont2.min->key) m->min = mont1.min;
			else m->min = mont2.min;
		}
		else if (mont1.size == 0) {
			m->min = mont2.min;
		}
		else {
			m->min = mont1.min;
		}

		list<BinomialTree*>::iterator it1 = mont1.trees.begin();
		list<BinomialTree*>::iterator it2 = mont2.trees.begin();

		// recorro las listas de raices de los monticulos y las voy uniendo en m
		
		while (it1 != mont1.trees.end() && it2 != mont2.trees.end()) {

			if ((*it1)->degree <= (*it2)->degree) {
				m->trees.push_back(*it1);
				it1++;
			}
			else {
				m->trees.push_back(*it2);
				it2++;
			}
		}

		while (it1 != mont1.trees.end()) {
			m->trees.push_back(*it1);
			it1++;
		}

		while (it2 != mont2.trees.end()) {
			m->trees.push_back(*it2);
			it2++;
		}

		return *m;
	}

	BinomialHeap unir(BinomialHeap m1, BinomialHeap m2) { //O(logn)
		BinomialHeap m;
		m = unirMonticulos(m1, m2); // O(log(n1) + log(n2) + 2) unimos los monticulos
		m.controlDegrees(); // O(log(n)) controlamos los grados de los arboles binomiales
		return m;
	}

	BinomialTree* findNodeinHeap(BinomialHeap m, int k) { //O(logn) 
		list<BinomialTree*>::iterator it = m.trees.begin();
		while (it != m.trees.end()) {
			BinomialTree* n = findNodeinTree(*it, k);
			if (n != NULL) return n;
			it++;
		}
		return NULL;
	}

	BinomialTree* findNodeinTree(BinomialTree* n, int k) 
	{
		if (n == NULL)
			return NULL;
		
		if (n->key > k) {
			return NULL;
		}

		if (n->key == k)
			return n;

		BinomialTree* c = findNodeinTree(n->child, k);
		if (c != NULL)
			return c;

		return findNodeinTree(n->sibling, k);
	}

	BinomialHeap controlDegrees() { // Funcion para controlar los grados de los arboles binomiales
		if (size <= 1) return *this;

		BinomialHeap* m = new BinomialHeap;
		list<BinomialTree*>::iterator i, j, k;

		i = trees.begin();
		j = trees.begin();
		k = trees.begin();

		if (size == 2) {
			j = i;
			j++;
			k = trees.end();
		}
		else {
			j++;
			k = j;
			k++;
		}

		// recorro la lista de arboles binomiales y ajusto los grados para mantener el invariante.

		while (i != trees.end()) {

			if (j == trees.end()) {
				i++;
			}
			else if ((*i)->degree < (*j)->degree) {
				i++;
				j++;
				if (k != trees.end()) {
					k++;
				}
			}
			else if (k != trees.end() && (*i)->degree == (*j)->degree && (*i)->degree == (*k)->degree) {
				i++;
				j++;
				k++;
			}
			else if ((*i)->degree == (*j)->degree && i != j) {
				*i = unirBinomialTrees(*i, *j);
				j = trees.erase(j);
				size = trees.size();
				if (k != trees.end()) {
					k++;
				}

			}
			else {
				i++;
			}
		}

		return *this;

	}


	void decrecerClave(BinomialTree* m, int newKey) { // Decremento la clave de un nodo

		if (m == NULL) return;

		if (newKey < m->key) {

			BinomialTree* m1 = m;

			m1->key = newKey;
			
			BinomialTree* p = new BinomialTree;
			p = m1->parent;

			while (p != NULL && m1->key < p->key) { 
				int a = m1->key;
				m1->key = p->key;
				p->key = a;

				m1 = p;
				p = p->parent;
			}

			
			//if (newKey < this->min->key) {  
			//	this->min = m1;
			//}
			
			this->min = recalcularMin();

		}

	}


	void insertar(int k) { //O(logn)
		BinomialTree* n = new BinomialTree;

		n->key = k;
		n->sibling = NULL;
		n->parent = NULL;
		n->child = NULL;
		n->degree = 0;

		//if (findNodeinHeap(*this, k) != NULL) return; // en el caso de que quisiesemos que no se repitieran claves
		

		BinomialHeap* m = new BinomialHeap;

		m->min = n;
		m->trees.push_back(n);
		m->size = 1;

		*this = unirMonticulos(*this, *m);

		this->controlDegrees();

		//this->min = recalcularMin();

	}

	BinomialTree* minimo() { //O(1)
		return min;
	}

	BinomialTree* recalcularMin() { // O(log(n)) solo recorro las cabezas al ser monticulo de minimos

		BinomialTree* min = this->trees.front();

		list<BinomialTree*>::iterator it;

		it = this->trees.begin();

		while (it != this->trees.end()) {
			if ((*it)->key < min->key) {
				min = *it;
			}
			it++;
		}

		return min;
	}

	int get_size() { //O(1)
		return this->size;
	}

	int borrarMin() { // O(log(n))

		BinomialTree* min = new BinomialTree;
		min = this->min;
		
		if (min == NULL) return -1;

		// BORRO EL MINIMO Y REAJUSTO LOS HERMANOS DE LOS ARBOLES

		BinomialTree* t = min->child;
		BinomialTree* nt;
		BinomialHeap* m = new BinomialHeap;
		int minKey = INT_MAX;
		
		if (t != NULL) {
			m->min = t;
			minKey = m->min->key;
		}

		this->trees.remove(min);
		if(this->size != 0)	this->size--;

		while (t != NULL)
		{
			nt = t;
			t = t->sibling;
			nt->sibling = NULL;
			m->trees.push_front(nt);
			m->size++;
			nt->degree--;

			if (nt->parent == min) {
				nt->parent = NULL;
			}

			if (nt->key < minKey) {
				minKey = nt->key;
				m->min = nt;
			}
		}

		if(size != 0) this->min = recalcularMin();
		*this = unirMonticulos(*this, *m);
		this->controlDegrees();
		this->size = this->trees.size();

		return min->key;

	}


};