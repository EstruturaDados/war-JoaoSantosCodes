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
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número máximo de territórios e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TERRITORIOS 20          // Máximo de territórios permitidos no jogo
#define MIN_TERRITORIOS 3           // Mínimo de territórios para um jogo válido
#define TAM_MAX_NOME 50             // Tamanho máximo para nomes de territórios
#define TAM_MAX_COR 20              // Tamanho máximo para nomes de cores
#define NUM_TERRITORIOS_PREDEFINIDOS 5  // Número de territórios pré-cadastrados
#define NUM_CORES_PREDEFINIDAS 5    // Número de cores disponíveis para exércitos
#define NUM_OPCOES_TROPAS 5         // Número de opções de tropas iniciais

// --- Códigos de Cores ANSI para Terminal ---
// Cores para melhorar a experiência visual do jogo
#define COR_RESET "\033[0m"             // Reset para cor padrão
#define COR_VERMELHO "\033[31m"         // Vermelho para perdas/erros
#define COR_VERDE "\033[32m"            // Verde para ganhos/sucessos
#define COR_AMARELO "\033[33m"          // Amarelo para informações importantes
#define COR_AZUL "\033[34m"             // Azul para atacante
#define COR_MAGENTA "\033[35m"          // Magenta para defensor
#define COR_CIANO "\033[36m"            // Ciano para informações neutras
#define COR_BRANCO "\033[37m"           // Branco para texto destacado
#define COR_NEGRITO "\033[1m"           // Negrito
#define COR_VERMELHO_NEGRITO "\033[1;31m"   // Vermelho negrito para erros críticos
#define COR_VERDE_NEGRITO "\033[1;32m"      // Verde negrito para vitórias
#define COR_AMARELO_NEGRITO "\033[1;33m"    // Amarelo negrito para títulos
#define COR_CIANO_NEGRITO "\033[1;36m"      // Ciano negrito para fases do jogo

// --- Estruturas ---
/**
 * @brief Estrutura que representa um território no jogo WAR
 * 
 * Esta estrutura contém todas as informações necessárias para um território:
 * - nome: Nome do território (ex: "Brasil", "Argentina")
 * - corExercito: Cor do exército que controla o território
 * - tropas: Número de tropas estacionadas no território
 */
typedef struct {
    char nome[TAM_MAX_NOME];        // Nome do território
    char corExercito[TAM_MAX_COR];  // Cor do exército controlador
    int tropas;                     // Número de tropas no território
} Territorio;

// --- Sistema de Missões ---
/**
 * @brief Enumeração dos tipos de missões disponíveis no jogo
 * 
 * Define os diferentes objetivos que um jogador pode receber:
 * - CONQUISTAR_CONTINENTE: Dominar todos os territórios de uma cor específica
 * - ELIMINAR_COR: Eliminar completamente uma cor específica do mapa
 * - CONTROLAR_TERRITORIOS: Controlar um número específico de territórios
 * - SOBREVIVER_TURNOS: Sobreviver por um número específico de turnos
 */
typedef enum {
    CONQUISTAR_CONTINENTE,      // Dominar todos os territórios de uma cor
    ELIMINAR_COR,              // Eliminar uma cor específica
    CONTROLAR_TERRITORIOS,     // Controlar X territórios
    SOBREVIVER_TURNOS          // Sobreviver X turnos
} TipoMissao;

/**
 * @brief Estrutura que representa uma missão no jogo
 * 
 * Contém todas as informações necessárias para uma missão:
 * - tipo: Tipo da missão (enum TipoMissao)
 * - descricao: Descrição textual da missão
 * - corAlvo: Cor alvo (para missões que envolvem cores específicas)
 * - valorAlvo: Valor numérico alvo (territórios ou turnos)
 * - cumprida: Status de cumprimento da missão
 */
typedef struct {
    TipoMissao tipo;                    // Tipo da missão
    char descricao[100];                // Descrição da missão
    char corAlvo[TAM_MAX_COR];         // Cor alvo (se aplicável)
    int valorAlvo;                      // Valor alvo (territórios/turnos)
    int cumprida;                       // 0 = não cumprida, 1 = cumprida
} Missao;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// === FUNÇÕES UTILITÁRIAS ===
/**
 * @brief Limpa o buffer de entrada para evitar problemas com scanf
 */
void limparBufferEntrada(void);

/**
 * @brief Lê uma string de forma segura, removendo quebras de linha
 * @param destino Ponteiro para onde armazenar a string
 * @param tamanhoMax Tamanho máximo da string
 */
void lerString(char* destino, int tamanhoMax);

// === FUNÇÕES DE CADASTRO E EXIBIÇÃO ===
/**
 * @brief Cadastra territórios com seleção dinâmica e validação de duplicatas
 * @param territorios Array de territórios a ser preenchido
 * @param tamanho Número de territórios a cadastrar
 */
void cadastrarTerritorios(Territorio territorios[], int tamanho);

/**
 * @brief Exibe o mapa atual com todos os territórios e suas informações
 * @param territorios Array de territórios (somente leitura)
 * @param tamanho Número de territórios no array
 */
void exibirMapa(const Territorio territorios[], int tamanho);

