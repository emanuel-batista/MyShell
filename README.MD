# MyShell

Uma implementação simples de shell em linha de comando escrita em C que fornece funcionalidades básicas de shell, incluindo execução de comandos, navegação de diretórios e gerenciamento de processos.

## Funcionalidades

- **Execução de Comandos**: Executa programas externos e comandos do sistema
- **Comandos Internos**:
  - `cd` - Muda diretório (suporta caminhos absolutos e relativos)
  - `exit` - Sai do shell graciosamente
- **Gerenciamento de Processos**: Usa fork() e execvp() para execução de comandos
- **Prompt Interativo**: Exibe o prompt "myshell>>" para entrada do usuário
- **Tratamento de Erros**: Mensagens de erro apropriadas para comandos e operações inválidas

## Requisitos

- Compilador GCC (ou qualquer compilador C)
- Sistema operacional tipo Unix (Linux, macOS) ou Windows com ferramentas de desenvolvimento C apropriadas
- Bibliotecas C padrão

## Instalação

1. Clone ou baixe o código fonte
2. Compile o programa:
   ```bash
   gcc -o myshell myshell.c
   ```

## Uso

1. Execute o executável compilado:
   ```bash
   ./myshell
   ```

2. Você verá o prompt do shell:
   ```
   myshell>>
   ```

3. Digite comandos como em um shell regular:
   ```
   myshell>> ls
   myshell>> cd /home/user
   myshell>> pwd
   myshell>> exit
   ```

## Comandos Internos

### `cd [diretório]`
Muda o diretório de trabalho atual.
- `cd` - Muda para o diretório home
- `cd <caminho>` - Muda para o diretório especificado

### `exit`
Sai do shell e exibe uma mensagem de agradecimento.

## Como Funciona

1. **Leitura de Entrada**: Usa `fgets()` para ler entrada do usuário do stdin
2. **Análise de Comando**: Tokeniza a entrada usando `strtok()` com espaço como delimitador
3. **Processamento de Comando**:
   - Verifica comandos internos (`cd`, `exit`)
   - Para comandos externos, cria um processo filho usando `fork()`
   - Executa o comando usando `execvp()`
   - Processo pai espera a conclusão do filho usando `wait()`

## Estrutura do Código

- **Loop Principal**: Loop contínuo esperando entrada do usuário
- **Análise de Comando**: Divide a entrada em comando e argumentos
- **Manipulador de Comandos Internos**: Processa comandos internos do shell
- **Manipulador de Comandos Externos**: Faz fork do processo e executa programas externos

## Limitações

- Comprimento máximo do comando: 100 caracteres
- Argumentos máximos: 10 por comando
- Sem suporte para pipes, redirecionamento ou processos em segundo plano
- Sem histórico de comandos ou completar com tab
- Sem suporte para recursos complexos do shell como variáveis ou scripts

## Exemplo de Sessão

```
myshell>> pwd
/home/user
myshell>> ls
file1.txt  file2.txt  directory1
myshell>> cd directory1
myshell>> pwd
/home/user/directory1
myshell>> cd ..
myshell>> echo "Olá Mundo"
Olá Mundo
myshell>> exit
Obrigado por testar o MyShell!
```

## Tratamento de Erros

O shell fornece mensagens de erro para:
- Mudanças de diretório inválidas
- Comando não encontrado
- Falhas na criação de processos

## Contribuindo

Esta é uma implementação básica educacional de shell. Sinta-se livre para estendê-la com recursos adicionais como:
- Histórico de comandos
- Completar com tab
- Suporte a pipes
- Execução de processos em segundo plano
- Suporte a variáveis de ambiente
- Execução de scripts

## Licença

Este projeto é fornecido como está para fins educacionais.

## Autor

Criado como um projeto de aprendizado para entender implementação de shell e gerenciamento de processos em C.