# Space Invaders

Este é um projeto de um clone simplificado do clássico jogo "Space Invaders", desenvolvido em linguagem C utilizando a biblioteca Allegro 5 na plataforma do WSL2 (Subsistema Linux do Windows).
Você pode acessar este projeto e esta documentação no meu github <https://github.com/hlgboot/space-invaders/> 

---

## 1. Manual de Uso

Este manual descreve como operar o jogo "Space Invaders", seus objetivos e funcionalidades básicas.

### 1.1. Objetivo do Jogo

O objetivo principal de "Space Invaders" é destruir todas as naves alienígenas inimigas que descem em direção à sua base, antes que elas cheguem ao solo ou colidam com a sua nave. Cada alien destruído concede pontos.

### 1.2. Instalação e Execução

Para jogar "Space Invaders", você precisará ter o **Allegro 5** configurado em seu ambiente de desenvolvimento C/C++.

1.  **Pré-requisitos:**
    * **Compilador C:** Recomenda-se GCC (GNU Compiler Collection).
    * **Allegro 5:** Certifique-se de ter o Allegro 5 e seus addons (primitives, font, ttf, audio, acodec, image) instalados e configurados corretamente no seu sistema.

2.  **Clonar o Repositório:**
    ```bash
    git clone [https://github.com/hlgboot/space-invaders.git](https://github.com/hlgboot/space-invaders.git)
    cd space-invaders
    ```

3.  **Compilação:**
    Compile o código-fonte (`.c`) usando um compilador C compatível, vinculando as bibliotecas Allegro 5 necessárias.
    * **Comando de compilação (Linux/MinGW no Windows):**
        ```bash
        make
        ```
    * *Nota:* O jogo foi desenvolvido em WSL2. O nome do executável de saída será `invaders.exe`.

4.  **Estrutura de Arquivos:**
    Certifique-se de que a estrutura de arquivos e pastas esteja correta para que o jogo encontre seus recursos:

    ```
    space-invaders/
    ├── inavders.c              # Código fonte principal do jogo
    ├── bitter.ttf              # Fonte utilizada para o texto no jogo
    ├── files/
    │   ├── audio/
    │   │   ├── bg.ogg                # Música de fundo do jogo
    │   │   ├── explosao.wav          # Efeito sonoro de explosão de alien
    │   │   ├── loss.wav              # Efeito sonoro de derrota
    │   │   └── win.wav               # Efeito sonoro de vitória
    │   ├── images/
    │   │   ├── alien.png             # Sprite da nave alienígena (em branco para coloração)
    │   │   └── nave.png              # Sprite da nave do jogador (em branco para coloração)
    │   └── record.txt                # Arquivo para armazenamento do recorde de pontuação
    └── README.md               # Este arquivo de documentação
    ```
    O arquivo `record.txt` será criado automaticamente na primeira execução se não existir.

5.  **Execução:**
    Após a compilação bem-sucedida, execute o arquivo gerado a partir do diretório raiz do projeto:
    ```bash
    ./invaders.exe
    ```

### 1.3. Controles

O jogo utiliza o teclado para os movimentos da nave e para atirar:

* **Setas Esquerda (`<-`) / Tecla A:** Mover a nave para a esquerda.
* **Setas Direita (`->`) / Tecla D:** Mover a nave para a direita.
* **Barra de Espaço (`SPACE`):** Disparar o tiro da nave.

### 1.4. Pontuação e Recorde

* **Pontuação Atual:** A pontuação é exibida no canto inferior esquerdo da tela. Cada nave alienígena destruída adiciona 1 ponto à sua pontuação.
* **Recorde:** O recorde máximo de pontos é exibido no canto inferior direito da tela. Este recorde é persistente, sendo salvo no arquivo `files/record.txt` e carregado a cada início de jogo.

### 1.5. Condições de Fim de Jogo

O jogo pode terminar das seguintes maneiras:

* **Vitória:** O jogador vence ao destruir todas as naves alienígenas na tela. Uma mensagem de "Parabéns! Você venceu!" e um som de vitória são exibidos.
* **Derrota (Aliens no Solo):** O jogo termina se qualquer nave alienígena conseguir alcançar a área verde do solo. Uma mensagem de "Que pena! Você perdeu!" e um som de derrota são exibidos.
* **Derrota (Colisão com a Nave):** O jogo termina se qualquer nave alienígena colidir com a nave do jogador. A mensagem e o som de derrota são exibidos.
* **Derrota (Tiro Alienígena):** O jogo termina se um tiro disparado por um alien colidir com a nave do jogador. A mensagem e o som de derrota são exibidos.

---

## 2. Detalhes da Implementação

Esta seção descreve a estrutura do código-fonte, as principais estruturas de dados, funções e bibliotecas utilizadas na implementação do jogo.

### 2.1. Estrutura do Código e Módulos Allegro 5

O projeto é desenvolvido em linguagem C e utiliza a biblioteca Allegro 5 para todas as funcionalidades gráficas, de entrada e de áudio. O código-fonte principal está contido em `invaders.c`.

