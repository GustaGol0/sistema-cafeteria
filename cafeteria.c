#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include "funcoes.h"

int main() {
  ItemCardapio* item = (ItemCardapio *)malloc(sizeof(ItemCardapio));

  int opcao = menuPrincipal();

  lidarMenuPrincipal(opcao);

  free(item);
  return 0;
}