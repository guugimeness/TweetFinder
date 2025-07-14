
#  TweetFinder

##  Visão Geral

Implementação em **C** de um buscador de postagens do Twitter que permite consultas com operadores lógicos (`AND`, `OR`, `NOT`). O programa indexa palavras via **Tabela Hash** e realiza operações de busca em conjuntos (**Set**).
  
---

##  Funcionalidades

-  **Leitura de arquivo**: Processa postagens extraídas do Twitter no formato ID, FLAG, TEXTO.
-  **Buscador avançado**: Suporte a consultas complexas com associação de operadores lógicos (ex: `(a AND NOT(b)) OR (c AND d)`).
-  **Múltiplas consultas**: Loop contínuo até o usuário optar por sair.

---

##  Observação
Para arquivos com muitas postagens — por exemplo, **mais de 1 milhão de registros** — o processamento inicial (indexação) pode levar **mais de 1 minuto**, a depender do desempenho da máquina. Durante esse tempo, o sistema **notifica o usuário periodicamente** sobre o andamento do processo.

---

##  Como Usar
```bash

git  clone  https://github.com/guugimeness/TweetFinder
make
./main
```

Depois disso, digite sua consulta no terminal, como por exemplo:

```bash
vacina AND NOT(covid)
```

Para mais intruções de uso, digite `help` no terminal.
