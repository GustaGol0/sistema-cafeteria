#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "variaveis.h"
#include "tipos.h"

int menuPrincipal() {
  int menu;
  puts("\n");
  puts(DIVISOR);
  printMenu("\tMenu Principal\n");
  puts("Digite a opção desejada");
  puts("1 - Adicionar Item ao Cardápio");
  puts("2 - Consultar Item do Cardápio");
  puts("3 - Mostrar Cardápio Completo");
  puts("4 - Vender Itens do Cardapio");
  puts("0 - Sair do Programa");
  scanf("%d", &menu);
  getchar();
  return menu;
}

int menuAlteracao() {
  int menu;
  puts(DIVISOR);
  puts("Selecione a opção desejada");
  puts("1 - Deletar item");
  puts("2 - Alterar item");
  puts("0 - Não realizar ação");
  scanf("%d", &menu);
  getchar();

  return menu;
}

// Função auxiliar para converter string para minúsculas
void paraMinusculas(char* str, char* resultado) {
  int i = 0;
  while (str[i]) {
    resultado[i] = tolower((unsigned char)str[i]);
    i++;
  }
  resultado[i] = '\0';
}

// Função para verificar se um item já existe no cardápio (case-insensitive)
int itemJaExiste(const char* nome) {
  FILE* arquivo = fopen(ARQUIVO_CARDAPIO, "r");
  if (arquivo == NULL) {
    return 0; // Se o arquivo não existe, o item não existe
  }

  char linha[1024];
  char nomeArquivo[TAMANHO_NOME];
  char nomeBusca[TAMANHO_NOME];
  char nomeArquivoLower[TAMANHO_NOME];
  float precoTemp;
  int estoqueTemp;

  // Converter nome buscado para minúsculas
  paraMinusculas((char*)nome, nomeBusca);

  while (fgets(linha, sizeof(linha), arquivo)) {
    int resultado = sscanf(linha, "%[^,],%f,%d", nomeArquivo, &precoTemp, &estoqueTemp);
    
    if (resultado == 3) {
      nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';
      
      // Converter nome do arquivo para minúsculas
      paraMinusculas(nomeArquivo, nomeArquivoLower);
      
      // Comparação case-insensitive
      if (strcmp(nomeArquivoLower, nomeBusca) == 0) {
        fclose(arquivo);
        return 1; // Item encontrado
      }
    }
  }

  fclose(arquivo);
  return 0; // Item não encontrado
}

void cadastrarItemCardapio() {
  ItemCardapio item;
  int resultado;

  puts(DIVISOR);
  printMenu("\tCADASTRO DE ITEM");
  
  printf("Insira o nome: ");
  fgets(item.nome, TAMANHO_NOME, stdin);
  limparString(item.nome);

  // Verificar se o item já existe (case-insensitive)
  if (itemJaExiste(item.nome)) {
    printErro("Este item já existe no cardápio!");
    printf("Dica: '%s' já está cadastrado (maiúsculas/minúsculas são ignoradas).\n", item.nome);
    return;
  }

  do {
      printf("Insira o preço: ");
      resultado = scanf("%f", &item.preco);
      if (resultado != 1) {
        printErro("Preço inválido. Digite novamente.");
        limparBuffer();
        continue;
      }
  } while (resultado != 1);

  do {
      printf("Insira o estoque: ");
      resultado = scanf("%d", &item.estoque);
      if (resultado != 1) {
        printErro("Estoque inválido. Digite novamente.");
        limparBuffer();
        continue;
      }
  } while (resultado != 1);

  salvarItemCardapio(&item);
  printf("\nItem '%s' cadastrado com sucesso!\n", item.nome);
}

// Função para buscar item no cardápio (case-insensitive)
int buscarItemCaseInsensitive(const char* nome, ItemCardapio* item) {
  FILE* arquivo = fopen(ARQUIVO_CARDAPIO, "r");
  if (arquivo == NULL) {
    return 0;
  }

  char linha[1024];
  char nomeArquivo[TAMANHO_NOME];
  char nomeBusca[TAMANHO_NOME];
  char nomeArquivoLower[TAMANHO_NOME];
  float precoTemp;
  int estoqueTemp;

  // Converter nome buscado para minúsculas
  paraMinusculas((char*)nome, nomeBusca);

  while (fgets(linha, sizeof(linha), arquivo)) {
    int resultado = sscanf(linha, "%[^,],%f,%d", nomeArquivo, &precoTemp, &estoqueTemp);
    
    if (resultado == 3) {
      nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';
      
      // Converter nome do arquivo para minúsculas
      paraMinusculas(nomeArquivo, nomeArquivoLower);
      
      // Comparação case-insensitive
      if (strcmp(nomeArquivoLower, nomeBusca) == 0) {
        // Item encontrado - copiar dados
        strcpy(item->nome, nomeArquivo);
        item->preco = precoTemp;
        item->estoque = estoqueTemp;
        fclose(arquivo);
        return 1;
      }
    }
  }

  fclose(arquivo);
  return 0;
}

void consultarItemCardapio() {
  puts(DIVISOR);
  printMenu("\tConsulta de Item");
  char nome[TAMANHO_NOME];
  printf("Insira o nome do item: ");
  fgets(nome, TAMANHO_NOME, stdin);
  limparString(nome);

  ItemCardapio* item = alocaItemCardapio();

  // Usar busca case-insensitive
  int achou = buscarItemCaseInsensitive(nome, item);

  if (achou == 1) {
    puts(DIVISOR);
    printMenu("Info do Item");
    printf("Nome: %s\n", item->nome);
    printf("Preço em R$: %.2f\n", item->preco);
    printf("Estoque: %d\n", item->estoque);

    int menu = menuAlteracao();

    if (menu == 1) {
      deletarItemCardapio(item);
    } else if (menu == 2) {
      puts(DIVISOR);
      puts("Selecione o que deseja alterar");
      puts("1 - Estoque");
      puts("2 - Preço");
      scanf("%d", &menu);

      alterarItemCardapio(item, menu);
    }
  } else {
    printErro("Item não encontrado");
  }
}

