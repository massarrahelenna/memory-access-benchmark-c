## Benchmark de Acesso à Memória em C

Este guia explica como compilar e executar o programa de **Benchmark de Acesso à Memória** escrito em C (`codigo.c`).  
O objetivo do programa é comparar o desempenho entre **acessos sequenciais** e **acessos randômicos** à memória RAM.

---

## Instruções de compilação

Clone o repositório 
```bash
git clone https://github.com/massarrahelenna/TrabalhoAOC
```
---

Dentro do diretório onde o arquivo `codigo.c` está localizado, execute:

```bash
gcc -o benchmark codigo.c -lrt
```
---

Para executar o programa e iniciar os testes:
```bash
./benchmark
```
---

Após a execução, visualize os dados gravados com:
```bash
cat benchmark_results.txt
```
---