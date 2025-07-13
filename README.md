# TweetFinder

## 📌 Visão Geral
Implementação em **C** de um buscador de postagens do Twitter que permite consultas com operadores lógicos (`AND`, `OR`, `NOT`). O programa indexa palavras via **Tabela Hash** e realiza operações de busca em conjuntos (**Set**).

---

## 🛠️ Funcionalidades
- **Leitura de arquivo**: Processa postagens extraídas do Twitter no formato ID, FLAG, TEXTO.
- **Buscador avançado**:  Suporte a consultas complexas com associação de operadores lógicos (ex: `(a AND NOT(b)) OR (c AND d)`).
- **Múltiplas consultas**: Loop contínuo até o usuário optar por sair.

---

## 📥 Como Usar

   ```bash
   git clone https://github.com/guugimeness/TweetFinder
   make
   ./main
