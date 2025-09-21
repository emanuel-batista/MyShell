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
		char *args[10];					// delimitado a quantidade de argumentos na função
		char *token;
		printf("myshell>>");
		fgets(comando, sizeof(comando), stdin);
		comando[strcspn(comando, "\n")] = '\0';

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
			{ // Se for apenas "cd"
				if (chdir(getenv("HOME")) != 0)
				{ // tenta mudar e já verifica o erro
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
				{
					// Esta parte do código SÓ executa se o comando não for encontrado.
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
