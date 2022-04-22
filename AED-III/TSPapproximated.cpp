#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>

using namespace std;

vector<int> final_path; //Aqui vai ficar o caminho final

int nVertex()
{
    int nV;
    FILE *archive;

    archive = fopen("tsp1_253.txt", "r"); 

    fscanf(archive, "%d", &nV); //Pega só o n° de vértices
    fclose(archive);
    return nV;
}

int numberV = nVertex(); //Pegando o n° de vértices

int minKey(int key[], bool mstSet[]);
vector<vector<int>> MST(int parent[]);
vector<vector<int>> primMST(int **graph);
void DFS(int **matrixAdj, int nNodes, int starting_vertex, bool *visited_nodes);
float time_diff(struct timeval *start, struct timeval *end);

int main()
{
    int first_line, **graph = new int *[numberV];
    struct timeval begin, end;
    FILE *archive;

    gettimeofday(&begin, NULL);
    archive = fopen("tsp1_253.txt", "r"); //Abrindo o arquivo e transcrevendo as palavras para uma struct

    fscanf(archive, "%d", &first_line); //Ignora essa first_line pq ja temos a info dela

    //Criando a matriz e setando 0 em tudo (pq vai preencher dps então whatever)
    for (int i = 0; i < numberV; i++)
    {
        graph[i] = new int[numberV];
        for (int j = 0; j < numberV; j++)
        {
            graph[i][j] = 0;
        }
    }

    //Colocando o grafo pra dentro da matriz
    for (int i = 0; i < numberV; i++)
    {
        for (int j = 0; j < numberV; j++)
        {
            fscanf(archive, "%d ", &graph[i][j]);
        }
    }
    fclose(archive);

    vector<vector<int>> vPrimResult;

    //Vetor que armazena o resultado de PRIM
    vPrimResult = primMST(graph);

    //Criando a matriz adjacente
    int** matrixAdj = new int*[numberV];
    for(int i=0;i<numberV;i++)
    {
        matrixAdj[i] = new int[numberV];
        for(int j=0;j<numberV;j++)
        {
            matrixAdj[i][j] = 0;
        }
    }

    //Colocando onde tem caminho o 1 (baseado no PRIM)
    for(int i=0;i<vPrimResult.size();i++)
    {
        int first_node = vPrimResult[i][0];
        int second_node = vPrimResult[i][1];
        matrixAdj[first_node][second_node] = 1;
        matrixAdj[second_node][first_node] = 1;
    }

    //Guarda se o nodo foi visitado ou não, setando no início falso pra todos
    bool *visited_nodes = new bool[numberV];
    for(int i=0;i<numberV;i++)
    {
        visited_nodes[i] = false;
    }

    DFS(matrixAdj,numberV,0,visited_nodes);

    //Coloca o primeiro vértice no fim agora
    final_path.push_back(final_path[0]);

    //Somando o custo total final
    int sumpath = 0;
    for(int i =0; i < final_path.size(); i++){
        sumpath += graph[final_path[i]][final_path[i+1]];
    }

    //Imprimindo as infos
    cout << "Caminho final: ";
    for(int i=0;i<final_path.size();i++)
    {
        cout << final_path[i] + 1 << " -> ";
    }
    cout << "\n" << "Custo do caminho: " << sumpath;
    cout << "\n";

    gettimeofday (&end, NULL);
    printf("\ntempo de execucao: %0.8f sec\n", time_diff(&begin, &end));
    return 0; 
}

int minKey(int key[], bool mstSet[])
{
    int min = INT_MAX, minIndex;

    for (int v = 0; v < numberV; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], minIndex = v;

    return minIndex;
}

vector<vector<int>> MST(int parent[])
{
    vector<vector<int>> v;
    for (int i = 1; i < numberV; i++)
    {
        vector<int> p;
        p.push_back(parent[i]);
        p.push_back(i);
        v.push_back(p);
        p.clear();
    }
    return v;
}

//Pegando a MST do grafo dado, utilizando o algoritmo de Prim
vector<vector<int>> primMST(int **graph)
{
    int parent[numberV];
    int key[numberV];

    bool mstSet[numberV];

    //Iniciando o key value com infinito e falso pra todo mstSet
    for (int i = 0; i < numberV; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    //Pegando o primeiro vértice e colocando 0
    key[0] = 0;
    parent[0] = -1;

    //O loop
    for (int count = 0; count < numberV - 1; count++)
    {
        //Verificando e atualizando os valores da key
        int u = minKey(key, mstSet);
        mstSet[u] = true;
        for (int v = 0; v < numberV; v++)
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
            {
                parent[v] = u;
                key[v] = graph[u][v];
            }
    }

    vector<vector<int>> v;
    v = MST(parent);

    return v;
}

void DFS(int **matrixAdj, int nNodes, int starting_vertex, bool *visited_nodes)
{
    //Colocando o vértice no final
    final_path.push_back(starting_vertex); 

    //Setando pra visitado agora o vertice
    visited_nodes[starting_vertex] = true; 

    //Fazendo por recursão
    for(int i=0;i<nNodes;i++) 
    {
        if(i==starting_vertex) 
        {
            continue;
        }
        if(matrixAdj[starting_vertex][i]==1)
        {
            if(visited_nodes[i])
            {
                continue;
            }
            DFS(matrixAdj,nNodes,i,visited_nodes);
        }
    }
}

float time_diff(struct timeval *start, struct timeval *end) {

    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}