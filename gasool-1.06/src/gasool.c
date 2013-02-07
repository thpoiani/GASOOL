/***************************************************************************
 *   GASOOL
 *   exibe preço dos combustíveis de postos cadastrados do Brasil.
 *
 *   Copyright (C) 2012, Marcel Luis Nishihara - Thiago Henrique Poiani
 *
 *   Este programa é software livre; você pode redistribuí-lo e/ou
 *   modificá-lo sob os termos da Licença Pública Geral GNU, conforme
 *   publicada pela Free Software Foundation; tanto a versão 2 da
 *   Licença como (a seu critério) qualquer versão mais nova.
 *
 *   Este programa é distribuído na expectativa de ser útil, mas SEM
 *   QUALQUER GARANTIA; sem mesmo a garantia implícita de
 *   COMERCIALIZAÇÃO ou de ADEQUAÇÃO A QUALQUER PROPÓSITO EM
 *   PARTICULAR. Consulte a Licença Pública Geral GNU para obter mais
 *   detalhes.
 ***************************************************************************/

/* gasool.c
 * ARQUIVO IMPLEMENTAÇÃO
 */

/* inclusão cabeçalhos do sistema */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* biblioteca para uso de rand */

/* inclusão cabeçalhos locais */
#include <curl/curl.h> /* /usr/local/include/curl */
#include "gasool.h"


