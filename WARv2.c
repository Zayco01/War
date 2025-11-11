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

// Lê string com segurança
void lerString(const char* mensagem, char* dest, int tamanho) {
    printf("%s", mensagem);
    if (!fgets(dest, tamanho, stdin)) {
        dest[0] = '\0';
        return;
    }
    dest[strcspn(dest, "\n")] = '\0'; // remove '\n'
}

// Lê inteiro com verificação simples
int lerInteiro(const char* mensagem) {
    int valor;
    printf("%s", mensagem);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada inválida. Digite um número inteiro: ");
        while (getchar() != '\n'); // descarta buffer
    }
    while (getchar() != '\n'); // limpa o '\n' restante
    return valor;
}

// Cadastra territórios no mapa 
void cadastrarTerritorios(Territorio* mapa, int qtd) {
    printf("\n=== Cadastro de Territórios ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("\n--- Território %d ---\n", i);
        lerString("Nome do território: ", mapa[i].nome, sizeof(mapa[i].nome));
        lerString("Cor do exército: ", mapa[i].cor, sizeof(mapa[i].cor));
        mapa[i].tropas = lerInteiro("Quantidade de tropas: ");
        if (mapa[i].tropas < 0) mapa[i].tropas = 0;
    }
}

// Exibe todos os territórios com índice
void exibirTerritorios(Territorio* mapa, int qtd) {
    printf("\n=== Dados dos Territórios ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
            i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Função que simula o ataque entre dois territórios (usa ponteiros)
void atacar(Territorio* atacante, Territorio* defensor) {
    // validações básicas
    if (atacante->tropas <= 0) {
        printf("O território atacante não possui tropas suficientes para atacar.\n");
        return;
    }
    if (defensor->tropas <= 0) {
        printf("O território defensor já não possui tropas. Ele pode ser automaticamente conquistado.\n");
        return;
    }

    printf("\n--- Simulando ataque ---\n");
    printf("%s (%s, %d tropas) está atacando %s (%s, %d tropas)\n",
        atacante->nome, atacante->cor, atacante->tropas,
        defensor->nome, defensor->cor, defensor->tropas);

    // rolagem de dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado do atacante: %d | Dado do defensor: %d\n",
        dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // atacante vence a rodada: causa dano às tropas do defensor
        int dano = dadoAtacante; // escolha de regra: dano igual ao valor do dado do atacante
        defensor->tropas -= dano;
        if (defensor->tropas < 0) defensor->tropas = 0;
        printf("O atacante venceu a rodada e causou %d de dano ao defensor. Tropas do defensor agora: %d\n",
            dano, defensor->tropas);

        // verifica se defensor foi reduzido a 0 tropas -> conquista
        if (defensor->tropas == 0) {
            printf("!!! Território conquistado !!!\n");
            // transferir metade das tropas do atacante para o território conquistado
            int transferidas = atacante->tropas / 2; // metade, truncando
            if (transferidas < 0) transferidas = 0;
            // atualiza tropas e cor
            defensor->tropas = transferidas;
            atacante->tropas -= transferidas;
            if (atacante->tropas < 0) atacante->tropas = 0;
            // muda a cor do defensor para a do atacante
            strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
            defensor->cor[sizeof(defensor->cor) - 1] = '\0';
            printf("Cor do defensor alterada para '%s'. %d tropas transferidas do atacante para o novo dono.\n",
                defensor->cor, transferidas);
        }
        else {
            // atacante também perde 1 tropa por custo da batalha
            atacante->tropas--;
            if (atacante->tropas < 0) atacante->tropas = 0;
            printf("O atacante perdeu 1 tropa no confronto. Tropas do atacante agora: %d\n", atacante->tropas);
        }
    }
    else {
        // defensor resiste: atacante perde 1 tropa
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("O defensor resistiu. O atacante perde 1 tropa. Tropas do atacante agora: %d\n", atacante->tropas);
    }

    printf("Ataque finalizado.\n");
}

// Libera memória alocada dinamicamente
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso!\n");
}

// Função principal com loop de múltiplas batalhas
int main(void) {
	setlocale(LC_ALL, "Portuguese");
    srand((unsigned)time(NULL));

    printf("Digite o número total de territórios: ");
    int qtd = lerInteiro("");
    while (qtd <= 0) {
        printf("Digite um número válido de territórios (> 0): ");
        qtd = lerInteiro("");
    }

    // aloca dinamicamente o mapa
    Territorio* mapa = (Territorio*)calloc((size_t)qtd, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 1;
    }

    // cadastro inicial
    cadastrarTerritorios(mapa, qtd);
    exibirTerritorios(mapa, qtd);

    int continuar = 1;
    while (continuar) {
        printf("\n=== Simulação de Ataque ===\n");
        exibirTerritorios(mapa, qtd);

        int iAtacante = -1, iDefensor = -1;

        // escolha válida de territórios
        while (1) {
            printf("\nEscolha o índice do território atacante: ");
            iAtacante = lerInteiro("");
            printf("Escolha o índice do território defensor: ");
            iDefensor = lerInteiro("");

            // validações de índice
            if (iAtacante < 0 || iAtacante >= qtd || iDefensor < 0 || iDefensor >= qtd) {
                printf("Índices inválidos (devem estar entre 0 e %d). Tente novamente.\n", qtd - 1);
                continue;
            }
            if (iAtacante == iDefensor) {
                printf("Um território não pode atacar a si mesmo. Tente novamente.\n");
                continue;
            }
            if (mapa[iAtacante].tropas <= 0) {
                printf("O território atacante não possui tropas suficientes. Escolha outro atacante.\n");
                continue;
            }
            if (strcmp(mapa[iAtacante].cor, mapa[iDefensor].cor) == 0) {
                printf("Um território não pode atacar outro da mesma cor. Tente novamente.\n");
                continue;
            }
            break; // escolhas válidas
        }

        // executa o ataque
        atacar(&mapa[iAtacante], &mapa[iDefensor]);

        // exibe estado atualizado
        exibirTerritorios(mapa, qtd);

        // verifica se o jogador quer continuar
        printf("\nDeseja realizar outro ataque? (1 = Sim / 0 = Não): ");
        continuar = lerInteiro("");
        while (continuar != 0 && continuar != 1) {
            printf("Opção inválida. Digite 1 para Sim ou 0 para Não: ");
            continuar = lerInteiro("");
        }
    }

    // libera memória e finaliza
    liberarMemoria(mapa);
    printf("\nJogo encerrado. Obrigado por jogar!\n");
    return 0;
}
