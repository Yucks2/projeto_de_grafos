#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <functional>

using namespace std;

// mostra o help
void mostrarhelp()
{
    cout << "Uso: ./kosaraju -f <arquivo_entrada> [-o <arquivo_saida>] [-h]\n";
    cout << "-h           : mostra o help\n";
    cout << "-o <arquivo> : redireciona a saida para o 'arquivo'\n";
    cout << "-f <arquivo> : indica o 'arquivo' que contem o grafo de entrada\n";
}

void dfs1(int v, const vector<vector<int>> &g, vector<bool> &visitado, stack<int> &ordem)
{
    visitado[v] = true;
    for(int u : g[v])
    {
        if(!visitado[u])
        {
            dfs1(u, g, visitado, ordem);
        }
    }
    ordem.push(v);
}

void dfs2(int v, const vector<vector<int>> &gt, vector<bool> &visitado, vector<int> &componente)
{
    visitado[v] = true;
    componente.push_back(v);
    for(int u : gt[v])
    {
        if(!visitado[u])
        {
            dfs2(u, gt, visitado, componente);
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
            cerr << "Erro ao abrir o arquivo de saída: " << sair_arquivo << "\n";
            return -1;
        }
        saida = arquivo_saida;
    }
    
    int n, m;
    *in >> n >> m;
    
    vector<vector<int>> g(n), gt(n);
    for(int i = 0; i < m; i++)
    {
        int v, u;
        *in >> v >> u;
        v--; 
        u--;
        g[v].push_back(u);
        gt[u].push_back(v);
    }
    
    vector<bool> visitado(n, false);
    stack<int> ordem;
    for(int i = 0; i < n; i++)
    {
        if(!visitado[i])
        {
            dfs1(i, g, visitado, ordem);
        }
    }
    
    for(int i = 0; i < n; i++)
    {
        visitado[i] = false;
    }
    
    vector<vector<int>> scc;
    while(!ordem.empty())
    {
        int v = ordem.top();
        ordem.pop();
        if(!visitado[v])
        {
            vector<int> componente;
            dfs2(v, gt, visitado, componente);
            scc.push_back(componente);
        }
    }
    
    for(size_t i = 0; i < scc.size(); i++)
    {
        for(int v : scc[i])
            *saida << (v + 1) << " ";
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
