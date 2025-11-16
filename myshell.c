#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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