// === FUNÇÕES DE SELEÇÃO DINÂMICA ===
/**
 * @brief Permite seleção de território com validação de duplicatas
 * @param territoriosUsados Array com índices dos territórios já utilizados
 * @param numUsados Número de territórios já utilizados
 * @return Índice do território selecionado
 */
int selecionarTerritorio(int territoriosUsados[], int numUsados);

/**
 * @brief Permite seleção de cor com validação de duplicatas
 * @param corEscolhida String onde será armazenada a cor escolhida
 * @param coresUsadas Array com índices das cores já utilizadas
 * @param numUsadas Número de cores já utilizadas
 * @return Índice da cor selecionada
 */
int selecionarCor(char* corEscolhida, int coresUsadas[], int numUsadas);

/**
 * @brief Permite seleção do número inicial de tropas
 * @return Número de tropas selecionado
 */
int selecionarTropas(void);

// === FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA ===
/**
 * @brief Aloca memória dinamicamente para o mapa de territórios
 * @param numTerritorios Número de territórios a alocar
 * @return Ponteiro para o array de territórios alocado
 */
Territorio* alocarMapa(int numTerritorios);

/**
 * @brief Libera a memória alocada para os territórios
 * @param territorios Ponteiro para o array a ser liberado
 */
void liberarMemoria(Territorio* territorios);

// === FUNÇÕES DE COMBATE ===
/**
 * @brief Simula um ataque entre dois territórios com sistema de dados
 * @param atacante Ponteiro para o território atacante (modificável)
 * @param defensor Ponteiro para o território defensor (modificável)
 * @return 0=ataque falhou, 1=ataque bem-sucedido, 2=território conquistado
 */
int simularAtaque(Territorio* atacante, Territorio* defensor);

/**
 * @brief Gerencia a fase de ataques do jogo
 * @param territorios Array de territórios (modificável)
 * @param numTerritorios Número de territórios no jogo
 */
void faseDeAtaque(Territorio* territorios, int numTerritorios);

// === FUNÇÕES DO NÍVEL MASTER ===
/**
 * @brief Implementa o Nível Master com territórios dinâmicos e sistema de reforços
 */
void nivelMaster(void);

/**
 * @brief Implementa a fase de reforços baseada no controle territorial
 * @param territorios Array de territórios (modificável)
 * @param numTerritorios Número de territórios no jogo
 */
void faseDeReforcos(Territorio* territorios, int numTerritorios);

/**
 * @brief Solicita ao usuário o número de territórios para o jogo
 * @return Número de territórios selecionado (entre MIN_TERRITORIOS e MAX_TERRITORIOS)
 */
int selecionarNumeroTerritorios(void);

// === FUNÇÕES DE CONTROLE DE JOGO ===
/**
 * @brief Verifica se o jogo terminou (todos territórios da mesma cor)
 * @param territorios Array de territórios (somente leitura)
 * @param numTerritorios Número de territórios no jogo
 * @return 1 se o jogo terminou, 0 caso contrário
 */
int verificarFimDeJogo(const Territorio territorios[], int numTerritorios);

// === FUNÇÕES DO SISTEMA DE MISSÕES ===
/**
 * @brief Gera uma missão aleatória para o jogador
 * @param missao Ponteiro para a estrutura Missao a ser preenchida
 * @param territorios Array de territórios (para análise das cores disponíveis)
 * @param numTerritorios Número de territórios no jogo
 */
void gerarMissaoAleatoria(Missao* missao, const Territorio territorios[], int numTerritorios);

/**
 * @brief Verifica se a missão atual foi cumprida
 * @param missao Ponteiro para a missão a ser verificada (somente leitura)
 * @param territorios Array de territórios (somente leitura)
 * @param numTerritorios Número de territórios no jogo
 * @param turnoAtual Turno atual do jogo
 * @return 1 se a missão foi cumprida, 0 caso contrário
 */
int verificarMissao(const Missao* missao, const Territorio territorios[], int numTerritorios, int turnoAtual);

/**
 * @brief Exibe a missão atual do jogador
 * @param missao Ponteiro para a missão a ser exibida (somente leitura)
 */