// int main (int argc, char *argv[])
int
main (argc, argv)
      int argc;    // argument count contém o número de argumentos
      char **argv; // argument vector utilizado para acessar os argumentos
{
  // se estiver fora dos parâmetros
  if (argc < 1 || argc > 3)
    {
      opcaoEINVAL(); // opção inválida
      exit (EXIT_FAILURE);
    }

  if (argc == 1)
    {
      // finaliza o programa
      opcaoEINVAL(); // opção inválida
      exit (EXIT_FAILURE);
    }

  // parâmetro 2: gasool [OPÇÃO]
  if (argc == 2) // argv[1]
    {
      // $ gasool -V
      // $ gasool --version
      // a função strcmp() compara duas strings. se iguais, retorna 0
      if (!strcmp (argv[1], "-V") || !strcmp (argv[1], "--version"))
        {
	  printf ("%s \n", VERSION); // versão
          exit (EXIT_SUCCESS);
        }

      // $ gasool -h
      // $ gasool --help
      if (!strcmp (argv[1], "-h") || !strcmp (argv[1], "--help"))
        {
	  HELP (); // ajuda
          exit (EXIT_SUCCESS);
        }

      // $ gasool --license
      if (!strcmp (argv[1], "--license"))
        {
	  LICENSE (); // licença
          exit (EXIT_SUCCESS);
        }
    // $ gasool [estado]
    else
      {
        // se tamanho do parâmetro for diferente que `-uf'
        if (strlen(argv[1]) != 3)
          {
            // finaliza o programa
            opcaoEINVAL(); // opção inválida
            exit (EXIT_FAILURE);
          }

        // vetor de char site recebe o tamanho do site mais o parâmetro
        // http://precodoscombustiveis.com.br/postos/estado/
        char *site = malloc (strlen (PRECODOSCOMBUSTIVEIS) +
                             strlen (ESTADO) + strlen (argv[1]));

        // verifica erro de memória insuficiente
        if (bufferENOMEM (site))
          {
            // finaliza o programa
            exit (EXIT_FAILURE);
          }

        // vetor de char uf recebe o tamanho do parâmetro
        char *uf = malloc (strlen (argv[1]));

        // verifica erro de memória insuficiente
        if (bufferENOMEM (uf))
          {
            // libera memória alocada
            free (site);

            // finaliza o programa
            exit (EXIT_FAILURE);
          }

        // vetor de char uf recebe o parâmetro sem o caracter '-'
        int i;
        for (i = 0; i < 2; i++) // percorre todo argv[1]
          {
            if (argv[1][0] == '-') // se o primeiro caractere de argv[1] for '-'
              {
                uf[i] = argv[1][i+1];
              }
            else // passagem de parâmetro incorreta
              {
                // libera memória alocada
                free (site);
                free (uf);

                // finaliza o programa
                opcaoEINVAL(); // opção inválida
                exit (EXIT_FAILURE);
              }
          }

        // trata char[] como string
        uf[2] = '\0';

        // passar letras maiúsculas para minúsculas
        for (i = 0; i < strlen (uf); i++)
          {
            uf[i] = tolower (uf[i]);
          }

        // copia o site padrão para site
        strcpy (site, PRECODOSCOMBUSTIVEIS);

        // concatena estado padrão no final de site
        strcat (site, ESTADO);

        // concatena estado no final de site
        strcat (site, uf);

      /* carrega site */
        // variável curl é um ponteiro do tipo CURL
        CURL *curl;

        // CURLcode curl_easy_perform (CURL *curl);
        // retorna codigo de erro
        CURLcode retorno;

        // inicializa libcurl
        curl = curl_easy_init ();

        // se curl não foi iniciado, finaliza o programa
        if (!curl) // if (curl == 0)
          {
            // libera memória alocada
            free (site);
            free (uf);

            // variável retorno recebe código de erro
            retorno = curl_easy_perform (curl);

            // verifica se ocorreu algum erro durante a execução do CURL
            if (curlERRO (retorno)) // if (curlERRO (retorno) != 0)
              {
                // libera os recursos adquiridos curl_easy_init
                curl_easy_cleanup (curl);

                // finaliza o programa
                exit (EXIT_FAILURE);
              }
          }

        // variável estado é um ponteiro do tipo FILE
        FILE *estado;

        // abre um arquivo para leitura e escrita
        // se o arquivo não existe, ele é criado
        estado = fopen (HTML_ESTADO, "w+");

        // verifica se ocorreu algum erro durante o acesso ao arquivo
        if (ferrorENOENT (estado))
          {
            // libera os recursos adquiridos curl_easy_init
            curl_easy_cleanup (curl);

            // libera memória alocada
            free (site);
            free (uf);

            // finaliza o programa
            exit (EXIT_FAILURE);
          }

        // executa abertura e escrita do site no arquivo
        // CURLcode curl_easy_setopt (CURL *curl, CURLoption option, parâmetro);
        curl_easy_setopt (curl, CURLOPT_URL, site);
        curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt (curl, CURLOPT_WRITEDATA, estado);

        /* CURLOPT_URL: carrega site definido
           CURLOPT_WRITEFUNCTION: executa a função
           CURLOPT_WRITEDATA: escreve no arquivo, como fwrite() */

        /* com NULL no CURLOPT_WRITEFUNCTION, executa uma função padrão que
           retorna a escrita do site dentro do file dado no CURLOPT_WRITEDATA */

        // libera memória alocada
        free (site);

        // variavel retorno recebe código de erro
        retorno = curl_easy_perform (curl);

        // fecha arquivo
        fclose (estado);

        // verifica se ocorreu algum erro durante a execução do CURL
        // ou se o parâmetro digitado é um estado válido
        if ((curlERRO (retorno) != 0) || ((verificacao_estado (estado) != 0)))
          {
            // libera os recursos adquiridos curl_easy_init
            curl_easy_cleanup (curl);

            // libera memória alocada
            free (uf);

            // apaga arquivos temporários
            remove (HTML_ESTADO);

            // finaliza o programa
            exit (EXIT_FAILURE);
          }

        // executa a função para escrever as cidades cadasatradas desse estado
        cidades_estado (estado, uf);

        // libera memória
        free (uf);

        // libera os recursos adquiridos curl_easy_init
        curl_easy_cleanup (curl);
      }
    }

  // $ gasool [OPÇÃO] [OPÇÃO]
  if (argc == 3)
    {
      // variável x representa o estado
      // variável y representa a cidade
      int x, y;
      x = y = 0;

      // verifica a ordem os argumentos passados pelo usuário
      if (strlen(argv[1]) == 3) // gasool -uf [CIDADE]
        {
          x = 1;
          y = 2;
        }
      else
        {
          if (strlen(argv[2]) == 3) // gasool [CIDADE] -uf
            {
              x = 2;
              y = 1;
            }
          else
            {
              // finaliza o programa
              opcaoEINVAL(); // opção inválida
              exit (EXIT_FAILURE);
            }
        }

      // vetor de char site recebe o tamanho do site mais o parâmetro
      // http://precodoscombustiveis.com.br/postos/estado/
      char *site = malloc (strlen (PRECODOSCOMBUSTIVEIS) +
                           strlen (ESTADO) + strlen (argv[x]));

      // verifica erro de memória insuficiente
      if (bufferENOMEM (site))
        {
          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // vetor de char uf recebe o tamanho do parâmetro
      char *uf = malloc (strlen (argv[x]));

      // verifica erro de memória insuficiente
      if (bufferENOMEM (uf))
        {
          // libera a memória
          free (site);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // vetor de char uf recebe o parâmetro sem o caracter '-'
      int i;
      for (i = 0; i < 2; i++) // percorre todo argv[1]
        {
          if (argv[x][0] == '-') // se o primeiro caractere de argv[1] for '-'
            {
              uf[i] = argv[x][i+1]; // uf recebe argv[1] sem o caracter '-'
            }
          else
            {
              // libera memória alocada
              free (site);
              free (uf);

              // finaliza o programa
              opcaoEINVAL(); // opção inválida
              exit (EXIT_FAILURE);
            }
        }

      // trata char[] como string
      uf[2] = '\0';

      // passar letras maiúsculas para minúsculas
      for (i = 0; i < strlen (uf); i++)
        {
          uf[i] = tolower (uf[i]);
        }

      // copia o site padrão para site
      strcpy (site, PRECODOSCOMBUSTIVEIS);

      // concatena estado padrão no final de site
      strcat (site, ESTADO);

      // concatena estado encontrado no final de site
      strcat (site, uf);

    /* carrega site */
      // variável curl é um ponteiro do tipo CURL
      CURL *curl;

      // CURLcode curl_easy_perform (CURL *curl);
      // retorna codigo de erro
      CURLcode retorno;

      // inicializa libcurl
      curl = curl_easy_init ();

      // se curl não foi iniciado, finaliza o programa
      if (!curl) // if (curl == 0)
        {
          // libera memória alocada
          free (site);
          free (uf);

          // variável retorno recebe código de erro
          retorno = curl_easy_perform (curl);

          // verifica se ocorreu algum erro durante a execução do CURL
          if (curlERRO (retorno)) // if (curlERRO (retorno) != 0)
            {
              // libera os recursos adquiridos curl_easy_init
              curl_easy_cleanup (curl);

              // finaliza o programa
              exit (EXIT_FAILURE);
            }
        }

      // variável estado é um ponteiro do tipo FILE
      FILE *estado;

      // abre um arquivo para leitura e escrita
      // se o arquivo não existe, ele é criado
      estado = fopen (HTML_ESTADO, "w+");

      // verifica se ocorreu algum erro durante o acesso ao arquivo
      if (ferrorENOENT (estado))
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // libera memória alocada
          free (site);
          free (uf);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // executa abertura e escrita do site no arquivo
      // CURLcode curl_easy_setopt (CURL *curl, CURLoption option, parâmetro);
      curl_easy_setopt (curl, CURLOPT_URL, site);
      curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, NULL);
      curl_easy_setopt (curl, CURLOPT_WRITEDATA, estado);

      /* CURLOPT_URL: carrega site definido
         CURLOPT_WRITEFUNCTION: executa a função
         CURLOPT_WRITEDATA: escreve no arquivo, como fwrite() */

      /* com NULL no CURLOPT_WRITEFUNCTION, executa uma função padrão que
         retorna a escrita do site dentro do file dado no CURLOPT_WRITEDATA */

      // libera memória alocada
      free (site);
      free (uf);

      // variavel retorno recebe código de erro
      retorno = curl_easy_perform (curl);

      // fecha arquivo
      fclose (estado);

      // verifica se ocorreu algum erro durante a execução do CURL
      // ou se o parâmetro digitado é um estado válido
      if ((curlERRO (retorno) != 0) || ((verificacao_estado (estado) != 0)))
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // apaga arquivos temporários
          remove (HTML_ESTADO);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }


      // vetor de char city recebe o tamanho do parâmetro
      char *city = malloc (strlen (argv[y]));

      // verifica erro de memória insuficiente
      if (bufferENOMEM (city))
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // apaga arquivos temporários
          remove (HTML_ESTADO);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // retirada do caractere '-' antes do parâmetro
      for (i = 0; i < strlen (argv[y]); i++) // percorre todo argv[1]
        {
          if (argv[y][0] == '-') // se o primeiro caractere de argv[1] for '-'
            {
              city[i] = argv[y][i+1]; // city recebe argv[1] se o caractere '-'
            }
          else
            {
              // libera os recursos adquiridos curl_easy_init
              curl_easy_cleanup (curl);

              // apaga arquivos temporários
              remove (HTML_ESTADO);

              // libera memória alocada
              free (city);

              // finaliza o programa
              opcaoEINVAL(); // opção inválida
              exit (EXIT_FAILURE);
            }
        }

      // passar letras maiúsculas para minúsculas
      for (i = 0; i < strlen (city); i++)
        {
          city[i] = tolower (city[i]);
        }

      // vetor de char site_parametro recebe o retorno da função
      // a função verificacao_cidade retorna a url da cidade
      char* site_parametro = verificacao_cidade (estado, city);

      // realocação
      site = malloc (strlen (PRECODOSCOMBUSTIVEIS) +
                     strlen (site_parametro) + 1);

      // libera a memória
      free (city);

      // verifica erro de memória insuficiente
      if (bufferENOMEM (site))
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // apaga arquivos temporários
          remove (HTML_ESTADO);

         // finaliza o programa
         exit (EXIT_FAILURE);
        }

      // copia o site padrão para site
      strcpy (site, PRECODOSCOMBUSTIVEIS);

      // concatena a cidade no final de site
      strcat (site, site_parametro);

      // variável estado é um ponteiro do tipo FILE
      FILE *cidade;

      // abre um arquivo para leitura e escrita
      // se o arquivo não existe, ele é criado
      cidade = fopen (HTML_CIDADE, "w+");

      // verifica se ocorreu algum erro durante o acesso ao arquivo
      if (ferrorENOENT (cidade))
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // apaga arquivos temporários
          remove (HTML_ESTADO);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // executa abertura e escrita do site no arquivo
      // CURLcode curl_easy_setopt (CURL *curl, CURLoption option, parâmetro);
      curl_easy_setopt (curl, CURLOPT_URL, site);
      curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, NULL);
      curl_easy_setopt (curl, CURLOPT_WRITEDATA, cidade);

      /* CURLOPT_URL: carrega site definido
         CURLOPT_WRITEFUNCTION: executa a função
         CURLOPT_WRITEDATA: escreve no arquivo, como fwrite() */

      /* com NULL no CURLOPT_WRITEFUNCTION, executa uma função padrão que
         retorna a escrita do site dentro do file dado no CURLOPT_WRITEDATA */

      // variavel retorno recebe código de erro
      retorno = curl_easy_perform (curl);

      // fecha arquivo
      fclose (cidade);

      // verifica se ocorreu algum erro durante a execução do CURL
      if (curlERRO (retorno)) // if (curlERRO (retorno) != 0)
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // libera memória alocada
          free (site);

          // apaga arquivos temporários
          remove (HTML_ESTADO);
          remove (HTML_CIDADE);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // variável pagina_max recebe o retorno da função
      // a função paginas retorna o número de páginas do da cidade
      int pagina_max = paginas (cidade);

      // gera número aleatório de 1 até pagina_max
      srand (time (NULL));
      pagina_max = rand() % pagina_max + 1;

      // variável tamanho_pagina recebe tamanho strlen de pagina_max
      int tamanho_pagina = 0;
      if (pagina_max > 9)
        {
          tamanho_pagina = 2;
        }
      else
        {
          tamanho_pagina = 1;
        }

      // vetor de char num_pagina recebe o valor de pagina_max
      char *num_pagina = malloc (tamanho_pagina + 1);

      // sprintf alternativa para itoa - converte int para string
      sprintf (num_pagina, "%d", pagina_max);

      // tratar char[] como string
      num_pagina[tamanho_pagina] = '\0';

      // realocação
      site = malloc (strlen (PRECODOSCOMBUSTIVEIS) +
                     strlen (site_parametro) + strlen (Pagina) +
                     tamanho_pagina + 1);

      // verifica erro de memória insuficiente
      if (bufferENOMEM (site))
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // libera memória alocada
          free (num_pagina);

          // apaga arquivos temporários
          remove (HTML_CIDADE);
          remove (HTML_ESTADO);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // copia o site padrão para site
      strcpy (site, PRECODOSCOMBUSTIVEIS);

      // concatena a cidade no final de site
      strcat (site, site_parametro);

      // concatena /pagina/ no final de site
      strcat (site, Pagina);

      // concatena número aleatório de páginas no final de site
      strcat (site, num_pagina);

      // sobrescreve cidade.tmp
      cidade = fopen (HTML_CIDADE, "w+");

      // verifica se ocorreu algum erro durante o acesso ao arquivo
      if (ferrorENOENT (cidade))
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // apaga arquivos temporários
          remove (HTML_ESTADO);
          remove (HTML_CIDADE);

          // libera a memória
          free (site);
          free (num_pagina);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // executa abertura e escrita do site no arquivo
      // CURLcode curl_easy_setopt (CURL *curl, CURLoption option, parâmetro);
      curl_easy_setopt (curl, CURLOPT_URL, site);
      curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, NULL);
      curl_easy_setopt (curl, CURLOPT_WRITEDATA, cidade);

      /* CURLOPT_URL: carrega site definido
         CURLOPT_WRITEFUNCTION: executa a função
         CURLOPT_WRITEDATA: escreve no arquivo, como fwrite() */

      /* com NULL no CURLOPT_WRITEFUNCTION, executa uma função padrão que
         retorna a escrita do site dentro do file dado no CURLOPT_WRITEDATA */

      // libera memória alocada
      free (site);
      free (num_pagina);

      // variavel retorno recebe código de erro
      retorno = curl_easy_perform (curl);

      // fecha arquivo
      fclose (cidade);

      // verifica se ocorreu algum erro durante a execução do CURL
      if (curlERRO (retorno)) // if (curlERRO (retorno) != 0)
        {
          // libera os recursos adquiridos curl_easy_init
          curl_easy_cleanup (curl);

          // apaga arquivos temporários
          remove (HTML_ESTADO);
          remove (HTML_CIDADE);

          // finaliza o programa
          exit (EXIT_FAILURE);
        }

      // executa a função para escrever os postos e seus preços de combustíveis
      postos (cidade, pagina_max);

      // libera os recursos adquiridos curl_easy_init
      curl_easy_cleanup (curl);
    }

  // apaga arquivos temporários
  remove (HTML_ESTADO);
  remove (HTML_CIDADE);

  exit (EXIT_SUCCESS);
}


