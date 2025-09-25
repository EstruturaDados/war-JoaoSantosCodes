// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <string.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define TAM_MAX_NOME 50
#define TAM_MAX_COR 20

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_MAX_NOME];
    char corExercito[TAM_MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Nível Novato: cadastro e exibição
void limparBufferEntrada(void);
void lerString(char* destino, int tamanhoMax);
void cadastrarTerritorios(Territorio territorios[], int tamanho);
void exibirMapa(const Territorio territorios[], int tamanho);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1) Setup básico
    setlocale(LC_ALL, "");

    Territorio territorios[NUM_TERRITORIOS];

    // 2) Cadastro dos 5 territórios
    cadastrarTerritorios(territorios, NUM_TERRITORIOS);

    // 3) Exibir o mapa resultante
    printf("\n================ MAPA ATUAL ================\n");
    exibirMapa(territorios, NUM_TERRITORIOS);

    printf("\nFim do nível Novato. Você pode evoluir o código para os próximos níveis.\n");
    return 0;
}

// --- Implementação das Funções ---

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerString(char* destino, int tamanhoMax) {
    if (fgets(destino, tamanhoMax, stdin)) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            // remove excesso até nova linha
            limparBufferEntrada();
        }
    }
}

void cadastrarTerritorios(Territorio territorios[], int tamanho) {
    printf("Cadastro de %d territórios\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        printf("\nTerritório %d\n", i + 1);

        printf("Nome: ");
        lerString(territorios[i].nome, TAM_MAX_NOME);

        printf("Cor do exército: ");
        lerString(territorios[i].corExercito, TAM_MAX_COR);

        printf("Número de tropas: ");
        while (scanf("%d", &territorios[i].tropas) != 1 || territorios[i].tropas < 0) {
            printf("Entrada inválida. Digite um número inteiro não negativo: ");
            limparBufferEntrada();
        }
        limparBufferEntrada(); // limpar após scanf
    }
}

void exibirMapa(const Territorio territorios[], int tamanho) {
    printf("\n%-3s | %-20s | %-15s | %-6s\n", "#", "Nome", "Cor", "Tropas");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-3d | %-20s | %-15s | %-6d\n", i + 1, territorios[i].nome, territorios[i].corExercito, territorios[i].tropas);
    }
}
