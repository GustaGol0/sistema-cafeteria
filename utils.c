#include "utils.h"
#include <stdio.h>
#include "variaveis.h"
#include "tipos.h"
#include <string.h>
#include <stdlib.h>

void printErro(char* string) {
  puts(COR_VERMELHO);
  printf("%s", string);
  puts(COR_DEFAULT);
}

void printMenu(char* string) {
  puts(COR_VERDE);
  printf("%s", string);
  puts(COR_DEFAULT);
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limparString(char* string) {
  int register i = 0;
  while (string[i] != '\0') {
    if (string[i] == '\0') {
      string[i] = ' ';
      break;
    }
    if(string[i] == '\n') string[i] = '\0';
    i++;
  }
}

void salvarItemCardapio(ItemCardapio* item) {
  FILE* arquivo = fopen(ARQUIVO_CARDAPIO, "a+");

  fprintf(arquivo, "%s,%.2f,%d\n", item->nome, item->preco, item->estoque);

  fclose(arquivo);
}

int obterItemCardapio(FILE* arquivo, ItemCardapio* item) {
  return fscanf(
      arquivo,
      "%[^,],%f,%d\n",
      item->nome, &(item->preco), &(item->estoque));
}

int buscarItemCardapio(char* nome, ItemCardapio* item) {
  FILE* arquivo = fopen(ARQUIVO_CARDAPIO, "r");
  if (arquivo == NULL) {
    printErro("Erro ao abrir o arquivo");
    return -1;
  }

  while(obterItemCardapio(arquivo, item) != EOF) {
      if (strcmp(item->nome, nome) == 0) {
        fclose(arquivo);
        return 1;
      }
  }

  fclose(arquivo);
  return 0;
}

void deletarItemCardapio(ItemCardapio* itemParaDeletar) {
  ItemCardapio* itemAtual = alocaItemCardapio();
  FILE* arquivo = fopen(ARQUIVO_CARDAPIO, "r");
  FILE* provisorio = fopen("provisorio.txt", "w");
  
  while(obterItemCardapio(arquivo, itemAtual) != EOF) {
      if (strcmp(itemParaDeletar->nome, itemAtual->nome) == 0) {
        continue;
      }
      
      fprintf(
        provisorio,
        "%s,%.2f,%d\n",
        itemAtual->nome,
        itemAtual->preco,
        itemAtual->estoque
      );
  }

  remove(ARQUIVO_CARDAPIO);
  rename("provisorio.txt", ARQUIVO_CARDAPIO);

  fclose(arquivo);
  fclose(provisorio);
  free(itemAtual);
}

void alterarItemCardapio(ItemCardapio* itemAlterado, int menu) {
  float precoNovo;
  int quantidadeNova, resultado;

  if (menu == 1) {
    do {
      printf("Insira a nova quantidade em estoque do ItemCardapio: ");
      resultado = scanf("%d", &quantidadeNova);
      if (resultado != 1) {
        printErro("Quantidade inválida. Digite novamente.");
        limparBuffer();
        continue;
      }
    } while (resultado != 1);

    getchar();

  } else if (menu == 2) {
    do {
      printf("Insira o novo preço do ItemCardapio: ");
      resultado = scanf("%f", &precoNovo);
      if (resultado != 1) {
        printErro("Preço inválido. Digite novamente.");
        limparBuffer();
        continue;
      }
  } while (resultado != 1);

    getchar();

  } else {
      return;
  }

  ItemCardapio* itemAtual = alocaItemCardapio();
  FILE* arquivo = fopen(ARQUIVO_CARDAPIO, "r");
  FILE* provisorio = fopen("provisorio.txt", "w");
  
  while(obterItemCardapio(arquivo, itemAtual) != EOF) {
      // Encontrou o ItemCardapio
      if (strcmp(itemAlterado->nome, itemAtual->nome) == 0) {
        if (menu == 1) {
          itemAtual->estoque = quantidadeNova;
        } else if (menu == 2) {
          itemAtual->preco = precoNovo;
        }
      } 
      fprintf(
        provisorio,
        "%s,%.2f,%d\n",
        itemAtual->nome,
        itemAtual->preco,
        itemAtual->estoque
      );
  }

  remove(ARQUIVO_CARDAPIO);
  rename("provisorio.txt", ARQUIVO_CARDAPIO);

  fclose(arquivo);
  fclose(provisorio);
  free(itemAtual);
}

ItemCardapio* alocaItemCardapio() {
  ItemCardapio* item = (ItemCardapio *)malloc(sizeof(ItemCardapio));

  return item;
}