/**
* \brief Mensagem de ajuda sobre o uso do programa
*/
void
HELP (void)
{
  printf ("GASOOL mostra os preços dos combustíveis postos cadastrados por\n");
  printf ("todo o território brasileiro. Os postos estão cadastrados no site");
  printf ("\nhttp://www.precodoscombustiveis.com.br\n\n");

  printf ("  Parâmetros\n");
  printf ("  ==========\n\n");

  printf ("  Exibe versão\n");
  printf ("   gasool -V\n");
  printf ("   gasool --version\n\n");

  printf ("  Exibe mensagem de ajuda descrevendo os comandos\n");
  printf ("   gasool -h\n");
  printf ("   gasool --help\n\n");

  printf ("  Exibe Licença Pública Geral GNU\n");
  printf ("   gasool --license\n\n");

  printf ("  Retorna as cidades cadastradas do estado procurado\n");
  printf ("   gasool [-ESTADO]\n");
  printf ("   Exemplo: gasool -ac\n");
  printf ("   [-cruzeiro-do-sul] [-sena-madureira] [-tarauaca] [-rio-branco]");
  printf ("\n   [-senador-guiomard] [-xapuri]\n\n");

  printf ("  Retorna os postos de combustíveis");
  printf (" (obrigatoriamente precisa do [-ESTADO])\n");
  printf ("   gasool [-ESTADO] [-CIDADE]\n");
  printf ("   Exemplo: gasool -ce -fortaleza\n");
  printf ("   POSTO 01: Super Com�rcio de Combust�veis Ltda\n");
  printf ("   Endereço: Travessa Paris, 1133 Qd. 29\n");
  printf ("   Gasolina 2.459  |   Álcool 2.070  |   Diesel 1.999\n\n");

  printf ("  Erros\n");
  printf ("  =====\n\n");

  printf ("   0 - não ocorreu problemas\n");
  printf ("   1 - erro genérico, gerado por falta de memória");
  printf (" ou falha na criação de arquivo\n");
  printf ("   CURLcode(x) - erro de execução da libcurl");
  printf (" (veja libcurl-errors(3))\n\n");

  printf ("Reporte bugs e defeitos para gasool.anp@live.com\n");
  printf ("Copyright (C) 2012, Marcel Luis Nishihara - Thiago Henrique Poiani");
  printf ("\n");
}

