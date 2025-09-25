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
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

// Nível Aventureiro: alocação dinâmica e simulação de ataques
Territorio* alocarMapa(int numTerritorios);
void liberarMemoria(Territorio* territorios);
int simularAtaque(Territorio* atacante, Territorio* defensor);
void faseDeAtaque(Territorio* territorios, int numTerritorios);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1) Setup básico
    setlocale(LC_ALL, "Portuguese_Brazil.1252"); // Configurar para português brasileiro
    srand((unsigned int)time(NULL)); // Inicializar gerador de números aleatórios

    // 2) Alocação dinâmica de memória para territórios
    Territorio* territorios = alocarMapa(NUM_TERRITORIOS);

    // 3) Cadastro dos territórios
    cadastrarTerritorios(territorios, NUM_TERRITORIOS);

    // 4) Exibir o mapa inicial
    printf("\n[*] INICIANDO JOGO WAR - NIVEL AVENTUREIRO [*]\n");
    exibirMapa(territorios, NUM_TERRITORIOS);

    // 5) Fase de ataques (Nível Aventureiro)
    faseDeAtaque(territorios, NUM_TERRITORIOS);

    // 6) Exibir o mapa final
    printf("\n[*] RESULTADO FINAL DA BATALHA [*]\n");
    exibirMapa(territorios, NUM_TERRITORIOS);

    // 7) Liberar memória alocada
    liberarMemoria(territorios);

    printf("\n[*] PARABENS! Voce completou o nivel Aventureiro com sucesso! [*]\n");
    printf("[*] Proximo desafio: Evolua o codigo para o nivel Mestre!\n");
    printf("[*] Sugestoes para o nivel Mestre:\n");
    printf("   - Sistema de cartas e missoes\n");
    printf("   - Multiplos jogadores\n");
    printf("   - Inteligencia artificial\n");
    printf("   - Interface grafica\n");
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
    printf("\n================ CADASTRO DE TERRITORIOS ================\n");
    printf("Configure os %d territorios do mapa de guerra:\n", tamanho);
    printf("=========================================================\n");
    
    for (int i = 0; i < tamanho; i++) {
        printf("\n[*] TERRITORIO %d/%d\n", i + 1, tamanho);
        printf("---------------------------------------------------------\n");

        printf(">> Nome do territorio: ");
        lerString(territorios[i].nome, TAM_MAX_NOME);

        printf(">> Cor do exercito (ex: azul, vermelho, verde): ");
        lerString(territorios[i].corExercito, TAM_MAX_COR);

        printf(">> Numero de tropas iniciais: ");
        while (scanf("%d", &territorios[i].tropas) != 1 || territorios[i].tropas < 1) {
            printf("[!] Entrada invalida! Digite um numero inteiro positivo (minimo 1): ");
            limparBufferEntrada();
        }
        limparBufferEntrada(); // limpar após scanf
        
        printf("[OK] Territorio '%s' cadastrado com sucesso!\n", territorios[i].nome);
    }
    
    printf("\n[*] Cadastro concluido! Todos os territorios foram configurados.\n");
}

void exibirMapa(const Territorio territorios[], int tamanho) {
    printf("\n[MAP] MAPA DOS TERRITORIOS\n");
    printf("=======================================================================\n");
    printf("%-4s | %-20s | %-15s | %-8s\n", "#", "TERRITORIO", "EXERCITO", "TROPAS");
    printf("-----+----------------------+-----------------+----------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-4d | %-20s | %-15s | %-8d\n", 
               i + 1, 
               territorios[i].nome, 
               territorios[i].corExercito, 
               territorios[i].tropas);
    }
    printf("=======================================================================\n");
}

// --- Funções do Nível Aventureiro ---

Territorio* alocarMapa(int numTerritorios) {
    Territorio* territorios = (Territorio*)malloc(numTerritorios * sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro: Não foi possível alocar memória para os territórios.\n");
        exit(1);
    }
    return territorios;
}

void liberarMemoria(Territorio* territorios) {
    if (territorios != NULL) {
        free(territorios);
    }
}