void mostrarItemCardapio() {
    puts(DIVISOR);
    printMenu("\tLISTA DE ITENS DO CARDÁPIO\n");

    FILE* arquivo = fopen(ARQUIVO_CARDAPIO, "r");
    if (arquivo == NULL) {
        printErro("Erro ao abrir o arquivo");
        return;
    }

    char linha[1024];
    float precoAtual;
    int estoqueAtual;
    char nomeAtual[TAMANHO_NOME];
    int contador = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        int resultado = sscanf(linha, "%[^,],%f,%d", nomeAtual, &precoAtual, &estoqueAtual);

        if (resultado == 3) {
            nomeAtual[strcspn(nomeAtual, "\n")] = '\0';
            contador++;

            printf("Item: %s\n", nomeAtual);
            printf("Preço: R$ %.2f\n", precoAtual);
            printf("Estoque: %d\n\n", estoqueAtual);
        } else {
            printf("Erro ao ler os dados do item: %s\n", linha);
        }
    }

    if (contador == 0) {
        printf("Nenhum item cadastrado no cardápio.\n");
    } else {
        printf("Total de itens: %d\n", contador);
    }

    fclose(arquivo);
}


typedef struct {
    int id_produto;
    char nomeproduto[50];
    int qtd_vendida;
    float preco;
} item_relatorio_vendas;

int conta_linhas(FILE* arq1) {
    char nome[50];
    int qtd;
    float preco;
    int i = 0;

    rewind(arq1);

    while (fscanf(arq1, " %49[^,], %f, %d", nome, &preco, &qtd) == 3) {
        i++;
    }

    rewind(arq1);
    return i;
}

void atualizarCardapio(item_relatorio_vendas** vetor, int* linhas, FILE* arq1) {
    int novas_linhas = conta_linhas(arq1);

    if (*linhas == 0) {
        *vetor = malloc(novas_linhas * sizeof(item_relatorio_vendas));
        printf("[INFO] Cardápio inicializado com %d itens.\n", novas_linhas);
    } else if (novas_linhas != *linhas) {
        *vetor = realloc(*vetor, novas_linhas * sizeof(item_relatorio_vendas));
        printf("[INFO] Cardápio atualizado: %d → %d itens.\n", *linhas, novas_linhas);
    } else {
        printf("[INFO] Cardápio mantido (%d itens).\n", *linhas);
    }

    *linhas = novas_linhas;

    char nome[50];
    int qtd;
    float preco;
    int i = 0;

    while (fscanf(arq1, " %49[^,], %f, %d", nome, &preco, &qtd) == 3) {
        (*vetor)[i].id_produto = i + 1;
        strcpy((*vetor)[i].nomeproduto, nome);
        (*vetor)[i].preco = preco;
        if ((*vetor)[i].qtd_vendida < 0) (*vetor)[i].qtd_vendida = 0;
        i++;
    }
}

void fazerRelatorio(item_relatorio_vendas vetor[], int linhas, int controle) {
    int qtd_vendida;
    printf("\nQuantos itens foram vendidos? ");
    scanf("%d", &qtd_vendida);

    if (controle >= 0 && controle < linhas) {
        vetor[controle].qtd_vendida += qtd_vendida;
        printf("\nQuantidade atualizada para '%s'!\n", vetor[controle].nomeproduto);
    } else {
        printf("\nNúmero de item inválido!\n");
    }
}

void vendas() {
    static item_relatorio_vendas* vetor = NULL;
    static int linhas = 0;

    FILE* arq1 = fopen("cardapio.txt", "r");
    FILE* arq = fopen("relatorio_vendas.txt", "w");
    if (!arq1 || !arq) {
        perror("Falha ao abrir arquivo");
        return;
    }

    atualizarCardapio(&vetor, &linhas, arq1);

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("\n--- CARDÁPIO ATUAL ---\n");
    for (int i = 0; i < linhas; i++) {
        printf("%d - %s (R$ %.2f)\n", i, vetor[i].nomeproduto, vetor[i].preco);
    }

    int controle;
    printf("\nDigite o número do item a vender (-1 para sair): ");
    scanf("%d", &controle);

    while (controle != -1) {
        fazerRelatorio(vetor, linhas, controle);
        printf("\nDigite outro item (-1 para sair): ");
        scanf("%d", &controle);
    }

    fprintf(arq, "\n=== RELATÓRIO DE VENDAS ===\n");
    for (int i = 0; i < linhas; i++) {
        if (vetor[i].qtd_vendida > 0) {
            fprintf(arq, "%s, %d, %.2f\n",
                    vetor[i].nomeproduto,
                    vetor[i].qtd_vendida,
                    vetor[i].preco);
        }
    }

    fclose(arq1);
    fclose(arq);
}

void lidarMenuPrincipal(unsigned int opcao) {
  switch (opcao) {
  case 0:
    return;
    break;
  case 1:
    cadastrarItemCardapio();
    break;
  case 2:
    consultarItemCardapio();
    break;
  case 3:
    mostrarItemCardapio();
    break;
  case 4:
    vendas();
    break;
  default:
    printErro("Opção não reconhecida");
    break;
  }
  int opcaoNova = menuPrincipal();
  lidarMenuPrincipal(opcaoNova);
}