/**
* \brief Termo de licença usado no programa
*
* Este programa é software livre sob os termos da
* Licença Pública Geral GNU tanto a versão 2
* como qualquer versão mais nova.
*/
void
LICENSE (void)
{
  printf ("%s%s%s\n", "GASOOL ", VERSION, ", Copyright (C) 2012");
  printf ("Marcel Luis Nishihara - Thiago Henrique Poiani\n");
  printf ("O GASOOL não possui QUALQUER GARANTIA. Ele é software livre\n");
  printf ("e você está convidado a redistribuí-lo sob certas condições.\n");
}

/**
* \brief Erro gerado por opção inválida
*/
void
opcaoEINVAL (void)
{
  printf ("%s \n", "gasool: Opção inválida"); // EINVAL: Invalid argument
  printf ("%s \n", "Experimente 'gasool --help' para mais informações.");
}

/**
* \brief Erro da libcurl
*
* \param Número do erro retornado da função curl_easy_perform(curl)
*
* A função verifica erros durante a execução da libcurl,
* retornando o número de erro e orientando o usuário
* a acessar 'man libcurl-errors' para mais detalhes.
*/
int
curlERRO (numERRO)
          int numERRO;
{
  if (numERRO) // if ((retorno = curl_easy_perform(curl)) != 0)
    {
      printf ("gasool: Erro ao executar\n");
      printf ("%s (%d). \n", "CURLcode", numERRO);
      printf ("Experimente 'man libcurl-errors' para mais");
      printf (" informações sobre o erro acima.\n");
      return (EXIT_FAILURE);
    }
  return (EXIT_SUCCESS);
}

/**
* \brief Erro da memória
*
* \param Vetor de caracteres com espaço alocado na memória
*
* A função verifica erros durante a alocação de memória,
* com a finalidade de encerrar o programa caso gere
* falha de memória insuficiente.
*/
int
bufferENOMEM (buffer)
             char* buffer;
{
  // se malloc() retornar ponteiro nulo (memória suficiente)
  if (buffer == NULL)
    {
      printf ("gasool: Erro ao executar\n"); // ENOMEM: Not enough space
      return (EXIT_FAILURE);
    }
  return (EXIT_SUCCESS);
}

