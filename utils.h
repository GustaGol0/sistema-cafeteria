#include "tipos.h"
#include <stdio.h>

void printErro(char* string);

void printMenu(char* string);

void limparBuffer();

void limparString(char* string);

void salvarItemCardapio(ItemCardapio* item);

int buscarItemCardapio(char* nome, ItemCardapio* item);

int obterItemCardapio(FILE* arquivo, ItemCardapio* item);

void deletarItemCardapio(ItemCardapio* item);

void alterarItemCardapio(ItemCardapio* item, int menu);

ItemCardapio* alocaItemCardapio();