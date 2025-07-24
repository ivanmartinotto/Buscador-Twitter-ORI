# Trabalho ORI - Buscador de Postagens Twitter

## 🎯 Objetivo

Este projeto tem como objetivo implementar um **buscador de postagens do Twitter**, utilizando estruturas eficientes de dados como **Tabela Hash** e **Árvore de Expressão Booleana**. O sistema lê um arquivo CSV contendo postagens e permite ao usuário realizar consultas com operadores lógicos (**AND**, **OR**, **NOT**), retornando as postagens que satisfazem os critérios.

O trabalho segue os requisitos da disciplina **Organização e Recuperação da Informação (ORI)**, conforme enunciado.

---

## 📜 Formato do Arquivo CSV
Cada linha do arquivo `corpus.csv` deve conter:
```
docID,Opiniao,Texto
```
Exemplo:
```
1,0,is so sad for my APL friend.............
2,0,I missed the New Moon trailer...
3,1,omg its already 7:30 :O
```

---

## 🚀 Como Compilar e Executar

### ✅ 1. Compile o projeto:

No terminal (Linux/macOS):
```bash
gcc main.c Parser/parser.c Indice/indice_invertido.c Indexador/indexador.c Avaliador/avaliador.c Set/set.c -o buscador
```

No Windows (MinGW):
```bash
gcc main.c Parser/parser.c Indice/indice_invertido.c Indexador/indexador.c Avaliador/avaliador.c Set/set.c -o buscador.exe
```

### ✅ 2. Execute o programa:

No mesmo diretório onde está o `corpus.csv`:
```bash
./buscador
```

### ✅ 3. Exemplo de consulta:

```
Digite sua consulta booleana (ou 'sair'): (usa AND NOT(winner)) OR (brazil AND carnaval)
```

A busca suporta:
- Operadores lógicos: `AND`, `OR`, `NOT`
- Parênteses para definir a ordem
- Palavras simples sem operadores

---
 
- Saia do programa digitando:
```
sair
```

---


