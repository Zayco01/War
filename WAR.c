#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>

// Definição da struct Territorio
// Representa um território com nome, cor do exército e quantidade de tropas
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
	setlocale(LC_ALL, "Portuguese");
    struct Territorio territorios[5]; // Vetor para armazenar 5 territórios
    int i;

    printf("=== Sistema de Cadastro de Territórios ===\n\n");

    // Entrada dos dados dos 5 territórios
    for (i = 0; i < 5; i++) {
        printf("Cadastro do territorio %d:\n", i + 1);

        // Leitura do nome do território
        printf("Digite o nome do territorio: ");
        scanf(" %[^\n]", territorios[i].nome); // Lê string com espaços

        // Leitura da cor do exército
        printf("Digite a cor do exercito: ");
        scanf(" %[^\n]", territorios[i].cor);

        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("--------------------------------------\n");
    }

    // Exibição dos dados após o cadastro
    printf("\n=== Dados dos Territorios Cadastrados ===\n\n");

    for (i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
        printf("--------------------------------------\n");
    }

    printf("Fim do cadastro.\n");
    return 0;
}
