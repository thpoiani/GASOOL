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

/* gasool.h
 * ARQUIVO CABEÇALHO
 */

/* Declarações e definições */
#ifndef PRECODOSCOMBUSTIVEIS
#define PRECODOSCOMBUSTIVEIS "http://precodoscombustiveis.com.br/"
#endif

#ifndef ESTADO
#define ESTADO "postos/estado/"
#endif

#ifndef HTML_ESTADO
#define HTML_ESTADO "/tmp/estado.tmp"
#endif

#ifndef estado_invalido
#define estado_invalido "<h1>Ops!</h1>"
#endif

#ifndef CIDADE
#define CIDADE "/postos/cidade/"
#endif

#ifndef Pagina
#define Pagina "/pagina/"
#endif

#ifndef HTML_CIDADE
#define HTML_CIDADE "/tmp/cidade.tmp"
#endif

#ifndef inicioCidades
#define inicioCidades "<div id=\"menu-localidade\">"
#endif

#ifndef finalCidades
#define finalCidades "<div id=\"redes_sociais\">"
#endif

#ifndef inicioTagTD
#define inicioTagTD "<td class=\"posto\">"
#endif

#ifndef finalTagTD
#define finalTagTD "</a></td>"
#endif

#ifndef inicioTagHref
#define inicioTagHref "<a href=\"/posto/"
#endif

#ifndef inicioComum
#define inicioComum "/"
#endif

#ifndef finalComum
#define finalComum "\">"
#endif

#ifndef inicioTagEndereco
#define inicioTagEndereco "<td class=\"endereco\">"
#endif

#ifndef finalTagEndereco
#define finalTagEndereco "<br/>"
#endif

#ifndef finalCombustivel
#define finalCombustivel "</td>"
#endif

#ifndef inicioTagPreco
#define inicioTagPreco "<span id=\"preco"
#endif

#ifndef finalTagPreco
#define finalTagPreco "</span>"
#endif

#ifndef Taghref
#define Taghref "<td><a href=\"/"
#endif

#ifndef gasolina
#define gasolina "<td class=\"gasolina preco\">"
#endif

#ifndef alcool
#define alcool "<td class=\"alcool preco\">"
#endif

#ifndef diesel
#define diesel "<td class=\"diesel preco\">"
#endif

#ifndef VERSION
#define VERSION "1.06"
#endif

/* Declaração de protótipos */
void HELP (void);

void LICENSE (void);

void opcaoEINVAL (void);

int curlERRO (int);

int bufferENOMEM (char*);

int ferrorENOENT (FILE*);

int FileSearch (FILE*, char*, int);

int verificacao_estado (FILE*);

int cidades_estado (FILE*, char*);

char* verificacao_cidade (FILE*, char*);

int paginas (FILE*);

int postos (FILE*, int);

int preco (FILE*, char*, int);