/**
* \brief Erro da acesso ao arquivo
*
* \param Ponteiro do tipo FILE para o arquivo
*
* A função verifica erros durante o acesso ao arquivo,
* com a finalidade de encerrar o programa caso gere
* falha de uso de arquivo.
*/
int
ferrorENOENT (stream)
             FILE* stream;
{
  if (ferror (stream)) // if (ferror (stream) != 0)
    {
      printf ("gasool: Erro ao executar\n");
      fclose (stream);
      return (EXIT_FAILURE);
    }
  return (EXIT_SUCCESS);
}

/**
* \brief Procura posição da expressão
*
* \param Ponteiro do tipo FILE para o arquivo
* \param Expressão procurada
* \param Posição dentro do arquivo
*
* A função procura, dentro do file, a posição da
* expressão procurada, retornando a posição se
* encontrada, se não retorna -1.
*/
int
FileSearch (file, SearchString, ponto_de_inicio)
            FILE* file;
            char* SearchString;
            int ponto_de_inicio;
{
  // verifica se ocorreu algum erro durante o acesso ao arquivo
  if (ferrorENOENT (file))
    {
      exit (EXIT_FAILURE);
    }

  // variável FileSize recebe tamanho do arquivo
  int FileSize = 0;
  // posiciona o cursor no final do arquivo
  fseek (file, 0, SEEK_END);
  // variável FileSize recebe a posição atual
  // ou seja, o tamanho do arquivo
  FileSize = ftell (file);

  // a variável tamanho recebe o comprimento da string SearchString
  // tamanho necessário para buffer
  int tamanho = strlen (SearchString);

  // aloca memória para char[tamanho]
  char* buffer = malloc (tamanho);

  // verifica erro de memória insuficiente
  if (bufferENOMEM (buffer))
    {
      // libera memória alocada
      free (buffer);

      // fecha o arquivo e finaliza o programa
      fclose (file);
      exit (EXIT_FAILURE);
    }

  // procura no arquivo a posição da string procurada
  while (ponto_de_inicio < FileSize - tamanho)
    {
      // procura no arquivo o que está na posição ponto_de_inicio
      fseek (file, ponto_de_inicio, SEEK_SET);

      // buffer recebe o que estiver a frente da posição
      // recebe char [tamanho]
      fread (buffer, 1, tamanho, file);

      // compara a área de memória apontada para buffer com a para SearchString
      // com tamanho bytes. retorna 0 se iguais
      // ou seja, se encontrar o que procura
      if (!memcmp (buffer, SearchString, tamanho))
        {
          // libera memória alocada
          free (buffer);

          // retorna a posição atual
          return (ponto_de_inicio);
        }
      // se não incrementa a posição e verifica novamente
      ponto_de_inicio++;
    }

  // se a string não for encontrada no arquivo

  //libera o buffer
  free (buffer);

  // retorna -1
  return (-1);
}

