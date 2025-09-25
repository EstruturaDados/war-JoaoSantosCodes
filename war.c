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
#define NUM_CARTAS 8
#define NUM_MISSOES 5

// --- Enumerações ---
// Define os tipos de cartas especiais disponíveis no nível Master
typedef enum {
    CARTA_REFORCO = 1,      // +2 tropas em um território
    CARTA_ATAQUE_DUPLO,     // Permite atacar duas vezes
    CARTA_DEFESA_EXTRA,     // +1 dado de defesa
    CARTA_CONQUISTA_RAPIDA, // Conquista território com 1 tropa
    CARTA_SABOTAGEM,        // Remove 1 tropa do inimigo
    CARTA_ALIANCA,          // Impede ataques por 1 turno
    CARTA_ESPIONAGEM,       // Revela tropas inimigas
    CARTA_BOMBARDEIO        // Ataque à distância
} TipoCarta;

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_MAX_NOME];
    char corExercito[TAM_MAX_COR];
    int tropas;
    int protegido;  // 0 = não protegido, 1 = protegido por aliança
} Territorio;

// Define a estrutura para uma carta especial
typedef struct {
    TipoCarta tipo;
    char nome[TAM_MAX_NOME];
    char descricao[100];
    int usada;  // 0 = disponível, 1 = já usada
} Carta;

// Define a estrutura para uma missão
typedef struct {
    int id;
    char descricao[150];
    int concluida;  // 0 = não concluída, 1 = concluída
    int territoriosNecessarios;
    int tropasMinimas;
} Missao;

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

// Nível Master: sistema de cartas e missões
void inicializarCartas(Carta cartas[]);
void inicializarMissoes(Missao missoes[]);
void exibirCartas(const Carta cartas[]);
void exibirMissoes(const Missao missoes[]);
int usarCarta(Carta cartas[], Territorio* territorios, int numTerritorios);
void verificarMissoes(Missao missoes[], const Territorio territorios[], int numTerritorios);
void nivelMaster(void);

