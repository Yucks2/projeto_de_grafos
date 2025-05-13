#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <tuple>
#include <string>
#include <cstdlib>

using namespace std;

// mostra o help
void mostrarhelp()
{
    cout << "Uso: ./prim -f <arquivo_entrada> -i <vertice_inicial> [-o <arquivo_saida>] [-h]\n";
    cout << "-h           : mostra o help\n";
    cout << "-o <arquivo> : redireciona a saida para o ''arquivo''\n";
    cout << "-f <arquivo> : indica o ''arquivo'' que contem o grafo de entrada\n";
    cout << "-s           : mostra a solução\n";
    cout << "-i <vertice> : vertice inicial\n";
}

int main(int argc, char* argv[])
{
    string inserir_arquivo = "";
    string sair_arquivo = "";
    int vertice_inicial = 1;
    bool mostrar_solucao = false;

    for (int i = 1; i < argc; i++)
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
                cerr << "-f necessita de um nome de arquivo." << endl;
                return 1;
            }
        }
        else if(arg == "-i")
        {
            if(i + 1 < argc)
            {
                vertice_inicial = stoi(argv[++i]) - 1;
            }
            else
            {
                cerr << "-i necessita de um vertice inicial." << endl;
                return 1;
            }
        }
        else if(arg == "-s")
        {
            mostrar_solucao = true;
        }
        else if(arg == "-o")
        {
            if(i + 1 < argc)
            {
                sair_arquivo = argv[++i];
            }
            else
            {
                cerr << "-o necessita de um nome de arquivo" << endl;
                return 1;
            }
        }
    }

    istream* in = &cin;
    if(!inserir_arquivo.empty())
    {
        ifstream *fin = new ifstream(inserir_arquivo);
        if(!fin->is_open())
        {
            cerr << "Erro ao abrir o arquivo de entrada: " << inserir_arquivo << endl;
            return 1;
        }
        in = fin;
    }

    ostream* saida = &cout;
    ofstream *arquvo_saida = nullptr;
    if(!sair_arquivo.empty())
    {
        arquvo_saida = new ofstream(sair_arquivo);
        if(!arquvo_saida->is_open())
        {
            cerr << "Erro ao abrir o arquivo de saida: " << sair_arquivo << endl;
            return -1;
        }
        saida = arquvo_saida;
    }
    
    while(true)
    {
        int n, m;
        if(!(*in >> n >> m))
        {
            break;
        }
        vector<vector<pair<int,int>>> g(n);
        for(int i = 0; i < m; i++)
        {
            int u, v, w;
            *in >> u >> v >> w;
            u--; v--;
            g[u].push_back({v, w});
            g[v].push_back({u, w});
        }

        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<tuple<int,int,int>>> fila_prim;
        vector<bool> mark(n, false);
        vector<tuple<int,int,int>> mstarestas;

        mark[vertice_inicial] = true;
        for(const auto &ed : g[vertice_inicial])
        {
            fila_prim.push(make_tuple(ed.second, ed.first, vertice_inicial));
        }

        int soma = 0;
        
        while(!fila_prim.empty())
        {
            auto atual = fila_prim.top();
            fila_prim.pop();
            int peso = std::get<0>(atual);
            int v = std::get<1>(atual);
            if(mark[v])
            {
                continue;
            }
            mark[v] = true;
            soma += peso;
            mstarestas.push_back(atual);
            for(const auto &aresta : g[v])
            {
                int proximo = aresta.first;
                int w = aresta.second;
                if(!mark[proximo])
                {
                    fila_prim.push(make_tuple(w, proximo, v));
                }
            }
        }
        
        if(mostrar_solucao)
        {
            for(const auto &t: mstarestas)
            {
                int v = std::get<1>(t);
                int pai = std::get<2>(t);
                *saida << "(" << pai + 1 << ", " << v + 1 << ") ";
            }
            *saida << "\n";
        }
        else
        {
            *saida << soma << "\n";
        }
    }
    if(in != &cin)
    {
        delete in;
    }
    if(saida != &cout)
    {
        delete saida;

    }
    return 0;
}