/**
* \brief Estado válido
*
* \param Ponteiro do tipo FILE para o arquivo
*
* A função procura, dentro do file, o estado
* informado no argumento e verifica validade.
*/
int
verificacao_estado (arquivo)
                   FILE* arquivo;
{
  // abre o arquivo somente para leitura
  arquivo = fopen (HTML_ESTADO, "r");

  // verifica se ocorreu algum erro durante o acesso ao arquivo
  if (ferrorENOENT (arquivo))
    {
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  // variável invalido recebe a posição de onde está <h1>Ops!</h1> no arquivo
  int invalido = 0;
  invalido = FileSearch (arquivo, estado_invalido, 0);

  // se encontrar a tag, retorna sua posição
  if (invalido != -1)
    {
      opcaoEINVAL(); // opção inválida
      fclose (arquivo);
      return (EXIT_FAILURE);
    }

  fclose (arquivo);
  return (EXIT_SUCCESS);
}

/**
* \brief Cidades cadastradas
*
* \param Ponteiro do tipo FILE para o arquivo
* \param Estado informada no argumento
*
* A função procura, dentro do file, as cidades
* cadastradas no estado válido e informado
* por argumento
*/
int
cidades_estado (arquivo, UF)
               FILE* arquivo;
               char* UF;
{
  // abre o arquivo somente para leitura
  arquivo = fopen (HTML_ESTADO, "r");

  // verifica se ocorreu algum erro durante o acesso ao arquivo
  if (ferrorENOENT (arquivo))
    {
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  int inicio_cidade, fim_cidade, tamanho_cidade;
  inicio_cidade = fim_cidade = tamanho_cidade = 0;

  int posicao, posicao_final;
  posicao = posicao_final = 0;

  int contador = 0;

  // variável posicao recebe o valor mais próximo das cidades
  posicao = FileSearch (arquivo, inicioCidades, posicao);

  // variável posicao_final recebe um valor posterior ao das cidades
  posicao_final = FileSearch (arquivo, finalCidades, posicao);

  // vetor de char uf recebe o estado por parâmetro, que possui char[3]
  char* uf = malloc (4);

  // copia estado passado por parâmetro para char[] uf e concatena '/'
  strcpy (uf, UF);
  strcat (uf, inicioComum);

  for ( ;; )
    {
    /* tratamento de tag com o nome da cidade */
      // variável inicio_cidade recebe a posição de onde está uf/ no arquivo
      inicio_cidade = FileSearch (arquivo, uf, posicao);

      // se não encontrar a tag, significa que o arquivo foi completamente lido
      if (inicio_cidade == -1)
        {
          break;
        }

      // fim_cidade recebe a posição de onde está "> após a posição de uf/
      fim_cidade = FileSearch (arquivo, finalComum, inicio_cidade);

      // tamanho_cidade recebe o tamanho da string contida entre uf/ e ">
      // ou seja, o tamanho do nome da cidade
      // tamanho necessário para alocar memória para char[]
      tamanho_cidade = fim_cidade - inicio_cidade - strlen (uf);
      char* buffer = malloc (tamanho_cidade + 1);

      // verifica erro de memória insuficiente
      if (bufferENOMEM (buffer))
        {
          fclose (arquivo);
          remove (HTML_ESTADO);
          exit (EXIT_FAILURE);
        }

      // procura e posiciona, no arquivo, o cursor após a posição de uf/
      // ou seja, posiciona antes do nome da cidade
      fseek (arquivo, inicio_cidade + strlen (uf), SEEK_SET);

      // char[tamanho_cidade] buffer recebe o nome da cidade
      fread (buffer, 1, tamanho_cidade, arquivo);

      // trata vetor de char como string
      buffer[tamanho_cidade] = '\0';

      // incrementa contador até chegar ao quarto termo, que faz quebra de linha
      contador++;
      contador % 4 ? printf ("[-%s] ", buffer) : printf ("[-%s]\n", buffer);

      // libera memória alocada
      free (buffer);

      // variável posicao recebe a posição atual
      posicao = ftell (arquivo);

      // se a posição atual for maior que a posição limite, para a execução
      if (posicao > posicao_final)
        {
          break;
        }

    /* tratamento de tag com o nome da cidade */
    }
  // pula linha
  if (contador % 4 != 0)
    {
      putchar ('\n');
    }

  // libera memória alocada
  free (uf);

  // fecha o arquivo
  fclose (arquivo);

  return (EXIT_SUCCESS);
}

/**
* \brief Cidade válida
*
* \param Ponteiro do tipo FILE para o arquivo
* \param Cidade informada no argumento
*
* A função procura, dentro do file, a cidade
* informada no argumento e verifica validade.
*/
char*
verificacao_cidade (arquivo, city)
                   FILE* arquivo;
                   char* city;
{
  // abre o arquivo somente para leitura
  arquivo = fopen (HTML_ESTADO, "r");

  // verifica se ocorreu algum erro durante o acesso ao arquivo
  if (ferrorENOENT (arquivo))
    {
      remove (HTML_ESTADO);
      remove (HTML_CIDADE);
      exit (EXIT_FAILURE);
    }

  int cidade = 0;

  // variável cidade recebe a posição de onde está o nome da cidade no arquivo
  cidade = FileSearch (arquivo, city, 0);

  // se não encontrar a cidade, retorna -1
  if (cidade == -1)
    {
      opcaoEINVAL(); // opção inválida
      fclose (arquivo);
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  // variáveis para verificação de o nome da cidade foi digitado completo
  int inicio_cidade, final_cidade;

/* verificação anterior ao nome da cidade */
  // inicio_cidade recebe posição anterior ao nome da cidade, que deve conter /
  inicio_cidade = cidade - 1;

  // aloca memória para char[1]
  char *buffer = malloc (1);

  // verifica erro de memória insuficiente
  if (bufferENOMEM (buffer))
    {
      fclose (arquivo);
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  // procura no arquivo o que está depois da posição de inicio_cidade
  fseek (arquivo, inicio_cidade, SEEK_SET);

  // buffer recebe o que estiver /
  fread (buffer, 1, 1, arquivo);

  // compara a área de memória apontada para buffer com a  para inicioComum (/)
  // retorna 0 se iguais. se diferentes, finaliza o programa
  if (memcmp (buffer, inicioComum, 1))
    {
      // libera memória alocada
      free (buffer);

      // como não encontrou, informa erro
      opcaoEINVAL(); // opção inválida
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  // libera memória alocada
  free (buffer);
/* verificação anterior ao nome da cidade */

/* verificação posterior ao nome da cidade */
  // final_cidade recebe posição posterior ao nome da cidade, que deve conter ">
  final_cidade = cidade + strlen (city);

  // aloca memória para char[2]
  buffer = malloc (2);

  // verifica erro de memória insuficiente
  if (bufferENOMEM (buffer))
    {
      fclose (arquivo);
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  // procura no arquivo o que está depois da posição de cidade, o char ">
  fseek (arquivo, final_cidade, SEEK_SET);

  // buffer recebe o que estiver após ">
  fread (buffer, 1, 2, arquivo);

  // compara a área de memória apontada para buffer com a para finalComum (">)
  // retorna 0 se iguais. se diferentes, finaliza o programa
  if (memcmp (buffer, finalComum, 2))
    {
      // libera memória alocada
      free (buffer);

      // fecha o arquivo
      fclose (arquivo);

      // como não encontrou, informa erro
      opcaoEINVAL(); // opção inválida
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  // libera memória alocada
  free (buffer);
/* verificação posterior ao nome da cidade */

/* busca URL da cidade */
  // <a href="/postos/cidade/xxxx/uf/city">
  int inicio_href, url, tamanho;
  inicio_href = url = tamanho = 0;

  // encontra a maior posição de inicio_href antes da posição da cidade
  // ou seja, o início da href
  while (inicio_href < cidade)
    {
      // variável url recebe a posição de <td><a href=" mais o tamanho da tag
      url = inicio_href + strlen (Taghref); //posiciona na URL

      // inicio_href recebe a posição de onde está <td><a href=" após posição
      inicio_href = FileSearch (arquivo, Taghref, url);

      // quando a posição de inicio_href for mais que da cidade, para!
      if ((inicio_href > cidade) || (inicio_href == -1))
        {
          break;
        }
    }

  // variável tamanho recebe o tamanho da URL contida entre
  // <td><a href=" e o nome da cidade: /postos/cidade/xxxx/uf/
  // tamanho necessário para alocar memória para char[]
  tamanho = cidade - url;

  // aloca memória para char[tamanho]
  buffer = malloc (tamanho + 1);

  // verifica erro de memória insuficiente
  if (bufferENOMEM (buffer))
    {
      fclose (arquivo);
      remove (HTML_ESTADO);
      exit (EXIT_FAILURE);
    }

  // procura e posiciona, no arquivo, o cursor após a posição da URL
  fseek (arquivo, url, SEEK_SET);

  // char[tamanho] buffer recebe a URL
  fread (buffer, 1, tamanho, arquivo);
  buffer[tamanho] = '\0';

  // vetor de char site recebe o tamanho de buffer mais o do nome da cidade
  char *site = malloc (tamanho + strlen (city) + 1);

  // copia a url para char site
  strcpy (site, buffer);

  // concatena a cidade no final da url que será retornada para main
  strcat (site, city);

  // libera memória alocada
  free (buffer);
/* busca URL da cidade */

  // fecha o arquivo
  fclose (arquivo);

  // retorna site para acessar o preço dos combustíveis da cidade
  return (site);
}

/**
* \brief Páginas
*
* \param Ponteiro do tipo FILE para o arquivo
*
* A função procura, dentro do file, o número
* de páginas da cidade, a fim de gerar acesso
* à uma página aleatória.
*/
int
paginas (arquivo)
        FILE* arquivo;
{
  // abre o arquivo somente para leitura
  arquivo = fopen (HTML_CIDADE, "r");

  // verifica se ocorreu algum erro durante o acesso ao arquivo
  if (ferrorENOENT (arquivo))
    {
      remove (HTML_ESTADO);
      remove (HTML_CIDADE);
      exit (EXIT_FAILURE);
    }

  int inicio_pagina, fim_pagina, pagina, x;
  inicio_pagina, fim_pagina = pagina = x = 0;

  int tamanho_pagina, posicao_pagina;
  tamanho_pagina = posicao_pagina = 0;

  char* buffer;

  int i;
  for (i = 0 ; i < 13 ; i++) // número máximo de páginas
    {
      /* <a href="/postos/cidade/xxxx/uf/cidade/pagina/yy?ordem=gasolina"> */

      // inicio_pagina recebe a posição de onde está /pagina/ no arquivo
      inicio_pagina = FileSearch (arquivo, Pagina, posicao_pagina);

      // se inicio_pagina receber -1, a cidade não possui mais de 20 postos
      if (inicio_pagina == -1)
        {
          pagina = 1;
          break;
        }

      // fim_pagina recebe a posição de onde está ? após /pagina/ no arquivo
      fim_pagina = FileSearch (arquivo, "?", inicio_pagina);

      // tamanho_pagina recebe o tamanho da string contida entre /pagina/ e ?
      // ou seja, a pagina
      tamanho_pagina = fim_pagina - inicio_pagina - strlen (Pagina);
      buffer = malloc (tamanho_pagina + 1);

      // verifica erro de memória insuficiente
      if (bufferENOMEM (buffer))
        {
          fclose (arquivo);
          remove (HTML_ESTADO);
          remove (HTML_CIDADE);
          exit (EXIT_FAILURE);
        }

      // procura e posiciona, no arquivo, o cursor após posição de inicio_pagina
      // ou seja, posiciona antes do número da páginaa
      fseek (arquivo, inicio_pagina + strlen (Pagina), SEEK_SET);

      // char[] buffer recebe o número da página
      fread (buffer, 1, tamanho_pagina, arquivo);

      // variável posicao_pagina recebe posição atual
      posicao_pagina = ftell (arquivo);

      // tratar buffer como string
      buffer[tamanho_pagina] = '\0';

      // variável x recebe converção da string para inteiro
      x = atoi (buffer);

      // libera a memória
      free (buffer);

      // variável pagina recebe valor convertido da string para inteiro
      if (x > pagina)
        {
          pagina = x; // número máximo de páginas dos postos
        }
      else
        {
          break;
        }
   }

  return (pagina);
}

/**
* \brief Postos de combustíveis
*
* \param Ponteiro do tipo FILE para o arquivo
*
* A função procura, dentro do file, o nome e
* endereço dos postos de combustíveis e o
* preço de seus combustíveis.
*/
int
postos (arquivo, pagina)
      FILE* arquivo;
      int pagina;
{
  // abre o arquivo somente para leitura
  arquivo = fopen (HTML_CIDADE, "r");

  // verifica se ocorreu algum erro durante o acesso ao arquivo
  if (ferrorENOENT (arquivo))
    {
      remove (HTML_ESTADO);
      remove (HTML_CIDADE);
      exit (EXIT_FAILURE);
    }

  int inicio_posto, inicio_href, inicio_endereco;
  inicio_posto = inicio_href = inicio_endereco = 0;

  int fim_posto, fim_href, fim_endereco;
  fim_posto = fim_href = fim_endereco = 0;

  int tamanho_posto, tamanho_endereco;
  tamanho_posto = tamanho_endereco = 0;

  int contador = 0;
  contador = 20 * pagina - 20;

  int posicao = 0;

  char* buffer;

  for ( ;; )
    {
    /* tratamento de <td class="posto"> */
      // inicio_posto recebe posição de onde está <td class="posto"> no arquivo
      inicio_posto = FileSearch (arquivo, inicioTagTD, posicao);

      // se não encontrar a tag, significa que o arquivo foi completamente lido
      if (inicio_posto == -1)
        {
          break;
        }

    /* tratamento de <a href> */
      // inicio_href recebe a posição de onde está <a href="/posto/
      // após a posição de <td class="posto">
      inicio_href = FileSearch (arquivo, inicioTagHref, inicio_posto);

      // fim_href recebe a posição de onde está >
      // após a posição de <a href="/posto/
      fim_href = FileSearch (arquivo, finalComum, inicio_href);
    /* tratamento de <a href> */

      // fim_posto recebe a posição de onde está </a></td>
      // após a posição de <td class="posto">
      fim_posto = FileSearch (arquivo, finalTagTD, inicio_posto);

      // tamanho_posto recebe o tamanho da string
      // contida entre <a href="/posto/..."> e </a></td>
      // ou seja, o tamanho do nome do posto
      // tamanho necessário para alocar memória para char[]
      tamanho_posto = fim_posto - fim_href - strlen (finalComum);
      buffer = malloc (tamanho_posto + 1);

      // verifica erro de memória insuficiente
      if (bufferENOMEM (buffer))
        {
          fclose (arquivo);
          remove (HTML_ESTADO);
          remove (HTML_CIDADE);
          exit (EXIT_FAILURE);
        }

      // procura e posiciona, no arquivo, o cursor após a posição de fim_href
      // ou seja, posiciona antes do nome do posto
      fseek (arquivo, fim_href + strlen (finalComum), SEEK_SET);

      // char[tamanho_posto] buffer recebe o nome do posto
      fread (buffer, 1, tamanho_posto, arquivo);

      // trata vetor de char como string e imprime
      buffer[tamanho_posto] = '\0';
      printf ("POSTO %02d: %s \n", ++contador, buffer);

      // libera memória alocada
      free (buffer);
    /* tratamento de <td class="posto"> */

    /* tratamento de <td class="endereco"> */
      // inicio_endereco recebe a posição de onde está <td class="endereco">
      // após a posição de </a></td>
      inicio_endereco = FileSearch (arquivo, inicioTagEndereco, fim_posto);

      // fim_endereco recebe a posição de onde está <br/>
      // após a posição de <td class="endereco">
      fim_endereco = FileSearch (arquivo, finalTagEndereco, inicio_endereco);

      // tamanho_endereco recebe o tamanho da string
      // contida entre <td class="endereco"> e <br/>
      // ou seja, o tamanho do endereço do posto
      // tamanho necessário para alocar memória para char[]
      tamanho_endereco = fim_endereco -
                         inicio_endereco - strlen (inicioTagEndereco);

      // aloca memória para char[tamanho_endereco]
      buffer = malloc (tamanho_endereco + 1);

      // verifica erro de memória insuficiente
      if (bufferENOMEM (buffer))
        {
          fclose (arquivo);
          remove (HTML_ESTADO);
          remove (HTML_CIDADE);
          exit (EXIT_FAILURE);
        }

      // procura e posiciona, no arquivo,
      // o cursor após a posição de inicio_endereco
      // ou seja, posiciona antes do endereço do posto
      fseek (arquivo, inicio_endereco + strlen (inicioTagEndereco), SEEK_SET);

      // char[tamanho_endereco] buffer recebe o endereço do posto
      fread (buffer, 1, tamanho_endereco, arquivo);

      // trata char[] como string e imprime
      buffer[tamanho_endereco] = '\0';
      printf ("Endereço: %s \n", buffer);

      // libera memória alocada
      free (buffer);
    /* tratamento de <td class="endereco"> */

      // variável posição armazena posição atual do cursor no arquivo
      posicao = ftell(arquivo);

    /* tratamento de <td class="combustivel preco"> */
      printf ("Gasolina ");
      preco (arquivo, gasolina, fim_endereco);
      printf ("  |  ");

      printf (" Álcool ");
      preco (arquivo, alcool, fim_endereco);
      printf ("  |  ");

      printf (" Diesel ");
      preco (arquivo, diesel, fim_endereco);
      printf ("\n\n");
    /* tratamento de <td class="combustivel preco"> */

      // se leu todos os postos de uma página
      if (contador == 20)
        {
          break;
        }
    }

  // fecha o arquivo e retorna ao main
  fclose (arquivo);
  return (EXIT_SUCCESS);
}

/**
* \brief Preço dos combustíveis
*
* \param Ponteiro do tipo FILE para o arquivo
* \param Expressão procurada
* \param Posição dentro do arquivo
*
* A função procura, dentro do file, o preço do
* combustível procurado, retornando o preço se
* encontrado, se não retorna vazio (-------).
*/
int
preco (arquivo, combustivel, posicao)
       FILE* arquivo;
       char* combustivel;
       int posicao;
{

  int inicio_combustivel, fim_combustivel;
  inicio_combustivel = fim_combustivel = 0;

  int inicio_preco, fim_preco;
  inicio_preco = fim_preco = 0;

  char* buffer;

  // verifica se ocorreu algum erro durante o acesso ao arquivo
  if (ferrorENOENT (arquivo))
    {
      remove (HTML_ESTADO);
      remove (HTML_CIDADE);
      exit (EXIT_FAILURE);
    }

  // inicio_combustível recebe a posição de <td class="combustivel preco">
  // após a posição de <br/> (endereço)
  inicio_combustivel = FileSearch (arquivo, combustivel, posicao);

  // fim_combustível recebe a posição de </td>
  // após a posição de <td class="combustivel preco">
  fim_combustivel = FileSearch (arquivo, finalCombustivel, inicio_combustivel);

  // inicio_preco recebe a posição de onde está <span id="preco
  // após a posição de <td class="combustivel preco">
  inicio_preco = FileSearch (arquivo, inicioTagPreco, inicio_combustivel);

  // se a posição de <span id="preco for menor que a posição de </td>
  // ou seja, se existir combustível para esse posto
  if ((inicio_preco < fim_combustivel) && (inicio_preco != -1))
    {
    /* tratamento de -xxx"> */
      // inicio_preco agora se posicina em "> após a posição de <span id="preco
      inicio_preco = FileSearch (arquivo, finalComum, inicio_preco);
    /* tratamento de -xxx"> */

      // o vetor de char buffer recebe o tamanho da string
      // contida entre <span id="preco..."> e </span>
      // ou seja, o preco do combustível
      buffer = malloc (6);

      // verifica erro de memória insuficiente
      if (bufferENOMEM (buffer))
        {
          fclose (arquivo);
          remove (HTML_ESTADO);
          remove (HTML_CIDADE);
          exit (EXIT_FAILURE);
        }

      // procura e posiciona, no arquivo, o cursor após posição de inicio_preco
      // ou seja, posiciona antes do preço do combustivel
      fseek (arquivo, inicio_preco + strlen (finalComum), SEEK_SET);

      // char* buffer recebe o preco do combustível
      fread (buffer, 1, 5, arquivo);

      // trata vetor de char como string e imprime
      buffer[5] = '\0';
      printf ("%s", buffer);

      // libera memória alocada
      free (buffer);
      return (EXIT_SUCCESS);
    }

    // se <span id="preco for maior que </td>, significa que encontrou o
    // preço de outro combustível, pois está fora do perímetro
    // ou se retornar -1, não encontrou a tag no resto do arquivo
    else
      {
        printf ("-----");
        return (EXIT_FAILURE);
      }
}