void exibirMissao(const Missao* missao);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1) Setup básico
    setlocale(LC_ALL, "Portuguese_Brazil.1252"); // Configurar para português brasileiro
    srand((unsigned int)time(NULL)); // Inicializar gerador de números aleatórios

    // Menu de seleção de nível
    int opcao;
    printf("\n%s================ JOGO WAR - SELECAO DE NIVEL ================%s\n", COR_AMARELO_NEGRITO, COR_RESET);
    printf("Escolha o nivel de dificuldade:\n");
    printf("%s1%s - %sNivel Aventureiro (Basico)%s\n", COR_VERDE_NEGRITO, COR_RESET, COR_VERDE, COR_RESET);
    printf("%s2%s - %sNivel Master (Cartas e Missoes)%s\n", COR_AZUL, COR_RESET, COR_AZUL, COR_RESET);
    printf("%s=========================================================%s\n", COR_AMARELO_NEGRITO, COR_RESET);
    printf(">> Digite sua opcao (1 ou 2): ");
    scanf("%d", &opcao);
    limparBufferEntrada();

    if (opcao == 2) {
        nivelMaster();
        return 0;
    }

    // Nível Aventureiro (código original)
    // 2) Alocação dinâmica de memória para territórios
    Territorio* territorios = alocarMapa(5);  // Nível básico usa 5 territórios fixos

    // 3) Cadastro dos territórios
    cadastrarTerritorios(territorios, 5);

    // 4) Exibir o mapa inicial
    printf("\n[*] INICIANDO JOGO WAR - NIVEL AVENTUREIRO [*]\n");
    exibirMapa(territorios, 5);

    // 5) Fase de ataques (Nível Aventureiro)
    faseDeAtaque(territorios, 5);

    // 6) Exibir o mapa final
    printf("\n[*] RESULTADO FINAL DA BATALHA [*]\n");
    exibirMapa(territorios, 5);

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
    // Lista de territórios predefinidos para referência
    const char* territoriosPredefinidos[NUM_TERRITORIOS_PREDEFINIDOS] = {
        "Brasil", "Argentina", "Chile", "Peru", "Colombia"
    };
    
    // Arrays para rastrear territórios e cores já utilizados
    int territoriosUsados[MAX_TERRITORIOS];
    int coresUsadas[MAX_TERRITORIOS];
    int numTerritoriosUsados = 0;
    int numCoresUsadas = 0;
    
    printf("\n================ CADASTRO DE TERRITORIOS ================\n");
    printf("Configure os %d territorios do mapa de guerra:\n", tamanho);
    printf("Agora com selecao dinamica e validacao de duplicatas!\n");
    printf("=========================================================\n");

    for (int i = 0; i < tamanho; i++) {
        printf("\n[*] TERRITORIO %d/%d\n", i + 1, tamanho);
        printf("---------------------------------------------------------\n");

        // Seleção dinâmica de território com validação de duplicatas
        int indiceTerritorioEscolhido = selecionarTerritorio(territoriosUsados, numTerritoriosUsados);
        strcpy(territorios[i].nome, territoriosPredefinidos[indiceTerritorioEscolhido]);
        territoriosUsados[numTerritoriosUsados++] = indiceTerritorioEscolhido;

        // Seleção dinâmica de cor com validação de duplicatas
        int indiceCorEscolhida = selecionarCor(territorios[i].corExercito, coresUsadas, numCoresUsadas);
        coresUsadas[numCoresUsadas++] = indiceCorEscolhida;

        // Seleção dinâmica de tropas
        territorios[i].tropas = selecionarTropas();
        
        printf("\n[OK] Territorio '%s' cadastrado com sucesso!\n", territorios[i].nome);
        printf("     Exercito: %s | Tropas: %d\n", territorios[i].corExercito, territorios[i].tropas);
    }
    
    // DISTRIBUIÇÃO ADICIONAL DE TROPAS PARA TORNAR O JOGO JOGÁVEL
    printf("\n================ DISTRIBUICAO ADICIONAL DE TROPAS ================\n");
    printf("Adicionando tropas extras para garantir combates interessantes...\n");
    
    srand(time(NULL));
    int tropasExtras = 10; // Total de tropas extras para distribuir
    
    for (int i = 0; i < tropasExtras; i++) {
        int territorioAleatorio = rand() % tamanho;
        territorios[territorioAleatorio].tropas++;
        printf("[+] %s recebeu +1 tropa (Total: %d)\n", 
               territorios[territorioAleatorio].nome, 
               territorios[territorioAleatorio].tropas);
    }
    
    printf("\n[*] Cadastro concluido! Todos os territorios foram configurados.\n");
    printf("[*] Sistema de validacao garante territorios e cores unicos!\n");
    printf("[*] Tropas extras distribuidas para garantir combates emocionantes!\n");
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
    // Usar calloc para inicialização automática com zeros
    Territorio* territorios = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
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
        printf("%s[!] ERRO: O atacante precisa ter pelo menos 2 tropas para atacar.%s\n", COR_VERMELHO_NEGRITO, COR_RESET);
        return 0; // Ataque falhou
    }
    
    if (defensor->tropas <= 0) {
        printf("%s[!] ERRO: O defensor nao possui tropas para defender.%s\n", COR_VERMELHO_NEGRITO, COR_RESET);
        return 0; // Ataque falhou
    }
    
    // Sistema de combate simplificado - máximo 2 dados por jogador
    int dadosAtacante = (atacante->tropas >= 3) ? 2 : 1;
    int dadosDefensor = (defensor->tropas >= 2) ? 2 : 1;
    
    // Arrays para armazenar os resultados dos dados
    int resultadosAtacante[2] = {0, 0};
    int resultadosDefensor[2] = {0, 0};
    
    // Rolar dados do atacante
    for (int i = 0; i < dadosAtacante; i++) {
        resultadosAtacante[i] = (rand() % 6) + 1;
    }
    
    // Rolar dados do defensor
    for (int i = 0; i < dadosDefensor; i++) {
        resultadosDefensor[i] = (rand() % 6) + 1;
    }
    
    // Ordenar dados em ordem decrescente (maior primeiro)
    if (dadosAtacante == 2 && resultadosAtacante[0] < resultadosAtacante[1]) {
        int temp = resultadosAtacante[0];
        resultadosAtacante[0] = resultadosAtacante[1];
        resultadosAtacante[1] = temp;
    }
    
    if (dadosDefensor == 2 && resultadosDefensor[0] < resultadosDefensor[1]) {
        int temp = resultadosDefensor[0];
        resultadosDefensor[0] = resultadosDefensor[1];
        resultadosDefensor[1] = temp;
    }
    
    printf("\n%s[*] COMBATE EM ANDAMENTO!%s\n", COR_AMARELO_NEGRITO, COR_RESET);
    printf("---------------------------------------------------------------------\n");
    printf("%s[ATK] ATACANTE:%s %s (Exercito %s)\n", COR_AZUL, COR_RESET, atacante->nome, atacante->corExercito);
    printf("      Tropas disponiveis: %d | Dados (%d): ", atacante->tropas, dadosAtacante);
    for (int i = 0; i < dadosAtacante; i++) {
        printf("%s%d%s ", COR_AZUL, resultadosAtacante[i], COR_RESET);
    }
    printf("\n");
    
    printf("\n%s[DEF] DEFENSOR:%s %s (Exercito %s)\n", COR_MAGENTA, COR_RESET, defensor->nome, defensor->corExercito);
    printf("      Tropas disponiveis: %d | Dados (%d): ", defensor->tropas, dadosDefensor);
    for (int i = 0; i < dadosDefensor; i++) {
        printf("%s%d%s ", COR_MAGENTA, resultadosDefensor[i], COR_RESET);
    }
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    
    // Comparar dados (maior dado vs maior dado)
    int perdas_atacante = 0;
    int perdas_defensor = 0;
    
    // Primeira comparação (dados maiores)
    printf("[DUELO 1] Atacante: %s%d%s vs Defensor: %s%d%s -> ", 
           COR_AZUL, resultadosAtacante[0], COR_RESET, 
           COR_MAGENTA, resultadosDefensor[0], COR_RESET);
    if (resultadosAtacante[0] > resultadosDefensor[0]) {
        perdas_defensor++;
        printf("%sAtacante vence!%s\n", COR_VERDE_NEGRITO, COR_RESET);
    } else {
        perdas_atacante++;
        printf("%sDefensor vence!%s\n", COR_VERMELHO_NEGRITO, COR_RESET);
    }
    
    // Segunda comparação (se ambos têm 2 dados)
    if (dadosAtacante == 2 && dadosDefensor == 2) {
        printf("[DUELO 2] Atacante: %s%d%s vs Defensor: %s%d%s -> ", 
               COR_AZUL, resultadosAtacante[1], COR_RESET, 
               COR_MAGENTA, resultadosDefensor[1], COR_RESET);
        if (resultadosAtacante[1] > resultadosDefensor[1]) {
            perdas_defensor++;
            printf("%sAtacante vence!%s\n", COR_VERDE_NEGRITO, COR_RESET);
        } else {
            perdas_atacante++;
            printf("%sDefensor vence!%s\n", COR_VERMELHO_NEGRITO, COR_RESET);
        }
    }
    
    // Aplicar perdas
    atacante->tropas -= perdas_atacante;
    defensor->tropas -= perdas_defensor;
    
    printf("\n%s[RESULTADO]%s Perdas: Atacante %s-%d%s, Defensor %s-%d%s\n", 
           COR_AMARELO_NEGRITO, COR_RESET,
           COR_VERMELHO, perdas_atacante, COR_RESET,
           COR_VERMELHO, perdas_defensor, COR_RESET);
    
    if (defensor->tropas == 0) {
        // Território conquistado
        printf("\n%s[***] CONQUISTA TOTAL! [***]%s\n", COR_VERDE_NEGRITO, COR_RESET);
        printf("      %s%s conquistou completamente %s!%s\n", COR_VERDE, atacante->nome, defensor->nome, COR_RESET);
        printf("      Transferindo controle do territorio...\n");
        
        strcpy(defensor->corExercito, atacante->corExercito);
        defensor->tropas = atacante->tropas - 1;
        atacante->tropas = 1;
        
        printf("      %s%s agora pertence ao exercito %s com %d tropas!%s\n", 
               COR_VERDE, defensor->nome, defensor->corExercito, defensor->tropas, COR_RESET);
        return 2; // Território conquistado
    } else if (atacante->tropas <= 1) {
        printf("      %sAtacante nao tem mais tropas suficientes para continuar.%s\n", COR_VERMELHO, COR_RESET);
        printf("      %s%s mantem o controle com %d tropa(s).%s\n", COR_CIANO, defensor->nome, defensor->tropas, COR_RESET);
        return 0; // Ataque falhou
    } else {
        printf("      %s%s ainda pode continuar atacando com %d tropa(s).%s\n", COR_AZUL, atacante->nome, atacante->tropas, COR_RESET);
        printf("      %s%s mantem o controle com %d tropa(s).%s\n", COR_MAGENTA, defensor->nome, defensor->tropas, COR_RESET);
        return 1; // Ataque bem-sucedido
    }
}

