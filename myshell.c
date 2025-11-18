#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <sys/stat.h>

int main()
{
    int i = 1;

    while (i == 1)
    {
        // leitura do que o usuário vai passar como comando
        char comando[100];
        const char delimitador[] = " "; // delimitador para pegar espaço que o usuário digitar
        char *args[10];                 // delimitado a quantidade de argumentos na função
        char *token;
        printf("myshell>>");
        fgets(comando, sizeof(comando), stdin);
        comando[strcspn(comando, "\n")] = '\0';

        int k = strlen(comando) - 1, background = 0;

        do {
            if (comando[k] != ' ') {
                break;
            }
            k--; // decrementa o k para verificar o próximo caractere à esquerda, para verificar se tem espaço
        } while (k >= 0);

        if (comando[k] == '&') {
            background = 1;
            comando[k] = '\0'; // remove o & do comando
        }

        char * comando2 = strrchr(comando, '|'); // procura o caractere pipe no comando
        char *comando_pipe = NULL;

        if (comando2 != NULL) {
            *comando2 = '\0';
            comando_pipe = comando2 + 1; // aqui pega o comando depois do pipe, usar essa variável para o próximo comando
        }

        // ==========================================================
        //          PARSING DE REDIRECIONAMENTO > < >>

        char *redirect_file = NULL;
        int redirect_type = 0; 
        // 1 = > , 2 = >> , 3 = <

        // Primeiro verifica >> (duplo)
        char *p = strstr(comando, ">>");
        if (p != NULL) {
            redirect_type = 2;
            *p = '\0';  
            p += 2;      
        } 
        else {
            // Verifica >
            p = strchr(comando, '>');
            if (p != NULL) {
                redirect_type = 1;
                *p = '\0';
                p += 1;
            }
        }

        // Verifica <
        if (redirect_type == 0) {
            p = strchr(comando, '<');
            if (p != NULL) {
                redirect_type = 3;
                *p = '\0';
                p += 1;
            }
        }

        // Se encontrou redirecionamento, extrair nome do arquivo
        if (redirect_type != 0) {
            while (*p == ' ') p++;  // pular espaços
            redirect_file = p;      // arquivo
        }


        // printf("[DEBUG] redirect_type = %d\n", redirect_type);
        // if (redirect_file != NULL) {
        //     printf("[DEBUG] redirect_file = '%s'\n", redirect_file);
        // }
        
        // ==========================================================

        // processamento do comando, sendo necessário quebrá-lo em partes
        int j = 0;
        token = strtok(comando, delimitador); // aqui quebra o comando de acordo com nosso delimitador acima, no caso o espaço
        while (token != NULL)
        {
            args[j] = token;
            j++;
            token = strtok(NULL, delimitador);
        }
        args[j] = NULL;

        if (args[0] == NULL)
        { // se o usuário não digitar nada, volta para o início do loop principal
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            printf("Obrigado por testar o MyShell!\n");
            break;
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                if (chdir(getenv("HOME")) != 0)
                {
                    perror("myshell");
                }
            }
            else
            {
                if (chdir(args[1]) != 0)
                { // tenta mudar para o diretório e já verifica
                    perror("myshell");
                }
            }
        }
        else
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("myshell"); // se ele nao criar o fork então ele vai dar esse erro
            }
            else if (pid == 0)
            {
                //PAULO COMEÇOU AQUI
                if (redirect_type != 0)
                {
                    int fd; // File descriptor

                    if (redirect_type == 1)
                    { // > (Saída, Truncar)
                        // Abre o arquivo para escrita, cria se não existir, e trunca (apaga) se existir.
                        fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    }
                    else if (redirect_type == 2)
                    { // >> (Saída, Anexar)
                        // Abre o arquivo para escrita, cria se não existir, e anexa no final.
                        fd = open(redirect_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    }
                    else if (redirect_type == 3)
                    { // < (Entrada)
                        // Abre o arquivo apenas para leitura.
                        fd = open(redirect_file, O_RDONLY);
                    }

                    // Se open() falhar, fd será -1
                    if (fd < 0)
                    {
                        perror("myshell: open");
                        exit(EXIT_FAILURE); // Sai do processo filho se não conseguir abrir o arquivo
                    }

                    // Redireciona a entrada ou saída
                    if (redirect_type == 1 || redirect_type == 2)
                    {
                        // dup2 faz o STDOUT_FILENO (fd 1, saída padrão) apontar para o nosso arquivo (fd)
                        dup2(fd, STDOUT_FILENO);
                    }
                    else if (redirect_type == 3)
                    {
                        // dup2 faz o STDIN_FILENO (fd 0, entrada padrão) apontar para o nosso arquivo (fd)
                        dup2(fd, STDIN_FILENO);
                    }

                    // Depois que o redirecionamento é feito, o fd original pode ser fechado.
                    close(fd);
                }
                //PAULO TERMINOU AQUI
                if (execvp(args[0], args) == -1)
                { // Esta parte do código SÓ executa se o comando não for encontrado.
                    perror("myshell");
                }
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
        }
    }
    return 0;
}