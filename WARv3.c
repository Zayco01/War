#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura que representa um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função auxiliar para ler string
void lerString(const char* mensagem, char* dest, int tamanho) {
    printf("%s", mensagem);
    if (fgets(dest, tamanho, stdin)) {
        dest[strcspn(dest, "\n")] = '\0';
    }
}

// Lê um número inteiro com validação
int lerInteiro(const char* mensagem) {
    int valor;
    printf("%s", mensagem);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada inválida. Digite um número inteiro: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}

// === MISSÕES ===

// Sorteia uma missão aleatória
void atribuirMissao(char* destino, char* missoes[], int total) {
    int indice = rand() % total;
    strcpy(destino, missoes[indice]);
}

// Exibe a missão do jogador
void exibirMissao(const char* missao) {
    printf("\n🎯 Sua missão é: %s\n", missao);
}

// Verifica se a missão foi cumprida
int verificarMissao(const char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        int cont = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                cont++;
        }
        if (cont >= 3) return 1;
    }
    else if (strcmp(missao, "Dominar todos os territorios") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0)
                return 0;
        }
        return 1;
    }
    else if (strcmp(missao, "Ter pelo menos 5 tropas em um territorio") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 5)
                return 1;
        }
    }
    else if (strcmp(missao, "Eliminar todos os territorios inimigos") == 0) {
        int inimigo = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0)
                inimigo++;
        }
        if (inimigo == 0) return 1;
    }
    return 0;
}

// === FUNÇÕES DE JOGO ===

// Cadastro inicial dos territórios
void cadastrarTerritorios(Territorio* mapa, int qtd) {
    printf("\n=== Cadastro de Territórios ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("\n--- Território %d ---\n", i);
        lerString("Nome: ", mapa[i].nome, sizeof(mapa[i].nome));
        lerString("Cor do exército: ", mapa[i].cor, sizeof(mapa[i].cor));
        mapa[i].tropas = lerInteiro("Quantidade de tropas: ");
        if (mapa[i].tropas < 0) mapa[i].tropas = 0;
    }
}

// Exibe todos os territórios
void exibirTerritorios(Territorio* mapa, int qtd) {
    printf("\n=== Estado Atual do Mapa ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
            i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula um ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 0) {
        printf("O atacante não possui tropas suficientes.\n");
        return;
    }
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Não é possível atacar territórios da mesma cor.\n");
        return;
    }

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;
    printf("\n⚔️ %s (%s) atacou %s (%s)\n", atacante->nome, atacante->cor,
        defensor->nome, defensor->cor);
    printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        defensor->tropas -= dadoA;
        if (defensor->tropas < 0) defensor->tropas = 0;
        printf("O atacante causou %d de dano! Tropas restantes do defensor: %d\n",
            dadoA, defensor->tropas);

        if (defensor->tropas == 0) {
            printf("🎉 Território conquistado!\n");
            strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
            int transferidas = atacante->tropas / 2;
            atacante->tropas -= transferidas;
            defensor->tropas = transferidas;
            printf("Cor alterada para '%s'. %d tropas transferidas.\n",
                defensor->cor, transferidas);
        }
    }
    else {
        atacante->tropas--;
        printf("O defensor resistiu! O atacante perdeu uma tropa.\n");
    }
}

// Libera memória alocada
void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
    printf("\nMemória liberada com sucesso!\n");
}

// === MAIN ===
int main(void) {
	setlocale(LC_ALL, "Portuguese");
    srand((unsigned)time(NULL));

    // Lista de missões possíveis
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Dominar todos os territorios",
        "Ter pelo menos 5 tropas em um territorio",
        "Eliminar todos os territorios inimigos"
    };
    int totalMissoes = 4;

    // Jogador único
    char corJogador[10];
    lerString("Digite a cor do seu exército: ", corJogador, sizeof(corJogador));

    // Missão dinâmica
    char* missaoJogador = (char*)malloc(100 * sizeof(char));
    atribuirMissao(missaoJogador, missoes, totalMissoes);
    exibirMissao(missaoJogador);

    // Cadastro do mapa
    int qtd = lerInteiro("\nDigite o número total de territórios: ");
    Territorio* mapa = (Territorio*)calloc((size_t)qtd, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, qtd);

    int opcao = -1;
    while (opcao != 0) {
        printf("\n===== MENU =====\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar missão\n");
        printf("3 - Mostrar territórios\n");
        printf("0 - Sair\n");
        printf("================\n");

        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
        case 1: {
            exibirTerritorios(mapa, qtd);
            int iAtacante = lerInteiro("\nÍndice do território atacante: ");
            int iDefensor = lerInteiro("Índice do território defensor: ");
            if (iAtacante >= 0 && iAtacante < qtd && iDefensor >= 0 && iDefensor < qtd)
                atacar(&mapa[iAtacante], &mapa[iDefensor]);
            else
                printf("Índices inválidos!\n");
            break;
        }
        case 2:
            if (verificarMissao(missaoJogador, mapa, qtd, corJogador))
                printf("\n🏆 Parabéns! Você concluiu sua missão: %s\n", missaoJogador);
            else
                printf("\n🚫 Missão ainda não concluída. Continue tentando!\n");
            break;
        case 3:
            exibirTerritorios(mapa, qtd);
            break;
        case 0:
            printf("\nSaindo do jogo...\n");
            break;
        default:
            printf("Opção inválida.\n");
        }

        // Verificação automática após cada turno
        if (verificarMissao(missaoJogador, mapa, qtd, corJogador)) {
            printf("\n🏆 Vitória! Você cumpriu sua missão: %s\n", missaoJogador);
            break;
        }
    }

    liberarMemoria(mapa, missaoJogador);
    printf("\nJogo encerrado.\n");
    return 0;
}