As seguintes bibliotecas do Allegro 5 são utilizadas:

* `allegro5/allegro.h`: Funções básicas do Allegro, gerenciamento de eventos e display.
* `allegro5/allegro_primitives.h`: Desenho de formas geométricas (círculos, retângulos, triângulos) para o cenário e tiros.
* `allegro5/allegro_font.h` e `allegro5/allegro_ttf.h`: Carregamento e renderização de fontes TrueType (TTF) para exibir pontuação e mensagens.
* `allegro5/allegro_audio.h` e `allegro5/allegro_acodec.h`: Gerenciamento e reprodução de áudio (efeitos sonoros e música de fundo).
* `allegro5/allegro_image.h`: Carregamento e manipulação de imagens (PNG) para os sprites da nave e dos aliens.

### 2.2. Constantes Globais

As constantes globais são definidas no início de `invaders.c` e controlam dimensões da tela, tamanhos de entidades, velocidades e contagens:

* `SCREEN_W`, `SCREEN_H`: Largura e altura da janela do jogo (960x700 pixels).
* `GRASS_H`: Altura da área do "solo" (grama) na parte inferior da tela.
* `NAVE_W`, `NAVE_H`: Largura e altura da nave do jogador (100x50 pixels).
* `N_ALIEN_ROWS`, `N_ALIEN_COLS`: Número de linhas (5) e colunas (8) da formação alienígena.
* `ALIEN_W`, `ALIEN_H`: Largura e altura dos sprites dos aliens (40x40 pixels).
* `ALIEN_X_VEL`: Velocidade de movimento horizontal dos aliens (0.8 pixels por frame). Inverte a direção ao atingir as bordas da tela. (Declarado junto às constantes, mas não é uma)
* `MAX_TIROS_ALIEN`: Número máximo de tiros que os aliens podem ter na tela simultaneamente (5).
* `FPS`: Taxa de quadros por segundo do jogo (100 FPS).

### 2.3. Estruturas de Dados

As principais entidades do jogo são representadas pelas seguintes estruturas:

* **`Nave`**:
    ```c
    typedef struct Nave {
        float x;        // Posição central horizontal da nave
        float vel;      // Velocidade de movimento (pixels/frame)
        int dir, esq;   // Flags de controle de direção (1=ativo, 0=inativo)
        ALLEGRO_COLOR cor; // Cor base para tinting do sprite (NAVE_COR global)
    } Nave;
    ```
    * **Propósito:** Gerenciar o estado e o comportamento da nave controlada pelo jogador.

* **`Alien`**:
    ```c
    typedef struct Alien {
        float x, y;     // Posição do canto superior esquerdo do alien
        ALLEGRO_COLOR cor; // Cor base para tinting do sprite (ALIEN_COR global)
        int ativo;      // Flag de estado (1=ativo/visível, 0=destruído)
    } Alien;
    ```
    * **Propósito:** Gerenciar o estado individual de cada nave alienígena. Uma matriz `Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS]` armazena a formação de inimigos.

* **`Tiro`**:
    ```c
    typedef struct Tiro {
        float x, y;     // Posição do centro do tiro
        float vel;      // Velocidade vertical (pixels/frame)
        int ativo;      // Flag de estado (1=ativo, 0=inativo/fora da tela)
    } Tiro;
    ```
    * **Propósito:** Representar o único tiro disparado pela nave do jogador.

* **`TiroAlien`**:
    ```c
    typedef struct TiroAlien {
        float x, y;     // Posição do centro do tiro alienígena
        float vel;      // Velocidade vertical (pixels/frame)
        int ativo;      // Flag de estado (1=ativo, 0=inativo/fora da tela)
    } TiroAlien;
    ```
    * **Propósito:** Representar os tiros disparados pelos aliens. Uma lista de `MAX_TIROS_ALIEN` tiros é gerenciada para permitir múltiplos projéteis inimigos na tela.

### 2.4. Funções e Lógica Principal

A seguir, são detalhadas as funções e a lógica por trás dos principais módulos do jogo:

* **`main()`**: A função `main` orquestra todo o ciclo do jogo. Ela inicializa todos os componentes do Allegro, carrega os recursos (fontes, sprites, áudios), configura a fila de eventos, inicializa as entidades do jogo (`Nave`, `Alien`, `Tiro`, `TiroAlien`), e entra no loop principal. Dentro do loop, ele processa eventos, atualiza o estado do jogo (movimento, colisões), desenha os elementos na tela e verifica as condições de fim de jogo. Ao final, lida com a exibição de vitória/derrota e libera todos os recursos alocados.

* **Gerenciamento de Recorde (`readRecord()`, `saveRecord(int newR)`)**:
    * `readRecord()`: Abre o arquivo `files/record.txt` em modo de leitura e recupera o recorde salvo. Se o arquivo não existir ou não puder ser lido, retorna 0.
    * `saveRecord(int newR)`: Abre o arquivo `files/record.txt` em modo de escrita e armazena a nova pontuação máxima.

