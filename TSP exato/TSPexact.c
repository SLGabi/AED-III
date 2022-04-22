#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void TSP(int *aux, int *graph, int *path, int *fpath, int *sum, int *fsum, int flag, int nVertex, int b, int currentVertex, int *qtdPath);
float time_diff(struct timeval *start, struct timeval *end);

int main()
{
    int nVertex, i, j, currentVertex, qtdPath = 0;

    struct timeval begin, end;
    FILE *archive;
    
    gettimeofday(&begin, NULL);
    archive = fopen("tsp1_253.txt", "r"); //Abrindo o arquivo e transcrevendo as palavras para uma matriz

    fscanf(archive, "%d ", &nVertex);
    int graph[nVertex][nVertex], aux[nVertex][nVertex], sum = 0, fsum = 99999, path[nVertex - 1], fpath[10000][nVertex - 1], vertice, bestpath[1][nVertex - 1], bestsum = 99999;

    //Colocando o grafo pra dentro da matriz
    for (i = 0; i < nVertex; i++)
    {
        for (j = 0; j < nVertex; j++)
        {
            fscanf(archive, "%d ", &graph[i][j]);
        }
    }
    fclose(archive);

    for (currentVertex = 1; currentVertex <= nVertex; currentVertex++) //Percorrendo todos os vértices pra encontrar o menor caminho
    {
        for (i = 0; i < nVertex; i++)
            for (j = 0; j < nVertex; j++)
                aux[i][j] = 0;

        for (i = 0; i < nVertex; i++)
            aux[i][currentVertex - 1] = currentVertex;

        TSP(aux, graph, path, fpath, &sum, &fsum, 0, nVertex, currentVertex - 1, currentVertex - 1, &qtdPath);

        if (bestsum > fsum)
        {
            vertice = currentVertex;
            bestsum = fsum;
            printf("Melhor custo até agora: %d\n", bestsum);
            for (j = 0; j < nVertex - 1; j++)
            {
                bestpath[0][j] = fpath[0][j];
            }
        }
    }

    printf("\nDistancia minima de viagem = %d.", bestsum);
    printf("\n\tCaminho de viagem %d -->", vertice);
    for (j = 0; j < nVertex - 1; j++)
        printf(" %d -->", bestpath[0][j] + 1);
    printf(" %d.", vertice);

    printf("\n\n");

    gettimeofday (&end, NULL);
    printf("\ntempo de execucao: %0.8f sec\n", time_diff(&begin, &end));
}

void TSP(int *aux, int *graph, int *path, int *fpath, int *sum, int *fsum, int flag, int nVertex, int b, int currentVertex, int *qtdPath)
{
    int i, k;
    flag++;

    for (k = 0; k < nVertex; k++)
    {
        if (*(aux + nVertex * flag + k) == 0) //aux[flag][k]
        {

            *(aux + nVertex * flag + k) = k + 1; //Coloca o vértice que já foi visitado pra posição checada
            *sum = *sum + *(graph + nVertex * b + k); //Incrementa a soma com a posição correspondente da matriz de distâncias
            *(path + flag - 1) = k;  //Adicionando o vértice (sem estar incrementado) ao caminho do caixeiro.

            //Coloca na coluna referente ao vértice que ele já foi visitado
            if (flag < nVertex)
            {
                for (i = flag + 1; i < nVertex; i++)
                    *(aux + i * nVertex + k) = k + 1;
            }

            //Chama a função de forma recursiva até encontrar todo o caminho
            if (flag < nVertex - 1)
                TSP(aux, graph, path, fpath, sum, fsum, flag, nVertex, k, currentVertex, qtdPath); //O b vira k

            //Agora ele faz a mágica
            if (flag == nVertex - 1)
            {
                *sum = *sum + *(graph + nVertex * k + currentVertex); //Soma o caminho com o que falta pra chegar no vértice inicial
                if (*sum == *fsum)              //Caso tenha um caminho com o mesmo tamanho de um já armazenado em fsum, ele seta essa info
                {
                    *qtdPath = *qtdPath + 1; //Ai ele incrementa sc para podermos imprimir os demais caminhos, caso sejam os mais otimizados
                    for (i = 0; i < nVertex - 1; i++)
                        *(fpath + (*qtdPath) * (nVertex - 1) + i) = *(path + i); //Adiciona numa nova linha o caminho
                }
                else if (*sum < *fsum)
                {
                    *fsum = *sum; //Coloca o que foi encontrado no fsum, porque é menor do que o que tem lá
                    *qtdPath = 0;
                    for (i = 0; i < nVertex - 1; i++)
                        *(fpath + i) = *(path + i); //Colocando no fpath o caminho encontrado
                }
                *sum = *sum - *(graph + nVertex * k + currentVertex); //Vai decrementando até limpar a matriz aux novamente
            }

            //Seta 0 novamente
            for (i = flag; i < nVertex; i++)
                *(aux + nVertex * i + k) = 0;
            *sum = *sum - *(graph + nVertex * b + k); //Decrementa o sum
        }
    }
}

float time_diff(struct timeval *start, struct timeval *end) {

    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}