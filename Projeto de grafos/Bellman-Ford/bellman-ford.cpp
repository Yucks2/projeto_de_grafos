#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <string>

using namespace std;

struct aresta
{
    int u, v, w;
};

void mostrarhelp()
{
    cout << "Uso: ./bellmanford -f <arquivo_entrada> -i <vertice_inicial> [-o <arquivo_saida>] [-h]\n";
    cout << "-h           : mostra o help\n";
    cout << "-o <arquivo> : redireciona a saída para o 'arquivo'\n";
    cout << "-f <arquivo> : indica o 'arquivo' que contém o grafo de entrada\n";
    cout << "-i <vertice> : vértice inicial\n";
}

int main(int argc, char* argv[])
{
    string inserir_arquivo = "";
    string sair_arquivo = "";
    int vertice_inicial = 1;

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
                cerr << "-f necessita de um nome de arquivo.\n";
                return -1;
            }
        }
        else if(arg == "-i")
        {
            if(i + 1 < argc)
            {
                vertice_inicial = stoi(argv[++i]);
            }
            else
            {
                cerr << "-i necessita de um vértice.\n";
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
            cerr << "Erro ao abrir o arquivo de saída: " << sair_arquivo << "\n";
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
        v--; u--;
        arestas.push_back({v, u, w});
    } 
    
    vector<int> dist(n, INT_MAX);
    int origem = vertice_inicial - 1;
    dist[origem] = 0;
    
    for(int i = 1; i < n; i++)
    {
        bool atualizado = false;
        for(const auto &aresta : arestas)
        {
            if (dist[aresta.u] != INT_MAX && dist[aresta.u] + aresta.w < dist[aresta.v])
            {
                dist[aresta.v] = dist[aresta.u] + aresta.w;
                atualizado = true;
            }
        }
        if(!atualizado)
        {
            break;
        }
    }
    
    bool ciclonegativo = false;
    for(const auto &aresta : arestas)
    {
        if(dist[aresta.u] != INT_MAX && dist[aresta.u] + aresta.w < dist[aresta.v])
        {
            ciclonegativo = true;
            break;
        }
    }
    
    if(ciclonegativo)
    {
        *saida << "O grafo contém ciclo negativo.\n";
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            *saida << (i + 1) << ":" << (dist[i] == INT_MAX ? -1 : dist[i]) << " ";
        }
        *saida << "\n";
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
