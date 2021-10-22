#include <bits/stdc++.h>

using namespace std;

#define ii pair<int, int>
#define ff first
#define ss second

bool comp(ii a, ii b){
	return a.ss > b.ss;
}

int my_random(int i){
	return rand() % i;
}

void shuffle_iguais(vector<ii> &grau){
	vector<ii>::iterator it = grau.begin();
	while (it != grau.end()){
		vector<ii>::iterator aux = upper_bound(it, grau.end(), *it, comp);
		random_shuffle(it, aux, my_random);
		it = aux;
	}
}

void unir_vector(vector<ii> &a, vector<ii> b){
	for (ii aux : b){
		a.push_back(aux);
	}
}

vector<ii> dfs_componente(vector<vector<int> > g, int u, vector<int> &cor){
	vector<ii> result;
	for (int v : g[u]){
		if (cor[v] == 0){
			cor[v] = 1;
			unir_vector(result, dfs_componente(g, v, cor));
			cor[v] = 2;
		}
		if (cor[v] == 1){
			result.push_back(ii(min(u,v), max(u,v)));
		}
	}

	return result;
}

vector<ii> juntar_componentes(vector<ii> a, vector<ii> b){
	int x = rand() % a.size();
	int y = rand() % b.size();

	vector<ii> result;
	for (int i = 0; i < (int)a.size(); i++){
		if (i != x) result.push_back(a[i]);
	}
	for (int j = 0; j < (int)b.size(); j++){
		if (j != y) result.push_back(b[j]);
	}

	int u = a[x].ff;
	int v = a[x].ss;
	int c = b[y].ff;
	int d = b[y].ss;

	result.push_back(ii(min(u, c), max(u, c)));
	result.push_back(ii(min(v, d), max(v, d)));

	return result;
}

int main(int argc, char *argv[]){

	if (argc < 4){
		printf("Numero invalido de argumentos\n");
		printf("./eulerian_graph [quantidade_vertices] [semente_aleatoria] [densidade]\n");
		exit(1);
	}

	int n = stoi(argv[1]);

	unsigned seed = stoi(argv[2]);
	srand(seed);
	double p = stod(argv[3]);

	int m = ((n * (n - 1)) / 2 * p) + 0.000001;
	m = max(m, n);
	int m_ = m - n;
	vector<ii> grau;
	grau.resize(n);
	for (int i = 0; i < n; i++){
		grau[i] = ii(i, 2);
	}


	while (m_ > 0){
		int r = rand() % n;
		grau[r].ss += 2;
		m_ -= 1;
	}

	sort(grau.begin(), grau.end(), comp);
	shuffle_iguais(grau);

	vector<vector<int> > g;
	g.resize(n);
	while (!grau.empty()){
		int i = rand() % grau.size();
		int u = grau[i].ff;
		int du = grau[i].ss;
		grau.erase(grau.begin() + i);

		if (du > (int)grau.size()){
			printf("Nao foi possivel gerar o grafo.\n");
			exit(1);
		}

		for (int j = 0; j < du; j++){
			grau[j].ss--;
			g[u].push_back(grau[j].ff);
			g[grau[j].ff].push_back(u);
		}

		for (int j = 0; j < du; j++){
			if (grau[j].ss == 0){
				grau.erase(grau.begin() + j);
			}
		}

		sort(grau.begin(), grau.end(), comp);
		shuffle_iguais(grau);
	}

	vector<vector<ii> > componentes;
	vector<int> cor;
	cor.resize(n, 0);

	for (int i = 0; i < n; i++){
		if (cor[i] == 0){
			cor[i] = 1;
			componentes.push_back(dfs_componente(g, i, cor));
			cor[i] = 2;
		}
	}

	while (componentes.size() > 1){
		vector<ii> a = componentes[componentes.size() - 1];
		componentes.pop_back();
		vector<ii> b = componentes[componentes.size() - 1];
		componentes.pop_back();
		componentes.push_back(juntar_componentes(a, b));
	}


	printf("%d %d\n", n, m);
	for (ii edge : componentes[0]){
		printf("%d %d\n", edge.ff, edge.ss);
	}

	return 0;
}