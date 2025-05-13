#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

struct aresta
{
    int u, v, w;
};

// mostra o help
void mostrarhelp()
{
    cout << "Uso: ./kruskal -f <arquivo_entrada> [-o <arquivo_saida>] [-h]\n";
    cout << "-h           : mostra o help\n";
    cout << "-o <arquivo> : redireciona a saida para o ''arquivo''\n";
    cout << "-f <arquivo> : indica o ''arquivo'' que contem o grafo de entrada\n";
}

int find(vector<int>& pai, int i)
{
    if(pai[i] != i)
    {
        pai[i] = find(pai, pai[i]);
    }
    return pai[i];
}

void unionf(vector<int>& pai, vector<int>& ranque, int x, int y)
{
    int raizx = find(pai, x);
    int raizy = find(pai, y);
    if(raizx != raizy)
    {
        if(ranque[raizx] < ranque[raizy])
        {
            pai[raizx] = raizy;
        }
        else if(ranque[raizx] > ranque[raizy])
        {
            pai[raizy] = raizx;
        }
        else
        {
            pai[raizy] = raizx;
            ranque[raizx]++;
        }
    }
}

int main(int argc, char* argv[])
{
    string inserir_arquivo = "";
    string sair_arquivo = "";

    for(int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if(arg == "-h")
        {
            mostrarhelp();
            return 0;
        }
        else if(arg == "-f")
        {
            if(i + 1 < argc)
            {
                inserir_arquivo = argv[++i];
            }
            else
            {
                cerr << "-f necessita de um nome de arquivo.\n";
                return -1;
            }
        }
        else if(arg == "-o")
        {
            if(i + 1 < argc)
            {
                sair_arquivo = argv[++i];
            }
            else
            {
                cerr << "-o necessita de um nome de arquivo.\n";
                return -1;
            }
        }
    }

    istream* in = &cin;
    if(!inserir_arquivo.empty())
    {
        ifstream* fin = new ifstream(inserir_arquivo);
        if(!fin->is_open())
        {
            cerr << "Erro ao abrir o arquivo de entrada: " << inserir_arquivo << "\n";
            return -1;
        }
        in = fin;
    }

    ostream* saida = &cout;
    ofstream* arquivo_saida = nullptr;
    if(!sair_arquivo.empty())
    {
        arquivo_saida = new ofstream(sair_arquivo);
        if(!arquivo_saida->is_open())
        {
            cerr << "Erro ao abrir o arquivo de saida: " << sair_arquivo << "\n";
            return -1;
        }
        saida = arquivo_saida;
    }

    int n, m;
    *in >> n >> m;

    vector<aresta> arestas;
    for(int i = 0; i < m; i++)
    {
        int v, u, w;
        *in >> v >> u >> w;
        arestas.push_back({v - 1, u - 1, w});
    }

    sort(arestas.begin(), arestas.end(), [](const aresta &a, const aresta &b)
    {
        return a.w < b.w;
    });

    vector<int> pai(n);
    vector<int> ranque(n, 0);
    for (int i = 0; i < n; i++)
    {
        pai[i] = i;
    }

    vector<aresta> mst;
    int peso_total = 0;
    for(auto &aresta : arestas)
    {
        int inserir_u = find(pai, aresta.u);
        int inserir_v = find(pai, aresta.v);
        if(inserir_u != inserir_v)
        {
            mst.push_back(aresta);
            peso_total += aresta.w;
            unionf(pai, ranque, inserir_u, inserir_v);
        }
    }

    if(mst.size() != static_cast<size_t>(n - 1))
    {
        *saida << "Grafo nao conexo.\n";
    }
    else
    {
        *saida << "Peso total: " << peso_total << "\n";
        *saida << "Arestas:\n";
        for(auto &aresta : mst)
        {
            *saida << (aresta.u + 1) << " - " << (aresta.v + 1) << " (Peso: " << aresta.w << ")\n";
        }
    }

    if(arquivo_saida)
    {
        arquivo_saida->close();
        delete arquivo_saida;
    }
    if(in != &cin)
    {
        ifstream* arquivo_entrada = dynamic_cast<ifstream*>(in);
        if(arquivo_entrada)
        {
            arquivo_entrada->close();
            delete arquivo_entrada;
        }
    }
    return 0;
}