* **Cores e Cenário (`getRandomColor()`, `changeColors()`, `draw_cenario(...)`)**:
    * `getRandomColor()`: Gera uma cor `ALLEGRO_COLOR` aleatória.
    * `changeColors()`: Define cores aleatórias para `ALIEN_COR`, `NAVE_COR`, `FUNDO_COR` e `GRAMA_COR`. É chamada a cada vez que um alien é destruído, proporcionando uma experiência visual dinâmica e de "cenários diferentes".
    * `draw_cenario(...)`: Limpa a tela com a `FUNDO_COR`, desenha o retângulo da "grama" com a `GRAMA_COR` e exibe a pontuação atual e o recorde.

* **Nave do Jogador (`initNave()`, `draw_nave()`, `update_nave()`, `atirar()`, `update_tiro()`, `draw_tiro()`)**:
    * `initNave()`: Configura a posição inicial, velocidade e estado de movimento da nave.
    * `draw_nave()`: Renderiza o sprite `nave.png` na posição da nave. O sprite é *tintado* (colorido) com a `NAVE_COR` para permitir mudança dinâmica de cor.
    * `update_nave()`: Move a nave horizontalmente com base nas teclas pressionadas (`dir`, `esq`), respeitando os limites da tela.
    * `atirar()`: Ativa o tiro da nave se não houver um tiro ativo, posicionando-o no centro da nave e acima dela.
    * `update_tiro()`: Move o tiro verticalmente para cima e o desativa se sair da tela.
    * `draw_tiro()`: Desenha o tiro como um círculo amarelo se estiver ativo.

* **Aliens (`initAliens()`, `draw_aliens()`, `update_aliens()`, `colisao_aliens_solo()`, `alien_atira()`, `update_tiros_alien()`, `draw_tiros_alien()`)**:
    * `initAliens()`: Preenche a matriz de `aliens`, definindo suas posições iniciais e estado ativo. A posição é calculada para garantir o 30px de espaçamento mínimo entre eles.
    * `draw_aliens()`: Itera sobre a matriz de aliens e chama `draw_alien()` para cada alien ativo.
    * `draw_alien()`: Renderiza o sprite `alien.png` na posição do alien, *tintado* com a `ALIEN_COR` para coloração dinâmica.
    * `update_aliens()`: Implementa o movimento em bloco dos aliens. Eles se movem horizontalmente e, ao atingir as bordas da tela, invertem a direção e descem uma linha.
    * `colisao_aliens_solo()`: Verifica se qualquer alien ativo alcançou a linha do solo, indicando uma condição de derrota.
    * `alien_atira()`: Seleciona aleatoriamente uma coluna e, se houver um alien ativo nela (o mais baixo), dispara um `TiroAlien` a partir de sua posição, se houver um tiro de alien disponível.
    * `update_tiros_alien()`: Move os tiros alienígenas verticalmente para baixo e os desativa se saírem da tela.
    * `draw_tiros_alien()`: Desenha os tiros alienígenas como círculos brancos.

* **Colisões (`colisao_tiro_alien()`, `colisao_tiro_nave()`, `colisao_nave_alien()`)**:
    * `colisao_tiro_alien()`: Verifica a sobreposição retangular entre o tiro do jogador e cada alien ativo. Se houver colisão, o alien e o tiro são desativados, pontos são adicionados, e as cores do cenário mudam.
    * `colisao_tiro_nave()`: Verifica a sobreposição retangular entre os tiros alienígenas ativos e a nave do jogador.
    * `colisao_nave_alien()`: Verifica a sobreposição retangular entre os aliens ativos e a nave do jogador, indicando contato direto.

### 2.5. Pontos Extras Implementados

As seguintes funcionalidades adicionais foram incorporadas ao projeto para enriquecer a experiência de jogo e demonstrar habilidades extras:

* **Uso de Imagens (Sprites):** A nave do jogador e as naves alienígenas são representadas por arquivos PNG (`nave.png`, `alien.png`), substituindo os desenhos primitivos. Isso permite um visual mais detalhado e fiel ao jogo original, além de possibilitar a aplicação de cores dinâmicas via tinting do Allegro.
* **Tiros das Naves Alienígenas:** As naves alienígenas possuem a capacidade de disparar seus próprios projéteis em direção à nave do jogador. Um alien ativo aleatório (o mais baixo da coluna) pode atirar periodicamente, gerenciando múltiplos tiros (`MAX_TIROS_ALIEN`) simultâneos na tela. A colisão desses tiros com a nave do jogador resulta no fim do jogo.
* **Geração de Diferentes Cenários (Coloração Dinâmica):** A cada vez que um alienígena é destruído, as cores do fundo, da grama, da nave do jogador e dos próprios aliens são alteradas aleatoriamente. Isso cria uma experiência visual variada e dinâmica a cada rodada.
* **Sons e Músicas:** O jogo inclui uma música de fundo que toca em loop durante toda a partida, além de efeitos sonoros para eventos importantes:
    * **`explosao.wav`**: Tocado quando um alien é atingido pelo tiro do jogador.
    * **`win.wav`**: Tocado quando o jogador vence a fase.
    * **`loss.wav`**: Tocado quando o jogador perde o jogo.

---