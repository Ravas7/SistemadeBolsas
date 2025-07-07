#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[50];
    int idade;
    char escola[50];
    float renda_familiar;
    float nota_media;
} Aluno;

typedef enum {
    ADMIN,
    USUARIO
} Perfil;


Perfil login() {
    char usuario[20], senha[20];
    printf("\n=== Login no Sistema ===\n");
    printf("Usuario: ");
    scanf(" %[^\n]", usuario);
    printf("Senha: ");
    scanf(" %[^\n]", senha);

    if (strcmp(usuario, "admin") == 0 && strcmp(senha, "admin123") == 0) {
        return ADMIN;
    } else if (strcmp(usuario, "usuario") == 0 && strcmp(senha, "usuario123") == 0) {
        return USUARIO;
    } else {
        printf("Credenciais invalidas. Encerrando...\n");
        exit(1);
    }
}

Aluno* cadastrar_aluno(Aluno *lista, int *total) {
    lista = realloc(lista, (*total + 1) * sizeof(Aluno));
    if (lista == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }

    printf("\n--- Cadastro de Aluno ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", lista[*total].nome);

    printf("Idade: ");
    scanf("%d", &lista[*total].idade);

    printf("Escola: ");
    scanf(" %[^\n]", lista[*total].escola);

    printf("Renda familiar (R$): ");
    scanf("%f", &lista[*total].renda_familiar);

    printf("Nota media: ");
    scanf("%f", &lista[*total].nota_media);

    (*total)++;
    return lista;
}

void listar_alunos(Aluno *lista, int total) {
    if (total == 0) {
        printf("\nNenhum aluno cadastrado!\n");
        return;
    }

    printf("\n--- Lista de Alunos ---\n");
    for (int i = 0; i < total; i++) {
        printf("\nAluno %d:\n", i + 1);
        printf("Nome: %s\n", lista[i].nome);
        printf("Idade: %d\n", lista[i].idade);
        printf("Escola: %s\n", lista[i].escola);
        printf("Renda familiar: R$ %.2f\n", lista[i].renda_familiar);
        printf("Nota media: %.1f\n", lista[i].nota_media);
    }
}

void verificar_bolsas(Aluno *lista, int total) {
    if (total == 0) {
        printf("\nNenhum aluno cadastrado!\n");
        return;
    }

    printf("\n--- Alunos Elegiveis para Bolsa ---\n");
    int elegiveis = 0;
    int renda_suficiente_nota_baixa = 0;

    for (int i = 0; i < total; i++) {
        if (lista[i].renda_familiar <= 1000 && lista[i].nota_media >= 7.0) {
            printf("\n%s (Escola: %s)\n", lista[i].nome, lista[i].escola);
            printf("Nota: %.1f | Renda: R$ %.2f\n",
                   lista[i].nota_media, lista[i].renda_familiar);
            elegiveis++;
        } else if (lista[i].renda_familiar <= 1000 && lista[i].nota_media < 7.0) {
            renda_suficiente_nota_baixa++;
        }
    }

    if (elegiveis == 0) {
        printf("\nNenhum aluno elegivel encontrado!\n");
    }

    if (renda_suficiente_nota_baixa >= total / 2) {
        printf("\nALERTA: Muitos alunos com renda adequada estão ficando sem bolsa por causa da nota.\n");
        printf("Isso indica que o ensino precisa ser melhorado.\n");
    }
}

void salvar_dados(Aluno *lista, int total) {
    FILE *arquivo = fopen("alunos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%s;%d;%s;%.2f;%.1f\n",
                lista[i].nome, lista[i].idade, lista[i].escola,
                lista[i].renda_familiar, lista[i].nota_media);
    }

    fclose(arquivo);
    printf("\nDados salvos com sucesso! Total de alunos: %d\n", total);
}

Aluno* carregar_dados(Aluno *lista, int *total) {
    FILE *arquivo = fopen("alunos.txt", "r");
    if (arquivo == NULL) {
        return lista;
    }

    char linha[256];
    *total = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = 0;

        lista = realloc(lista, (*total + 1) * sizeof(Aluno));
        if (lista == NULL) {
            printf("Erro de alocacao durante carregamento!\n");
            exit(1);
        }

        char *token = strtok(linha, ";");
        if (token) strcpy(lista[*total].nome, token);

        token = strtok(NULL, ";");
        if (token) lista[*total].idade = atoi(token);

        token = strtok(NULL, ";");
        if (token) strcpy(lista[*total].escola, token);

        token = strtok(NULL, ";");
        if (token) lista[*total].renda_familiar = atof(token);

        token = strtok(NULL, ";");
        if (token) lista[*total].nota_media = atof(token);

        (*total)++;
    }

    fclose(arquivo);
    return lista;
}

int main() {
    Aluno *alunos = NULL;
    int total_alunos = 0;
    int opcao;

    Perfil perfil_usuario = login();  

    alunos = carregar_dados(alunos, &total_alunos);

    do {
        printf("\n--- Sistema de Bolsas de Estudo (ODS 4) ---\n");
        printf("1. Listar alunos\n");
        printf("2. Verificar elegibilidade para bolsa\n");

        if (perfil_usuario == ADMIN) {
            printf("3. Cadastrar aluno\n");
            printf("4. Salvar e sair\n");
        } else {
            printf("3. Sair\n");
        }

        printf("Opcao: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');

        if (perfil_usuario == ADMIN) {
            switch (opcao) {
                case 1:
                    listar_alunos(alunos, total_alunos);
                    break;
                case 2:
                    verificar_bolsas(alunos, total_alunos);
                    break;
                case 3:
                    alunos = cadastrar_aluno(alunos, &total_alunos);
                    break;
                case 4:
                    salvar_dados(alunos, total_alunos);
                    break;
                default:
                    printf("Opcao invalida!\n");
            }
        } else { 
            switch (opcao) {
                case 1:
                    listar_alunos(alunos, total_alunos);
                    break;
                case 2:
                    verificar_bolsas(alunos, total_alunos);
                    break;
                case 3:
                    printf("Encerrando o sistema...\n");
                    break;
                default:
                    printf("Opcao invalida!\n");
            }
        }
    } while ((perfil_usuario == ADMIN && opcao != 4) || (perfil_usuario == USUARIO && opcao != 3));

    free(alunos);
    return 0;
}
