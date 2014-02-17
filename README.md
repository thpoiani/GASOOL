GASOOL 1.06 - PREÇO DOS COMBUSTÍVEIS
====================================

Sobre o programa
----------------

  GASOOL é um software livre realizado para fins acadêmicos, idealizado para 
  a disciplina "Introdução à Sistemas Operacionais", ministrada pelo 
  Prof. Dr. [Pedro Northon Nobile](mailto:nobilecefetsp@gmail.com) no Instituto Federal
  de Educação, Ciência e Tecnologia de São Paulo (IFSP), campus São Carlos.

  A principal função desse programa é retornar os preços dos combustíveis
  dos postos cadastrados no site [Pre&ccedil;o dos Combust&iacute;veis no Brasil](http://www.precodoscombustiveis.com.br/).
  O seu retorno é dado graças a biblioteca libcurl, a qual é necessária para o
  funcionamento correto do programa. O download de sua última versão pode ser
  feito em [http://curl.haxx.se](http://curl.haxx.se/).

  Esse repositório contem a GASOOL versão 1.06.
  
Instalação
----------

Basicamente, os seguintes comandos shell devem configurar, criar e instalar o esse programa.

	./configure
	make
	make install

  O shell script `configure` prepara os componentes presentes no sistema
  que serão necessários durante a compilação, criando assim o `Makefile`,
  o shell script `config.status`, que você pode executar futuramente para
  recriar os configurações atuais, e o arquivo `config.log` contendo as
  saídas do compilador.

  O arquivo `configure.ac`, foi gerado pelo `autoscan`, e é usado para
  criar o `configure` com um programa chamado `autoconf`. Se você deseja
  modificar o programa, você precisa do `configure.ac` para atualizar o
  `configure`.
  
### Passo a passo

O modo mais simples de compilar o programa, após o download, é:

  1. `cd` para o diretório que contém o arquivo gasool-x.y.tar.gz.
  2. `tar zxvf gasool-x.y.tar.gz` para descompactar os arquivos.
  3. `cd gasool-x.y` para ir ao diretório extraído.
  4. Digite `./configure` para configurar o programa para seu sistema.
  5. Digite `make` para compilar o programa.
  6. Digite `make install` para instalar `gasool` e seu manual.

  É recomendado que o programa seja configurado e compilado com um usuário
  regular, e somente o `make install` executado com privilégios de root
  (administrador), portanto use `su` antes dessa fase.

### LIBCURL

  É preciso ter instalado a biblioteca "libcurl" para a execução do programa.
  Caso seu sistema não possuia a biblioteca citada, durante a configuração
  (`/configure`) é gerado um erro. Para mais detalhes, leia o arquivo `CURL`.

### Desinstalação

  Para desinstalar o programa, vá para a pasta onde se encontra o `Makefile`
  do programa e digite `make clean`. Serão desinstalados o programa `gasool`
  e seu manual.











Modo de uso
-----------

  O programa retorna o nome e endereço dos postos cadastrados, junto dos 
  valores de seus combustíveis, de uma cidade específica.
  Para verificar as cidades de determinado estado, digite `gasool -sp`, onde
  "sp" poderia ser substituído por qualquer unidade federativa do Brasil.
  Então, para verificar o retorno completo, digite `gasool -sp -sao-carlos`,
  sempre usando o caractere "-" no lugar dos espaços e evitando caracteres
  especiais.
  O programa não é case-sensitive.

  Para maiores informações, veja `gasool(1)` ou `gasool --help`.

Licença
-------

  GASOOL é um software livre sobre os termos da Licença Pública Geral GNU 
  versão 2. Veja uma cópia da licença no arquivo [COPYING](https://github.com/thpoiani/GASOOL/blob/master/gasool-1.06/COPYING).

Bugs e Contato
--------------

  Você pode mandar relatórios de bugs e defeitos para <[gasool.anp@live.com](mailto:gasool.anp@live.com)>.
  Entre em contato pelo mesmo email citado acima ou pelo site
  [SourceForge.net](http://sourceforge.net/projects/gasool/).

Autores
-------

* [Marcel Luis Nishihara](mailto:marcelnishihara@hotmail.com) - Idealizador, autor e desenvolvedor do programa.
* [Thiago Henrique Poiani](mailto:thpoiani@gmail.com) - Autor, desenvolvedor do programa e responsável pela manutenção.


### Agradecimentos

* [Pedro Northon Nobile](nobilecefetsp@gmail.com) - Professor orientador e idealizador do projeto.
* [Thiago Silva Barros](thiagosbarros02@gmail.com) - Considerável ajuda no desenvolvimento do programa, especialmente em manutenções e testes.
* [Lucas Fernandes Masalskas](lu_masalskas@hotmail.com) - Considerável ajuda no desenvolvimento do programa, especialmente em manutenções e testes.
* [Matheus Maricondi](matheusz_65@hotmail.com) - Considerável ajuda no desenvolvimento do programa, especialmente
      em interpretação da biblioteca libcurl.

Copyright
=========

  Copyright (C) 2012, Marcel Luis Nishihara - Thiago Henrique Poiani
  
  Este programa é software livre; você pode redistribuí-lo e/ou
  modificá-lo sob os termos da Licença Pública Geral GNU, conforme
  publicada pela Free Software Foundation; tanto a versão 2 da
  Licença como (a seu critério) qualquer versão mais nova.

  Este programa é distribuído na expectativa de ser útil, mas SEM
  QUALQUER GARANTIA; sem mesmo a garantia implícita de
  COMERCIALIZAÇÃO ou de ADEQUAÇÃO A QUALQUER PROPÓSITO EM PARTICULAR.
  Consulte a Licença Pública Geral GNU para obter mais detalhes.