void faseDeAtaque(Territorio* territorios, int numTerritorios) {
    printf("\n[*] FASE DE COMBATE [*]\n");
    printf("=======================================================================\n");
    printf("[*] INSTRUCOES:\n");
    printf("   - Escolha um territorio atacante (deve ter pelo menos 2 tropas)\n");
    printf("   - Escolha um territorio defensor (cor diferente do atacante)\n");
    printf("   - Digite -1 para encerrar a fase de ataques\n");
    printf("=======================================================================\n");
    
    int atacanteIdx, defensorIdx;
    
    while (1) {
        // Verificar se o jogo terminou
        if (verificarFimDeJogo(territorios, numTerritorios)) {
            printf("\n[***] FIM DE JOGO! [***]\n");
            printf("Todos os territorios pertencem ao mesmo exercito!\n");
            break;
        }
        
        printf("\n[INFO] SITUACAO ATUAL DO CAMPO DE BATALHA:\n");
        exibirMapa(territorios, numTerritorios);
        
        printf("\n[ATK] Escolha o territorio ATACANTE (1-%d) ou -1 para parar: ", numTerritorios);
        if (scanf("%d", &atacanteIdx) != 1) {
            limparBufferEntrada();
            printf("[!] Entrada invalida!\n");
            continue;
        }
        limparBufferEntrada();
        
        if (atacanteIdx == -1) {
            printf("\n[END] Fase de ataques encerrada.\n");
            break;
        }
        
        if (atacanteIdx < 1 || atacanteIdx > numTerritorios) {
            printf("[!] Territorio invalido!\n");
            continue;
        }
        
        if (territorios[atacanteIdx - 1].tropas <= 1) {
            printf("[!] O territorio '%s' nao tem tropas suficientes para atacar!\n", territorios[atacanteIdx - 1].nome);
            continue;
        }
        
        printf("[DEF] Escolha o territorio DEFENSOR (1-%d): ", numTerritorios);
        if (scanf("%d", &defensorIdx) != 1) {
            limparBufferEntrada();
            printf("[!] Entrada invalida!\n");
            continue;
        }
        limparBufferEntrada();
        
        if (defensorIdx < 1 || defensorIdx > numTerritorios) {
            printf("[!] Territorio invalido!\n");
            continue;
        }
        
        if (atacanteIdx == defensorIdx) {
            printf("[!] Um territorio nao pode atacar a si mesmo!\n");
            continue;
        }
        
        if (strcmp(territorios[atacanteIdx - 1].corExercito, territorios[defensorIdx - 1].corExercito) == 0) {
            printf("[!] Territorios aliados nao podem se atacar!\n");
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

void nivelMaster(void) {
    printf("\n================ JOGO WAR - NIVEL MASTER ================\n");
    printf("Bem-vindo ao nivel Master com sistema de missoes!\n");
    printf("=========================================================\n");
    
    // Solicitar número de territórios
    int numTerritorios = selecionarNumeroTerritorios();
    
    // Alocação dinâmica de memória para territórios
    Territorio* territorios = alocarMapa(numTerritorios);
    
    // Cadastro dos territórios
    cadastrarTerritorios(territorios, numTerritorios);
    
    // Gerar missão aleatória para o jogador
    Missao missaoJogador;
    gerarMissaoAleatoria(&missaoJogador, territorios, numTerritorios);
    
    printf("\n" COR_AMARELO_NEGRITO "================ SUA MISSAO ================\n" COR_RESET);
    exibirMissao(&missaoJogador);
    printf(COR_AMARELO_NEGRITO "============================================\n" COR_RESET);
    
    printf("\n[PAUSE] Pressione Enter para iniciar o jogo...");
    getchar();
    
    // Loop principal do jogo Master com sistema de missões
    int turno = 1;
    
    while (turno <= 15) {  // Máximo 15 turnos (ajustado para mais territórios)
        printf("\n================ TURNO %d - NIVEL MASTER ================\n", turno);
        
        // Exibir mapa atual
        exibirMapa(territorios, numTerritorios);
        
        // Exibir missão atual
        printf("\n" COR_CIANO "--- SUA MISSAO ATUAL ---\n" COR_RESET);
        exibirMissao(&missaoJogador);
        
        // Verificar se a missão foi cumprida
        if (verificarMissao(&missaoJogador, territorios, numTerritorios, turno)) {
            printf("\n" COR_VERDE_NEGRITO "[***] PARABENS! MISSAO CUMPRIDA! [***]\n" COR_RESET);
            printf(COR_VERDE "Voce venceu o jogo completando sua missao!\n" COR_RESET);
            break;
        }
        
        // Verificar se o jogo terminou por dominação total
        if (verificarFimDeJogo(territorios, numTerritorios)) {
            printf("\n[***] FIM DE JOGO POR DOMINACAO TOTAL! [***]\n");
            printf("Todos os territorios pertencem ao mesmo exercito!\n");
            break;
        }
        
        // Fase de reforços (a partir do turno 2)
        if (turno > 1) {
            faseDeReforcos(territorios, numTerritorios);
        }
        
        // Fase de ataques
        printf("\n[FASE DE ATAQUES]\n");
        faseDeAtaque(territorios, numTerritorios);
        
        turno++;
        
        printf("\n[PAUSE] Pressione Enter para continuar para o proximo turno...");
        getchar();
    }
    
    // Resultado final
    printf("\n================ RESULTADO FINAL - NIVEL MASTER ================\n");
    exibirMapa(territorios, numTerritorios);
    
    printf("\n[ESTATISTICAS FINAIS]\n");
    printf("- Turnos jogados: %d\n", turno - 1);
    printf("- Territorios no jogo: %d\n", numTerritorios);
    
    if (verificarFimDeJogo(territorios, numTerritorios)) {
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
    // Validação de entrada
    if (numTerritorios <= 0 || territorios == NULL) {
        return 0;
    }
    
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

// ============================================================================
// FUNÇÃO: faseDeReforcos
// Implementa a fase de reforços onde cada exército recebe tropas adicionais
// baseado no número de territórios que controla
// ============================================================================
void faseDeReforcos(Territorio* territorios, int numTerritorios) {
    printf("\n%s================ FASE DE REFORCOS ================%s\n", COR_CIANO_NEGRITO, COR_RESET);
    
    // Contar territórios por cor de exército
    char cores[MAX_TERRITORIOS][TAM_MAX_COR];
    int contadores[MAX_TERRITORIOS] = {0};
    int numCores = 0;
    
    // Identificar cores únicas e contar territórios
    for (int i = 0; i < numTerritorios; i++) {
        int corEncontrada = 0;
        for (int j = 0; j < numCores; j++) {
            if (strcmp(territorios[i].corExercito, cores[j]) == 0) {
                contadores[j]++;
                corEncontrada = 1;
                break;
            }
        }
        if (!corEncontrada) {
            strcpy(cores[numCores], territorios[i].corExercito);
            contadores[numCores] = 1;
            numCores++;
        }
    }
    
    // Distribuir reforços para cada exército
    for (int i = 0; i < numCores; i++) {
        int reforcos = contadores[i]; // 1 reforço por território controlado
        if (reforcos < 2) reforcos = 2; // Mínimo de 2 reforços por turno
        
        printf("\n%s[REFORCO]%s Exercito %s%s%s controla %s%d%s territorio(s) -> %s+%d tropas%s de reforco\n", 
               COR_VERDE_NEGRITO, COR_RESET, COR_AMARELO, cores[i], COR_RESET, 
               COR_CIANO, contadores[i], COR_RESET, COR_VERDE, reforcos, COR_RESET);
        
        // Distribuir reforços entre os territórios do exército
        int reforcosPorTerritorio = reforcos / contadores[i];
        int reforcosSobrando = reforcos % contadores[i];
        
        for (int j = 0; j < numTerritorios; j++) {
            if (strcmp(territorios[j].corExercito, cores[i]) == 0) {
                int reforcoAtual = reforcosPorTerritorio;
                if (reforcosSobrando > 0) {
                    reforcoAtual++;
                    reforcosSobrando--;
                }
                territorios[j].tropas += reforcoAtual;
                printf("  %s+%s %s%s%s recebeu %s%d%s tropa(s) (total: %s%d%s)\n", 
                       COR_VERDE, COR_RESET, COR_BRANCO, territorios[j].nome, COR_RESET,
                       COR_VERDE, reforcoAtual, COR_RESET, COR_AMARELO, territorios[j].tropas, COR_RESET);
            }
        }
    }
    
    printf("%s==================================================%s\n", COR_CIANO_NEGRITO, COR_RESET);
}

// ============================================================================
// FUNÇÃO: selecionarNumeroTerritorios
// Solicita ao usuário o número de territórios para o jogo
// ============================================================================
int selecionarNumeroTerritorios(void) {
    int numTerritorios;
    
    printf("\n================ CONFIGURACAO DO JOGO ================\n");
    printf("Quantos territorios voce deseja no jogo?\n");
    printf("Minimo: %d territorios\n", MIN_TERRITORIOS);
    printf("Maximo: %d territorios\n", MAX_TERRITORIOS);
    printf("Recomendado: 5-10 territorios para melhor jogabilidade\n");
    printf("======================================================\n");
    
    do {
        printf(">> Digite o numero de territorios (%d-%d): ", MIN_TERRITORIOS, MAX_TERRITORIOS);
        
        if (scanf("%d", &numTerritorios) != 1) {
            printf("[ERRO] Entrada invalida! Digite apenas numeros.\n");
            limparBufferEntrada();
            continue;
        }
        
        limparBufferEntrada();
        
        if (numTerritorios < MIN_TERRITORIOS || numTerritorios > MAX_TERRITORIOS) {
            printf("[ERRO] Numero invalido! Deve estar entre %d e %d territorios.\n", 
                   MIN_TERRITORIOS, MAX_TERRITORIOS);
        }
        
    } while (numTerritorios < MIN_TERRITORIOS || numTerritorios > MAX_TERRITORIOS);
    
    printf("\n[CONFIGURADO] Jogo sera jogado com %d territorios!\n", numTerritorios);
    
    return numTerritorios;
}

// ============================================================================
// FUNCOES DE SELECAO DINAMICA
// ============================================================================

int selecionarTerritorio(int territoriosUsados[], int numUsados) {
    // Lista de territórios predefinidos
    const char* territorios[NUM_TERRITORIOS_PREDEFINIDOS] = {
        "Brasil", "Argentina", "Chile", "Peru", "Colombia"
    };
    
    printf("\n================ SELECAO DE TERRITORIO ================\n");
    printf("Escolha um territorio da lista:\n");
    printf("======================================================\n");
    
    for (int i = 0; i < NUM_TERRITORIOS_PREDEFINIDOS; i++) {
        // Verificar se o território já foi usado
        int jaUsado = 0;
        for (int j = 0; j < numUsados; j++) {
            if (territoriosUsados[j] == i) {
                jaUsado = 1;
                break;
            }
        }
        
        if (jaUsado) {
            printf("%2d - %s [JA USADO]\n", i + 1, territorios[i]);
        } else {
            printf("%2d - %s\n", i + 1, territorios[i]);
        }
    }
    
    printf("======================================================\n");
    printf(">> Digite o numero do territorio (1-%d): ", NUM_TERRITORIOS_PREDEFINIDOS);
    
    int opcao;
    while (1) {
        if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > NUM_TERRITORIOS_PREDEFINIDOS) {
            printf("[!] Opcao invalida! Digite um numero entre 1 e %d: ", NUM_TERRITORIOS_PREDEFINIDOS);
            limparBufferEntrada();
            continue;
        }
        
        // Verificar se o território já foi usado
        int jaUsado = 0;
        for (int j = 0; j < numUsados; j++) {
            if (territoriosUsados[j] == (opcao - 1)) {
                jaUsado = 1;
                break;
            }
        }
        
        if (jaUsado) {
            printf("[!] Este territorio ja foi escolhido! Selecione outro: ");
            limparBufferEntrada();
            continue;
        }
        
        break;
    }
    limparBufferEntrada();
    
    return opcao - 1; // Retorna índice (0-based)
}

int selecionarCor(char* corEscolhida, int coresUsadas[], int numUsadas) {
    // Lista de cores predefinidas
    const char* cores[NUM_CORES_PREDEFINIDAS] = {
        "Azul", "Vermelho", "Verde", "Amarelo", "Roxo"
    };
    
    printf("\n================ SELECAO DE COR ================\n");
    printf("Escolha uma cor para o exercito:\n");
    printf("===============================================\n");
    
    for (int i = 0; i < NUM_CORES_PREDEFINIDAS; i++) {
        // Verificar se a cor já foi usada
        int jaUsada = 0;
        for (int j = 0; j < numUsadas; j++) {
            if (coresUsadas[j] == i) {
                jaUsada = 1;
                break;
            }
        }
        
        if (jaUsada) {
            printf("%2d - %s [JA USADA]\n", i + 1, cores[i]);
        } else {
            printf("%2d - %s\n", i + 1, cores[i]);
        }
    }
    
    printf("===============================================\n");
    printf(">> Digite o numero da cor (1-%d): ", NUM_CORES_PREDEFINIDAS);
    
    int opcao;
    while (1) {
        if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > NUM_CORES_PREDEFINIDAS) {
            printf("[!] Opcao invalida! Digite um numero entre 1 e %d: ", NUM_CORES_PREDEFINIDAS);
            limparBufferEntrada();
            continue;
        }
        
        // Verificar se a cor já foi usada
        int jaUsada = 0;
        for (int j = 0; j < numUsadas; j++) {
            if (coresUsadas[j] == (opcao - 1)) {
                jaUsada = 1;
                break;
            }
        }
        
        if (jaUsada) {
            printf("[!] Esta cor ja foi escolhida! Selecione outra: ");
            limparBufferEntrada();
            continue;
        }
        
        break;
    }
    limparBufferEntrada();
    
    strcpy(corEscolhida, cores[opcao - 1]);
    return opcao - 1; // Retorna índice (0-based)
}

int selecionarTropas(void) {
    // Opções de tropas predefinidas
    const int opcoesTropas[NUM_OPCOES_TROPAS] = {1, 2, 3, 4, 5};
    
    printf("\n================ SELECAO DE TROPAS ================\n");
    printf("Escolha o numero inicial de tropas:\n");
    printf("==================================================\n");
    
    for (int i = 0; i < NUM_OPCOES_TROPAS; i++) {
        printf("%d - %d tropa%s\n", i + 1, opcoesTropas[i], opcoesTropas[i] > 1 ? "s" : "");
    }
    
    printf("==================================================\n");
    printf(">> Digite o numero da opcao (1-%d): ", NUM_OPCOES_TROPAS);
    
    int opcao;
    while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > NUM_OPCOES_TROPAS) {
        printf("[!] Opcao invalida! Digite um numero entre 1 e %d: ", NUM_OPCOES_TROPAS);
        limparBufferEntrada();
    }
    limparBufferEntrada();
    
    return opcoesTropas[opcao - 1]; // Retorna o número de tropas
}

// === IMPLEMENTAÇÃO DAS FUNÇÕES DO SISTEMA DE MISSÕES ===

void gerarMissaoAleatoria(Missao* missao, const Territorio territorios[], int numTerritorios) {
    // Inicializar missão
    missao->cumprida = 0;
    strcpy(missao->corAlvo, "");
    missao->valorAlvo = 0;
    
    // Gerar tipo de missão aleatório
    missao->tipo = (TipoMissao)(rand() % 4);
    
    switch (missao->tipo) {
        case CONQUISTAR_CONTINENTE: {
            // Encontrar uma cor aleatória que existe no mapa
            int corEncontrada = 0;
            int tentativas = 0;
            while (!corEncontrada && tentativas < 10) {
                int indiceAleatorio = rand() % numTerritorios;
                strcpy(missao->corAlvo, territorios[indiceAleatorio].corExercito);
                
                // Verificar se esta cor tem pelo menos 2 territórios
                int count = 0;
                for (int i = 0; i < numTerritorios; i++) {
                    if (strcmp(territorios[i].corExercito, missao->corAlvo) == 0) {
                        count++;
                    }
                }
                if (count >= 2) {
                    corEncontrada = 1;
                    sprintf(missao->descricao, "Conquistar todos os territorios da cor %s", missao->corAlvo);
                }
                tentativas++;
            }
            if (!corEncontrada) {
                // Fallback para missão de controlar territórios
                missao->tipo = CONTROLAR_TERRITORIOS;
                missao->valorAlvo = (numTerritorios * 60) / 100; // 60% dos territórios
                sprintf(missao->descricao, "Controlar pelo menos %d territorios", missao->valorAlvo);
            }
            break;
        }
        
        case ELIMINAR_COR: {
            // Encontrar uma cor diferente da primeira para eliminar
            strcpy(missao->corAlvo, territorios[1 % numTerritorios].corExercito);
            sprintf(missao->descricao, "Eliminar completamente a cor %s do mapa", missao->corAlvo);
            break;
        }
        
        case CONTROLAR_TERRITORIOS: {
            // Controlar uma porcentagem dos territórios
            missao->valorAlvo = (numTerritorios * 70) / 100; // 70% dos territórios
            if (missao->valorAlvo < 3) missao->valorAlvo = 3; // Mínimo 3 territórios
            sprintf(missao->descricao, "Controlar pelo menos %d territorios", missao->valorAlvo);
            break;
        }
        
        case SOBREVIVER_TURNOS: {
            // Sobreviver por um número de turnos
            missao->valorAlvo = 8 + (rand() % 5); // Entre 8 e 12 turnos
            sprintf(missao->descricao, "Sobreviver por pelo menos %d turnos", missao->valorAlvo);
            break;
        }
    }
}

int verificarMissao(const Missao* missao, const Territorio territorios[], int numTerritorios, int turnoAtual) {
    if (missao->cumprida) {
        return 1; // Já foi cumprida
    }
    
    switch (missao->tipo) {
        case CONQUISTAR_CONTINENTE: {
            // Verificar se todos os territórios da cor alvo pertencem ao jogador
            int territoriosDaCor = 0;
            int territoriosControlados = 0;
            
            for (int i = 0; i < numTerritorios; i++) {
                if (strcmp(territorios[i].corExercito, missao->corAlvo) == 0) {
                    territoriosDaCor++;
                    // Assumindo que o jogador é sempre a primeira cor cadastrada
                    if (strcmp(territorios[i].corExercito, territorios[0].corExercito) == 0) {
                        territoriosControlados++;
                    }
                }
            }
            return (territoriosControlados == territoriosDaCor && territoriosDaCor > 0);
        }
        
        case ELIMINAR_COR: {
            // Verificar se a cor alvo não existe mais no mapa
            for (int i = 0; i < numTerritorios; i++) {
                if (strcmp(territorios[i].corExercito, missao->corAlvo) == 0) {
                    return 0; // Cor ainda existe
                }
            }
            return 1; // Cor foi eliminada
        }
        
        case CONTROLAR_TERRITORIOS: {
            // Contar territórios controlados pelo jogador (primeira cor)
            int territoriosControlados = 0;
            const char* corJogador = territorios[0].corExercito;
            
            for (int i = 0; i < numTerritorios; i++) {
                if (strcmp(territorios[i].corExercito, corJogador) == 0) {
                    territoriosControlados++;
                }
            }
            return (territoriosControlados >= missao->valorAlvo);
        }
        
        case SOBREVIVER_TURNOS: {
            return (turnoAtual >= missao->valorAlvo);
        }
        
        default:
            return 0;
    }
}

void exibirMissao(const Missao* missao) {
    printf("\n%s================ MISSAO ATUAL ================%s\n", COR_AMARELO_NEGRITO, COR_RESET);
    
    if (missao->cumprida) {
        printf("%s[MISSAO CUMPRIDA!]%s %s\n", COR_VERDE_NEGRITO, COR_RESET, missao->descricao);
        printf("%s=============================================%s\n", COR_VERDE_NEGRITO, COR_RESET);
    } else {
        printf("%s[OBJETIVO:]%s %s\n", COR_CIANO, COR_RESET, missao->descricao);
        
        // Informações adicionais baseadas no tipo
        switch (missao->tipo) {
            case CONQUISTAR_CONTINENTE:
                printf("%s[DICA:]%s Foque em atacar territorios da cor %s%s%s\n", 
                       COR_AMARELO, COR_RESET, COR_MAGENTA, missao->corAlvo, COR_RESET);
                break;
            case ELIMINAR_COR:
                printf("%s[DICA:]%s Elimine todos os territorios da cor %s%s%s\n", 
                       COR_AMARELO, COR_RESET, COR_MAGENTA, missao->corAlvo, COR_RESET);
                break;
            case CONTROLAR_TERRITORIOS:
                printf("%s[DICA:]%s Expanda seu territorio conquistando %s%d%s territorios\n", 
                       COR_AMARELO, COR_RESET, COR_VERDE, missao->valorAlvo, COR_RESET);
                break;
            case SOBREVIVER_TURNOS:
                printf("%s[DICA:]%s Defenda seus territorios por %s%d%s turnos\n", 
                       COR_AMARELO, COR_RESET, COR_VERDE, missao->valorAlvo, COR_RESET);
                break;
        }
        printf("%s=============================================%s\n", COR_AMARELO_NEGRITO, COR_RESET);
    }
}