int simularAtaque(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 1) {
        printf("[!] ERRO: O atacante precisa ter pelo menos 2 tropas para atacar.\n");
        return 0; // Ataque falhou
    }
    
    if (defensor->tropas <= 0) {
        printf("[!] ERRO: O defensor nao possui tropas para defender.\n");
        return 0; // Ataque falhou
    }
    
    // Simular dados (1-6 para cada lado)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("\n[*] COMBATE EM ANDAMENTO!\n");
    printf("---------------------------------------------------------------------\n");
    printf("[ATK] ATACANTE: %s (Exercito %s)\n", atacante->nome, atacante->corExercito);
    printf("      Tropas disponiveis: %d | Resultado do dado: %d\n", atacante->tropas, dadoAtacante);
    printf("\n[DEF] DEFENSOR: %s (Exercito %s)\n", defensor->nome, defensor->corExercito);
    printf("      Tropas disponiveis: %d | Resultado do dado: %d\n", defensor->tropas, dadoDefensor);
    printf("---------------------------------------------------------------------\n");
    
    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        defensor->tropas--;
        printf("[WIN] VITORIA DO ATACANTE!\n");
        printf("      %s perde 1 tropa no combate.\n", defensor->nome);
        
        if (defensor->tropas == 0) {
            // Território conquistado
            printf("\n[***] CONQUISTA TOTAL! [***]\n");
            printf("      %s conquistou completamente %s!\n", atacante->nome, defensor->nome);
            printf("      Transferindo controle do territorio...\n");
            
            strcpy(defensor->corExercito, atacante->corExercito);
            defensor->tropas = atacante->tropas - 1;
            atacante->tropas = 1;
            
            printf("      %s agora pertence ao exercito %s com %d tropas!\n", 
                   defensor->nome, defensor->corExercito, defensor->tropas);
            return 2; // Território conquistado
        } else {
            printf("      Territorio ainda resiste com %d tropa(s)!\n", defensor->tropas);
        }
        return 1; // Ataque bem-sucedido
    } else {
        // Defensor vence
        atacante->tropas--;
        printf("[DEF] DEFESA BEM-SUCEDIDA!\n");
        printf("      %s perde 1 tropa no ataque fracassado.\n", atacante->nome);
        printf("      %s mantem o controle com %d tropa(s).\n", defensor->nome, defensor->tropas);
        return 0; // Ataque falhou
    }
}

void faseDeAtaque(Territorio* territorios, int numTerritorios) {
    printf("\n[*] FASE DE COMBATE [*]\n");
    printf("=======================================================================\n");
    printf("[*] INSTRUCOES:\n");
    printf("   - Escolha um territorio atacante (deve ter pelo menos 2 tropas)\n");
    printf("   - Escolha um territorio defensor (cor diferente do atacante)\n");
    printf("   - O resultado sera determinado por dados aleatorios\n");
    printf("   - Digite -1 para encerrar a fase de ataques\n");
    printf("=======================================================================\n");
    
    int atacanteIdx, defensorIdx;
    
    while (1) {
        printf("\n[INFO] SITUACAO ATUAL DO CAMPO DE BATALHA:\n");
        exibirMapa(territorios, numTerritorios);
        
        printf("\n[ATK] Escolha o territorio ATACANTE (1-%d) ou -1 para parar: ", numTerritorios);
        if (scanf("%d", &atacanteIdx) != 1) {
            limparBufferEntrada();
            printf("[!] Entrada invalida! Digite apenas numeros.\n");
            continue;
        }
        limparBufferEntrada();
        
        if (atacanteIdx == -1) {
            printf("\n[END] Fase de ataques encerrada pelo jogador.\n");
            break;
        }
        
        if (atacanteIdx < 1 || atacanteIdx > numTerritorios) {
            printf("[!] Territorio invalido! Escolha entre 1 e %d.\n", numTerritorios);
            continue;
        }
        
        if (territorios[atacanteIdx - 1].tropas <= 1) {
            printf("[!] O territorio '%s' nao tem tropas suficientes para atacar!\n", territorios[atacanteIdx - 1].nome);
            printf("    Dica: E necessario ter pelo menos 2 tropas (1 fica para defesa).\n");
            continue;
        }
        
        printf("[DEF] Escolha o territorio DEFENSOR (1-%d): ", numTerritorios);
        if (scanf("%d", &defensorIdx) != 1) {
            limparBufferEntrada();
            printf("[!] Entrada invalida! Digite apenas numeros.\n");
            continue;
        }
        limparBufferEntrada();
        
        if (defensorIdx < 1 || defensorIdx > numTerritorios) {
            printf("[!] Territorio invalido! Escolha entre 1 e %d.\n", numTerritorios);
            continue;
        }
        
        if (atacanteIdx == defensorIdx) {
            printf("[!] Um territorio nao pode atacar a si mesmo!\n");
            printf("    Dica: Escolha territorios diferentes.\n");
            continue;
        }
        
        if (strcmp(territorios[atacanteIdx - 1].corExercito, territorios[defensorIdx - 1].corExercito) == 0) {
            printf("[!] Territorios aliados nao podem se atacar!\n");
            printf("    Dica: Escolha territorios de cores diferentes.\n");
            continue;
        }
        
        // Executar o ataque
        printf("\n[*] INICIANDO COMBATE...\n");
        simularAtaque(&territorios[atacanteIdx - 1], &territorios[defensorIdx - 1]);
        
        printf("\n[PAUSE] Pressione Enter para continuar...");
        getchar();
    }
}