// Função para verificar fim de jogo
int verificarFimDeJogo(const Territorio territorios[], int numTerritorios);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1) Setup básico
    setlocale(LC_ALL, "Portuguese_Brazil.1252"); // Configurar para português brasileiro
    srand((unsigned int)time(NULL)); // Inicializar gerador de números aleatórios

    // Menu de seleção de nível
    int opcao;
    printf("\n================ JOGO WAR - SELECAO DE NIVEL ================\n");
    printf("Escolha o nivel de dificuldade:\n");
    printf("1 - Nivel Aventureiro (Basico)\n");
    printf("2 - Nivel Master (Cartas e Missoes)\n");
    printf("=========================================================\n");
    printf(">> Digite sua opcao (1 ou 2): ");
    scanf("%d", &opcao);
    limparBufferEntrada();

    if (opcao == 2) {
        nivelMaster();
        return 0;
    }

    // Nível Aventureiro (código original)
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
    
    // Sistema de combate melhorado - múltiplos dados baseado nas tropas
    int dadosAtacante = (atacante->tropas >= 4) ? 3 : (atacante->tropas >= 3) ? 2 : 1;
    int dadosDefensor = (defensor->tropas >= 2) ? 2 : 1;
    
    // Arrays para armazenar os resultados dos dados
    int resultadosAtacante[3] = {0, 0, 0};
    int resultadosDefensor[2] = {0, 0};
    
    // Rolar dados do atacante
    for (int i = 0; i < dadosAtacante; i++) {
        resultadosAtacante[i] = (rand() % 6) + 1;
    }
    
    // Rolar dados do defensor
    for (int i = 0; i < dadosDefensor; i++) {
        resultadosDefensor[i] = (rand() % 6) + 1;
    }
    
    // Ordenar dados em ordem decrescente (bubble sort simples)
    for (int i = 0; i < dadosAtacante - 1; i++) {
        for (int j = 0; j < dadosAtacante - 1 - i; j++) {
            if (resultadosAtacante[j] < resultadosAtacante[j + 1]) {
                int temp = resultadosAtacante[j];
                resultadosAtacante[j] = resultadosAtacante[j + 1];
                resultadosAtacante[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < dadosDefensor - 1; i++) {
        for (int j = 0; j < dadosDefensor - 1 - i; j++) {
            if (resultadosDefensor[j] < resultadosDefensor[j + 1]) {
                int temp = resultadosDefensor[j];
                resultadosDefensor[j] = resultadosDefensor[j + 1];
                resultadosDefensor[j + 1] = temp;
            }
        }
    }
    
    printf("\n[*] COMBATE EM ANDAMENTO!\n");
    printf("---------------------------------------------------------------------\n");
    printf("[ATK] ATACANTE: %s (Exercito %s)\n", atacante->nome, atacante->corExercito);
    printf("      Tropas disponiveis: %d | Dados (%d): ", atacante->tropas, dadosAtacante);
    for (int i = 0; i < dadosAtacante; i++) {
        printf("%d ", resultadosAtacante[i]);
    }
    printf("\n");
    
    printf("\n[DEF] DEFENSOR: %s (Exercito %s)\n", defensor->nome, defensor->corExercito);
    printf("      Tropas disponiveis: %d | Dados (%d): ", defensor->tropas, dadosDefensor);
    for (int i = 0; i < dadosDefensor; i++) {
        printf("%d ", resultadosDefensor[i]);
    }
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    
    // Comparar dados (maior dado vs maior dado, segundo maior vs segundo maior)
    int perdas_atacante = 0;
    int perdas_defensor = 0;
    int comparacoes = (dadosAtacante < dadosDefensor) ? dadosAtacante : dadosDefensor;
    
    for (int i = 0; i < comparacoes; i++) {
        printf("[DUELO %d] Atacante: %d vs Defensor: %d -> ", i + 1, resultadosAtacante[i], resultadosDefensor[i]);
        if (resultadosAtacante[i] > resultadosDefensor[i]) {
            perdas_defensor++;
            printf("Atacante vence!\n");
        } else {
            perdas_atacante++;
            printf("Defensor vence!\n");
        }
    }
    
    // Aplicar perdas
    atacante->tropas -= perdas_atacante;
    defensor->tropas -= perdas_defensor;
    
    printf("\n[RESULTADO] Perdas: Atacante -%d, Defensor -%d\n", perdas_atacante, perdas_defensor);
    
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
    } else if (atacante->tropas <= 1) {
        printf("      Atacante nao tem mais tropas suficientes para continuar.\n");
        printf("      %s mantem o controle com %d tropa(s).\n", defensor->nome, defensor->tropas);
        return 0; // Ataque falhou
    } else {
        if (perdas_defensor > perdas_atacante) {
            printf("[WIN] ATACANTE TEVE VANTAGEM!\n");
            printf("      %s ainda pode continuar atacando com %d tropa(s).\n", atacante->nome, atacante->tropas);
            printf("      %s resiste com %d tropa(s).\n", defensor->nome, defensor->tropas);
            return 1; // Ataque bem-sucedido
        } else {
            printf("[DEF] DEFESA BEM-SUCEDIDA!\n");
            printf("      %s ainda pode continuar atacando com %d tropa(s).\n", atacante->nome, atacante->tropas);
            printf("      %s mantem o controle com %d tropa(s).\n", defensor->nome, defensor->tropas);
            return 0; // Ataque falhou
        }
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
        // Verificar se o jogo terminou (todos territórios do mesmo exército)
        if (verificarFimDeJogo(territorios, numTerritorios)) {
            printf("\n[***] FIM DE JOGO! [***]\n");
            printf("Todos os territorios pertencem ao mesmo exercito!\n");
            printf("O jogo foi concluido automaticamente.\n");
            break;
        }
        
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

// ============================================================================
// FUNCOES DO NIVEL MASTER - SISTEMA DE CARTAS E MISSOES
// ============================================================================

void inicializarCartas(Carta cartas[]) {
    // Carta 1: Reforço
    cartas[0].tipo = CARTA_REFORCO;
    strcpy(cartas[0].nome, "Reforco");
    strcpy(cartas[0].descricao, "Adiciona +2 tropas em um territorio escolhido");
    cartas[0].usada = 0;
    
    // Carta 2: Ataque Duplo
    cartas[1].tipo = CARTA_ATAQUE_DUPLO;
    strcpy(cartas[1].nome, "Ataque Duplo");
    strcpy(cartas[1].descricao, "Permite realizar dois ataques consecutivos");
    cartas[1].usada = 0;
    
    // Carta 3: Defesa Extra
    cartas[2].tipo = CARTA_DEFESA_EXTRA;
    strcpy(cartas[2].nome, "Defesa Extra");
    strcpy(cartas[2].descricao, "Adiciona +1 dado de defesa no proximo ataque");
    cartas[2].usada = 0;
    
    // Carta 4: Conquista Rápida
    cartas[3].tipo = CARTA_CONQUISTA_RAPIDA;
    strcpy(cartas[3].nome, "Conquista Rapida");
    strcpy(cartas[3].descricao, "Conquista automaticamente territorio com 1 tropa");
    cartas[3].usada = 0;
    
    // Carta 5: Sabotagem
    cartas[4].tipo = CARTA_SABOTAGEM;
    strcpy(cartas[4].nome, "Sabotagem");
    strcpy(cartas[4].descricao, "Remove 1 tropa de um territorio inimigo");
    cartas[4].usada = 0;
    
    // Carta 6: Aliança
    cartas[5].tipo = CARTA_ALIANCA;
    strcpy(cartas[5].nome, "Alianca");
    strcpy(cartas[5].descricao, "Protege um territorio de ataques por 1 turno");
    cartas[5].usada = 0;
    
    // Carta 7: Espionagem
    cartas[6].tipo = CARTA_ESPIONAGEM;
    strcpy(cartas[6].nome, "Espionagem");
    strcpy(cartas[6].descricao, "Revela o numero de tropas de todos os territorios");
    cartas[6].usada = 0;
    
    // Carta 8: Bombardeio
    cartas[7].tipo = CARTA_BOMBARDEIO;
    strcpy(cartas[7].nome, "Bombardeio");
    strcpy(cartas[7].descricao, "Ataque a distancia sem perder tropas");
    cartas[7].usada = 0;
}

void inicializarMissoes(Missao missoes[]) {
    // Missão 1: Dominar 3 territórios
    missoes[0].id = 1;
    strcpy(missoes[0].descricao, "Dominar pelo menos 3 territorios");
    missoes[0].concluida = 0;
    missoes[0].territoriosNecessarios = 3;
    missoes[0].tropasMinimas = 0;
    
    // Missão 2: Acumular 15 tropas
    missoes[1].id = 2;
    strcpy(missoes[1].descricao, "Acumular pelo menos 15 tropas no total");
    missoes[1].concluida = 0;
    missoes[1].territoriosNecessarios = 0;
    missoes[1].tropasMinimas = 15;
    
    // Missão 3: Território com 8 tropas
    missoes[2].id = 3;
    strcpy(missoes[2].descricao, "Ter um territorio com pelo menos 8 tropas");
    missoes[2].concluida = 0;
    missoes[2].territoriosNecessarios = 0;
    missoes[2].tropasMinimas = 8;
    
    // Missão 4: Usar 3 cartas
    missoes[3].id = 4;
    strcpy(missoes[3].descricao, "Usar pelo menos 3 cartas especiais");
    missoes[3].concluida = 0;
    missoes[3].territoriosNecessarios = 0;
    missoes[3].tropasMinimas = 0;
    
    // Missão 5: Conquistar território protegido
    missoes[4].id = 5;
    strcpy(missoes[4].descricao, "Conquistar um territorio que estava protegido");
    missoes[4].concluida = 0;
    missoes[4].territoriosNecessarios = 0;
    missoes[4].tropasMinimas = 0;
}

void exibirCartas(const Carta cartas[]) {
    printf("\n================ CARTAS ESPECIAIS DISPONIVEIS ================\n");
    for (int i = 0; i < NUM_CARTAS; i++) {
        if (!cartas[i].usada) {
            printf("[%d] %s - %s\n", i + 1, cartas[i].nome, cartas[i].descricao);
        }
    }
    printf("===============================================================\n");
}

void exibirMissoes(const Missao missoes[]) {
    printf("\n================ MISSOES ESPECIAIS ================\n");
    for (int i = 0; i < NUM_MISSOES; i++) {
        char status[20];
        strcpy(status, missoes[i].concluida ? "[CONCLUIDA]" : "[PENDENTE]");
        printf("%s Missao %d: %s\n", status, missoes[i].id, missoes[i].descricao);
    }
    printf("===================================================\n");
}

int usarCarta(Carta cartas[], Territorio* territorios, int numTerritorios) {
    exibirCartas(cartas);
    
    int opcao;
    printf(">> Escolha uma carta para usar (0 para pular): ");
    scanf("%d", &opcao);
    limparBufferEntrada();
    
    if (opcao == 0) return 0;
    
    if (opcao < 1 || opcao > NUM_CARTAS || cartas[opcao - 1].usada) {
        printf("[ERRO] Carta invalida ou ja usada!\n");
        return 0;
    }
    
    Carta* carta = &cartas[opcao - 1];
    carta->usada = 1;
    
    printf("\n[CARTA USADA] %s ativada!\n", carta->nome);
    
    switch (carta->tipo) {
        case CARTA_REFORCO: {
            printf(">> Escolha um territorio para reforcar (1-%d): ", numTerritorios);
            int idx;
            scanf("%d", &idx);
            limparBufferEntrada();
            if (idx >= 1 && idx <= numTerritorios) {
                territorios[idx - 1].tropas += 2;
                printf("[REFORCO] +2 tropas adicionadas ao territorio %s!\n", territorios[idx - 1].nome);
            }
            break;
        }
        case CARTA_SABOTAGEM: {
            printf(">> Escolha um territorio inimigo para sabotar (1-%d): ", numTerritorios);
            int idx;
            scanf("%d", &idx);
            limparBufferEntrada();
            if (idx >= 1 && idx <= numTerritorios && territorios[idx - 1].tropas > 1) {
                territorios[idx - 1].tropas--;
                printf("[SABOTAGEM] -1 tropa removida do territorio %s!\n", territorios[idx - 1].nome);
            }
            break;
        }
        case CARTA_ALIANCA: {
            printf(">> Escolha um territorio para proteger (1-%d): ", numTerritorios);
            int idx;
            scanf("%d", &idx);
            limparBufferEntrada();
            if (idx >= 1 && idx <= numTerritorios) {
                territorios[idx - 1].protegido = 1;
                printf("[ALIANCA] Territorio %s protegido por 1 turno!\n", territorios[idx - 1].nome);
            }
            break;
        }
        case CARTA_ESPIONAGEM: {
            printf("\n[ESPIONAGEM] Revelando informacoes dos territorios:\n");
            exibirMapa(territorios, numTerritorios);
            break;
        }
        default:
            printf("[INFO] Efeito da carta sera aplicado automaticamente.\n");
            break;
    }
    
    return 1;
}

void verificarMissoes(Missao missoes[], const Territorio territorios[], int numTerritorios) {
    // Missão 1: Dominar 3 territórios (assumindo que territórios "verdes" são do jogador)
    int territoriosJogador = 0;
    int totalTropas = 0;
    int maxTropasTerratorio = 0;
    
    for (int i = 0; i < numTerritorios; i++) {
        if (strcmp(territorios[i].corExercito, "verde") == 0) {
            territoriosJogador++;
        }
        totalTropas += territorios[i].tropas;
        if (territorios[i].tropas > maxTropasTerratorio) {
            maxTropasTerratorio = territorios[i].tropas;
        }
    }
    
    // Verificar missão 1
    if (!missoes[0].concluida && territoriosJogador >= 3) {
        missoes[0].concluida = 1;
        printf("\n[MISSAO CONCLUIDA] %s\n", missoes[0].descricao);
    }
    
    // Verificar missão 2
    if (!missoes[1].concluida && totalTropas >= 15) {
        missoes[1].concluida = 1;
        printf("\n[MISSAO CONCLUIDA] %s\n", missoes[1].descricao);
    }
    
    // Verificar missão 3
    if (!missoes[2].concluida && maxTropasTerratorio >= 8) {
        missoes[2].concluida = 1;
        printf("\n[MISSAO CONCLUIDA] %s\n", missoes[2].descricao);
    }
}

void nivelMaster(void) {
    printf("\n================ JOGO WAR - NIVEL MASTER ================\n");
    printf("Bem-vindo ao nivel Master com cartas especiais e missoes!\n");
    printf("=========================================================\n");
    
    // Inicializar sistemas
    Carta cartas[NUM_CARTAS];
    Missao missoes[NUM_MISSOES];
    inicializarCartas(cartas);
    inicializarMissoes(missoes);
    
    // Alocação dinâmica de memória para territórios
    Territorio* territorios = alocarMapa(NUM_TERRITORIOS);
    
    // Inicializar campo protegido
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        territorios[i].protegido = 0;
    }
    
    // Cadastro dos territórios
    cadastrarTerritorios(territorios, NUM_TERRITORIOS);
    
    // Exibir missões iniciais
    exibirMissoes(missoes);
    
    // Loop principal do jogo Master
    int turno = 1;
    int cartasUsadas = 0;
    
    while (turno <= 5) {  // Máximo 5 turnos
        printf("\n================ TURNO %d - NIVEL MASTER ================\n", turno);
        
        // Exibir mapa atual
        exibirMapa(territorios, NUM_TERRITORIOS);
        
        // Fase de cartas
        printf("\n[FASE DE CARTAS]\n");
        if (usarCarta(cartas, territorios, NUM_TERRITORIOS)) {
            cartasUsadas++;
        }
        
        // Verificar missão de usar cartas
        if (!missoes[3].concluida && cartasUsadas >= 3) {
            missoes[3].concluida = 1;
            printf("\n[MISSAO CONCLUIDA] %s\n", missoes[3].descricao);
        }
        
        // Fase de ataques (simplificada)
        printf("\n[FASE DE ATAQUES]\n");
        faseDeAtaque(territorios, NUM_TERRITORIOS);
        
        // Remover proteções
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            territorios[i].protegido = 0;
        }
        
        // Verificar missões
        verificarMissoes(missoes, territorios, NUM_TERRITORIOS);
        
        // Exibir status das missões
        exibirMissoes(missoes);
        
        turno++;
        
        printf("\n[PAUSE] Pressione Enter para continuar para o proximo turno...");
        getchar();
    }
    
    // Resultado final
    printf("\n================ RESULTADO FINAL - NIVEL MASTER ================\n");
    exibirMapa(territorios, NUM_TERRITORIOS);
    
    int missoesConcluidas = 0;
    for (int i = 0; i < NUM_MISSOES; i++) {
        if (missoes[i].concluida) missoesConcluidas++;
    }
    
    printf("\n[ESTATISTICAS FINAIS]\n");
    printf("- Missoes concluidas: %d/%d\n", missoesConcluidas, NUM_MISSOES);
    printf("- Cartas usadas: %d/%d\n", cartasUsadas, NUM_CARTAS);
    printf("- Turnos jogados: %d\n", turno - 1);
    
    if (missoesConcluidas >= 3) {
        printf("\n[*] PARABENS! Voce dominou o nivel Master! [*]\n");
        printf("[*] Voce e um verdadeiro estrategista de guerra! [*]\n");
    } else {
        printf("\n[*] Bom jogo! Continue praticando para dominar o nivel Master! [*]\n");
    }
    
    // Liberar memória
    liberarMemoria(territorios);
}

// ============================================================================
// FUNÇÃO PARA VERIFICAR FIM DE JOGO
// ============================================================================

int verificarFimDeJogo(const Territorio territorios[], int numTerritorios) {
    if (numTerritorios <= 0) return 0;
    
    // Pegar a cor do primeiro território como referência
    const char* corReferencia = territorios[0].corExercito;
    
    // Verificar se todos os territórios têm a mesma cor
    for (int i = 1; i < numTerritorios; i++) {
        if (strcmp(territorios[i].corExercito, corReferencia) != 0) {
            return 0; // Encontrou território de cor diferente, jogo continua
        }
    }
    
    // Todos os territórios têm a mesma cor
    printf("\n[VITORIA] O exercito %s conquistou todos os territorios!\n", corReferencia);
    return 1; // Fim de jogo
}
