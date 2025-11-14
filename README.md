# Sistema de Gerenciamento de Cafeteria

Sistema CRUD (Create, Read, Update, Delete) completo para gerenciamento de cafeteria desenvolvido em linguagem C.

## 📋 Funcionalidades

- **Cadastrar** itens no cardápio (nome, preço, estoque)
- **Consultar** itens específicos (busca case-insensitive)
- **Listar** todo o cardápio
- **Alterar** preço ou estoque de itens
- **Deletar** itens do cardápio
- **Sistema de vendas** com geração de relatórios
- **Prevenção de duplicatas** (ignora maiúsculas/minúsculas)

## 🚀 Como usar

### Compilação

```bash
gcc -o cafeteria main.c utils.c -I.
```

### Execução

```bash
./cafeteria
```

## 📁 Estrutura do Projeto

```
.
├── main.c              # Arquivo principal com menu e funções CRUD
├── utils.c             # Funções utilitárias
├── utils.h             # Cabeçalhos das funções utilitárias
├── variaveis.h         # Definições de variáveis e constantes
├── tipos.h             # Definição de estruturas de dados
├── cardapio.txt        # Armazenamento do cardápio (gerado automaticamente)
├── relatorio_vendas.txt # Relatório de vendas (gerado automaticamente)
└── README.md           # Este arquivo
```

## 🛠️ Tecnologias

- Linguagem C
- Manipulação de arquivos
- Alocação dinâmica de memória

## 📝 Formato dos arquivos

### cardapio.txt

```
Pizza,25.50,10
Café,5.00,50
Suco,7.00,30
```

### relatorio_vendas.txt

```
=== RELATÓRIO DE VENDAS ===
Pizza, 5, 25.50
Café, 15, 5.00
```

## 👨‍💻 Autor

Gustavo Soares Almeida - [GitHub](https://github.com/GustaGol0)
Guilherme Piques
Rafaella Rodrigues
Fellipe Duarte Santos
Enzo Gabriel Santana da Silva

## 📄 Licença

Este projeto está sob a licença MIT.
