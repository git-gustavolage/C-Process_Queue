#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_PROCESSOS 100

typedef enum
{
    PRONTO,
    EXECUTANDO,
    TERMINADO
} EstadoProcesso;


typedef struct
{
    int id;
    double prioridade;
    double tempo_execucao;
    EstadoProcesso estado;
} Processo;


typedef struct
{
    Processo processos[MAX_PROCESSOS];
    int tamanho;
} GerenciadorProcessos;


void iniciar_gerenciador(GerenciadorProcessos *gerenciador)
{
    gerenciador->tamanho = 0;
}

//bubble sort para ordenar osp rocessos de acordo com a sua prioridade
void ordenar_processos(GerenciadorProcessos *gerenciador)
{
    for(int i = 0; i < gerenciador->tamanho - 1; i++){
        for(int j = 0; j < gerenciador->tamanho - i - 1; j++){
            if(gerenciador->processos[j].prioridade < gerenciador->processos[j + 1].prioridade){
                Processo temp = gerenciador->processos[j];
                gerenciador->processos[j] = gerenciador->processos[j + 1];
                gerenciador->processos[j + 1] = temp;
            }
        }
    }
}


void exibir_processos(GerenciadorProcessos *gerenciador)
{
    for (int i = 0; i < gerenciador->tamanho; i++)
    {
        printf("Processo ID: %d Prioridade: %.2f.\n", gerenciador->processos[i].id, gerenciador->processos[i].prioridade);
    }
    printf("\n\n");
}


bool adicionar_processo(GerenciadorProcessos *gerenciador, int id, double prioridade, double tempo_execucao)
{
    if (gerenciador->tamanho >= MAX_PROCESSOS)
        return 0;

    Processo novo_processo = {id, prioridade, tempo_execucao};
    gerenciador->processos[gerenciador->tamanho++] = novo_processo;
    ordenar_processos(gerenciador);
    return 1;
}


void remover_processos_terminados(GerenciadorProcessos *gerenciador)
{
    int novo_tamanho = 0;
    for (int i = 0; i < gerenciador->tamanho; i++){
        if (gerenciador->processos[i].estado != TERMINADO){
            gerenciador->processos[novo_tamanho++] = gerenciador->processos[i];
        }
    }
    gerenciador->tamanho = novo_tamanho;
    ordenar_processos(gerenciador);
}


//itera por todos os itens (já ordenados) e os executa até o seu término
void executar_processos(GerenciadorProcessos *gerenciador)
{
    for (int i = 0; i < gerenciador->tamanho; i++){
        if (gerenciador->processos[i].estado == PRONTO){
            printf("Executando processo ID: %d\n", gerenciador->processos[i].id);
            gerenciador->processos[i].estado = EXECUTANDO;
            sleep(gerenciador->processos[i].tempo_execucao);
            gerenciador->processos[i].estado = TERMINADO;
            printf("Processo ID: %d terminou.\n", gerenciador->processos[i].id);
        }
    }

    remover_processos_terminados(gerenciador);
}


//executa em forma de fila circular. dando para cada processo uma pequena quantidade de tempo para ele ser executado.
//evita a monopolização da cpu por um unico processo;
//evita a "fome de processos" (situação em que processos de baixa prioridade nunca são executados);
void round_robin(GerenciadorProcessos *gerenciador)
{
    while (gerenciador->tamanho > 0){
        for (int i = 0; i < gerenciador->tamanho; i++){
            if (gerenciador->processos[i].estado == PRONTO){
                printf("Executando processo ID: %d\n", gerenciador->processos[i].id);
                gerenciador->processos[i].estado = EXECUTANDO;

                sleep(1);
                gerenciador->processos[i].tempo_execucao -= 1.0;

                if (gerenciador->processos[i].tempo_execucao <= 0){
                    gerenciador->processos[i].estado = TERMINADO;
                    printf("Processo ID: %d terminou.\n", gerenciador->processos[i].id);
                }
                else{
                    gerenciador->processos[i].estado = PRONTO;
                }
            }
        }
        remover_processos_terminados(gerenciador);
        sleep(1);
    }
}


int main()
{
    GerenciadorProcessos gerenciador;
    iniciar_gerenciador(&gerenciador);

    adicionar_processo(&gerenciador, 1, 0.0, 3.0);
    adicionar_processo(&gerenciador, 2, 8.0, 2.0);
    adicionar_processo(&gerenciador, 3, 3.0, 4.0);
    adicionar_processo(&gerenciador, 4, 1.0, 4.0);
    adicionar_processo(&gerenciador, 5, 20.0, 4.0);
    adicionar_processo(&gerenciador, 6, 10.0, 4.0);
    adicionar_processo(&gerenciador, 7, 11.0, 4.0);
    adicionar_processo(&gerenciador, 8, 12.0, 4.0);

    exibir_processos(&gerenciador);

    round_robin(&gerenciador);

    return 0;
}
