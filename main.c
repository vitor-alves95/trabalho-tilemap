#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>

#define BLOCKSIZE 32
#define WMAPA 50
#define HMAPA 30
#define SCREENWIDTH (BLOCKSIZE * WMAPA)
#define PLAYABLE_SCREENHEIGHT (BLOCKSIZE * HMAPA)
#define INVENTORY_AREA_HEIGHT 96
#define TOTAL_SCREENHEIGHT (PLAYABLE_SCREENHEIGHT + INVENTORY_AREA_HEIGHT)

// Enums para as teclas de controle dos jogadores
enum KEYS { UP, DOWN, LEFT, RIGHT }; // Para o Player 1 (setas)
enum KEYS2 { W, S, A, D }; // Para o Player 2 (WASD)

// Bitmaps globais para os blocos do mapa
ALLEGRO_BITMAP *bloco2 = NULL; // Representa o espaço vazio (caminho)
ALLEGRO_BITMAP *bloco1 = NULL; // Representa a parede

// Estrutura para definir um item colecionável (moeda)
typedef struct {
    int x;
    int y;
    bool collected; // Indica se o item foi coletado
    int owner; // 0 = não coletado, 1 = Pac-Man, 2 = Fantasma
    ALLEGRO_BITMAP *image; // Imagem do item
} Item;

#define MAX_ITEMS 10 // Número máximo de itens no jogo
Item items[MAX_ITEMS]; // Array para armazenar todos os itens do jogo

// O mapa do jogo. '1' para parede, '0' para espaço vazio.
int mapa[HMAPA][WMAPA] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,1},
    {1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1},
    {1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

ALLEGRO_FONT *game_font = NULL; // Fonte para exibir texto

// --- Funções de Desenho ---

// Desenha o mapa do jogo
void DrawMap() {
    for (int i = 0; i < HMAPA; i++) {
        for (int j = 0; j < WMAPA; j++) {
            if (mapa[i][j] == 0) { // Se for espaço vazio, desenha bloco2
                al_draw_bitmap(bloco2, j * BLOCKSIZE, i * BLOCKSIZE, 0);
            } else { // Se for parede, desenha bloco1
                al_draw_bitmap(bloco1, j * BLOCKSIZE, i * BLOCKSIZE, 0);
            }
        }
    }
}

// Inicializa a posição e estado dos itens (moedas)
void InitItems() {
    // Carrega a imagem da moeda UMA VEZ e a reutiliza para todos os itens.
    // Isso é mais eficiente do que carregar a mesma imagem 10 vezes.
    ALLEGRO_BITMAP *coin_image = al_load_bitmap("coin.png");
    if (!coin_image) {
        fprintf(stderr, "Erro ao carregar a imagem 'coin.png'. Verifique o caminho e se o arquivo existe.\n");
        // Em um jogo real, você poderia sair ou carregar uma imagem de erro padrão aqui.
    }

    // Define as posições iniciais dos itens no mapa
    // Certifique-se de que as posições estejam em espaços vazios (onde mapa[y][x] == 0)
    // As posições são em pixels, então multiplicamos por BLOCKSIZE.
    items[0] = (Item){.x = 2 * BLOCKSIZE, .y = 1 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[1] = (Item){.x = 5 * BLOCKSIZE, .y = 3 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[2] = (Item){.x = 8 * BLOCKSIZE, .y = 6 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[3] = (Item){.x = 12 * BLOCKSIZE, .y = 9 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[4] = (Item){.x = 1 * BLOCKSIZE, .y = 10 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[5] = (Item){.x = 15 * BLOCKSIZE, .y = 1 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[6] = (Item){.x = 18 * BLOCKSIZE, .y = 4 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[7] = (Item){.x = 22 * BLOCKSIZE, .y = 7 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[8] = (Item){.x = 25 * BLOCKSIZE, .y = 10 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
    items[9] = (Item){.x = 28 * BLOCKSIZE, .y = 13 * BLOCKSIZE, .collected = false, .owner = 0, .image = coin_image};
}

// Desenha os itens que AINDA NÃO foram coletados
void DrawItems() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].collected && items[i].image) { // Só desenha se não foi coletado e a imagem existe
            al_draw_bitmap(items[i].image, items[i].x, items[i].y, 0);
        }
    }
}

// Desenha os itens coletados na área de inventário na parte inferior da tela
void DrawCollectedItems() {
    int pacman_item_count = 0;
    int ghost_item_count = 0;

    // Desenha o fundo cinza para a área de inventário
    al_draw_filled_rectangle(0, PLAYABLE_SCREENHEIGHT, SCREENWIDTH, TOTAL_SCREENHEIGHT, al_map_rgb(50, 50, 50));

    // Desenha o texto do inventário
    if (game_font) {
        al_draw_text(game_font, al_map_rgb(255, 255, 255), 10, PLAYABLE_SCREENHEIGHT + 5, 0, "Itens Player 1:");
        al_draw_text(game_font, al_map_rgb(255, 255, 255), SCREENWIDTH / 2 + 10, PLAYABLE_SCREENHEIGHT + 5, 0, "Itens Player 2:");
    }

    // Desenha os itens coletados para cada jogador
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].collected && items[i].image) { // Só desenha se foi coletado e a imagem existe
            if (items[i].owner == 1) { // Pac-Man
                // Calcula a posição para desenhar o item no inventário do Pac-Man
                al_draw_bitmap(items[i].image, 10 + (pacman_item_count * (BLOCKSIZE + 5)), PLAYABLE_SCREENHEIGHT + 30, 0);
                pacman_item_count++;
            } else if (items[i].owner == 2) { // Fantasma
                // Calcula a posição para desenhar o item no inventário do Fantasma
                al_draw_bitmap(items[i].image, SCREENWIDTH / 2 + 10 + (ghost_item_count * (BLOCKSIZE + 5)), PLAYABLE_SCREENHEIGHT + 30, 0);
                ghost_item_count++;
            }
        }
    }
}

// --- Funções de Lógica de Jogo ---

// Verifica colisão do jogador com as paredes do mapa
bool colide_mapa(int x, int y, int player_width, int player_height) {
    // Calcula os tiles (células) que o jogador ocupa
    int left_tile = x / BLOCKSIZE;
    int top_tile = y / BLOCKSIZE;
    int right_tile = (x + player_width - 1) / BLOCKSIZE;
    int bottom_tile = (y + player_height - 1) / BLOCKSIZE;

    // Garante que os índices não saiam dos limites do mapa
    if (left_tile < 0) left_tile = 0;
    if (top_tile < 0) top_tile = 0;
    if (right_tile >= WMAPA) right_tile = WMAPA - 1;
    if (bottom_tile >= HMAPA) bottom_tile = HMAPA - 1;

    // Verifica se qualquer um dos quatro cantos do jogador está em um bloco de parede (valor '1' no mapa)
    if (mapa[top_tile][left_tile] == 1 ||
        mapa[top_tile][right_tile] == 1 ||
        mapa[bottom_tile][left_tile] == 1 ||
        mapa[bottom_tile][right_tile] == 1) {
        return true; // Há colisão
    }
    return false; // Não há colisão
}

// Verifica e gerencia a colisão do jogador com os itens
void CheckItemCollision(int playerX, int playerY, int playerWidth, int playerHeight, int playerOwner) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].collected) { // Só verifica colisão se o item ainda não foi coletado
            // Verifica a sobreposição dos retângulos do jogador e do item
            if (playerX < items[i].x + BLOCKSIZE &&
                playerX + playerWidth > items[i].x &&
                playerY < items[i].y + BLOCKSIZE &&
                playerY + playerHeight > items[i].y) {

                items[i].collected = true; // Marca o item como coletado
                items[i].owner = playerOwner; // Atribui o item ao jogador que o coletou
            }
        }
    }
}

// --- Função Principal ---

int main() {
    ALLEGRO_DISPLAY *display = NULL; // Ponteiro para a janela do jogo
    ALLEGRO_TIMER *timer = NULL; // Ponteiro para o timer do jogo
    ALLEGRO_EVENT_QUEUE *event_queue = NULL; // Ponteiro para a fila de eventos

    const float FPS = 60.0; // Frames por segundo
    bool done = false; // Flag para controlar o loop principal do jogo
    bool redesenha = true; // Flag para indicar quando a tela precisa ser redesenhada

    // Posições iniciais dos jogadores
    // **CORREÇÃO 1: Player 2 (Ghost) começando em uma posição válida.**
    // As posições devem ser em tiles vazios (valor 0 no mapa).
    // Posicionei o Player 2 em (48, 2) que é um espaço vazio.
    int posX = 1 * BLOCKSIZE; // Posição inicial do Player 1 (Pac-Man)
    int posY = 1 * BLOCKSIZE;
    int posX2 = 48 * BLOCKSIZE; // Posição inicial do Player 2 (Fantasma)
    int posY2 = 2 * BLOCKSIZE;

    // Estados das teclas pressionadas para cada jogador
    bool keys[4] = {false, false, false, false}; // Para o Player 1 (UP, DOWN, LEFT, RIGHT)
    bool keys2[4] = {false, false, false, false}; // Para o Player 2 (W, S, A, D)

    // --- Inicialização do Allegro ---
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar o Allegro.\n");
        return -1;
    }
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar o addon de primitivas do Allegro.\n");
        return -1;
    }
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar o addon de imagem do Allegro.\n");
        return -1;
    }
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado do Allegro.\n");
        return -1;
    }
    if (!al_init_font_addon()) {
        fprintf(stderr, "Falha ao inicializar o addon de fonte do Allegro.\n");
        return -1;
    }

    // Cria a janela do jogo
    display = al_create_display(SCREENWIDTH, TOTAL_SCREENHEIGHT);
    if (!display) {
        fprintf(stderr, "Falha ao criar o display.\n");
        return -1;
    }
    al_set_window_position(display, 300, 200); // Define a posição da janela na tela

    // Cria uma fonte embutida para o texto (inventário)
    game_font = al_create_builtin_font();
    if (!game_font) {
        fprintf(stderr, "Erro ao criar a fonte embutida do Allegro.\n");
        al_destroy_display(display);
        return -1;
    }

    // Cria o timer do jogo
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Falha ao criar o timer.\n");
        al_destroy_display(display);
        if (game_font) al_destroy_font(game_font);
        return -1;
    }

    // Cria a fila de eventos
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        if (game_font) al_destroy_font(game_font);
        return -1;
    }

    // Registra as fontes de eventos na fila
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Carrega as imagens dos jogadores
    ALLEGRO_BITMAP *player = al_load_bitmap("Player2.png"); // Corrigi o nome para Pac-Man (Player2.png)
    ALLEGRO_BITMAP *player2 = al_load_bitmap("Player1.png"); // Corrigi o nome para Fantasma (Player1.png)

    // Carrega as imagens dos blocos do mapa
    bloco2 = al_load_bitmap("wallInte.jpeg"); // Bloco para caminhos
    bloco1 = al_load_bitmap("wall.jpeg"); // Bloco para paredes

    // Verifica se todas as imagens foram carregadas com sucesso
    if (!player || !player2 || !bloco1 || !bloco2) {
        fprintf(stderr, "Erro ao carregar uma ou mais imagens de jogo (players ou blocos). Verifique os caminhos.\n");
        // Liberação de recursos em caso de falha no carregamento
        if (player) al_destroy_bitmap(player);
        if (player2) al_destroy_bitmap(player2);
        if (bloco1) al_destroy_bitmap(bloco1);
        if (bloco2) al_destroy_bitmap(bloco2);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        if (game_font) al_destroy_font(game_font);
        return -1;
    }

    InitItems(); // Inicializa os itens do jogo

    al_start_timer(timer); // Inicia o timer

    // Obtém as dimensões dos jogadores para a colisão
    int player_width = al_get_bitmap_width(player);
    int player_height = al_get_bitmap_height(player);

    // --- Loop Principal do Jogo ---
    while (!done) {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events); // Espera por um evento

        // Eventos de Timer (atualização da lógica do jogo)
        if (events.type == ALLEGRO_EVENT_TIMER) {
            int new_posX = posX;
            int new_posY = posY;

            // Atualiza a posição do Player 1 com base nas teclas pressionadas
            if (keys[UP]) new_posY -= 2;
            if (keys[DOWN]) new_posY += 2;
            if (keys[LEFT]) new_posX -= 2;
            if (keys[RIGHT]) new_posX += 2;

            // Verifica colisão com o mapa antes de atualizar a posição
            if (!colide_mapa(new_posX, new_posY, player_width, player_height)) {
                posX = new_posX;
                posY = new_posY;
            }
            // Verifica colisão com itens para o Player 1 (dono 1)
            CheckItemCollision(posX, posY, player_width, player_height, 1);

            int new_posX2 = posX2;
            int new_posY2 = posY2;

            // Atualiza a posição do Player 2 com base nas teclas pressionadas
            if (keys2[W]) new_posY2 -= 2;
            if (keys2[S]) new_posY2 += 2;
            if (keys2[A]) new_posX2 -= 2;
            if (keys2[D]) new_posX2 += 2;

            // Verifica colisão com o mapa antes de atualizar a posição
            if (!colide_mapa(new_posX2, new_posY2, player_width, player_height)) {
                posX2 = new_posX2;
                posY2 = new_posY2;
            }
            // Verifica colisão com itens para o Player 2 (dono 2)
            CheckItemCollision(posX2, posY2, player_width, player_height, 2);

            redesenha = true; // Define a flag para redesenhar a tela
        }
        // Evento de fechamento da janela
        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true; // Sai do loop principal
        }
        // Eventos de teclado (tecla pressionada)
        else if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
            int tecla = events.keyboard.keycode;
            switch (tecla) {
                case ALLEGRO_KEY_UP:    keys[UP] = true;    break;
                case ALLEGRO_KEY_DOWN:  keys[DOWN] = true;  break;
                case ALLEGRO_KEY_LEFT:  keys[LEFT] = true;  break;
                case ALLEGRO_KEY_RIGHT: keys[RIGHT] = true; break;

                case ALLEGRO_KEY_W: keys2[W] = true; break;
                case ALLEGRO_KEY_S: keys2[S] = true; break;
                case ALLEGRO_KEY_A: keys2[A] = true; break;
                case ALLEGRO_KEY_D: keys2[D] = true; break;
            }
        }
        // Eventos de teclado (tecla solta)
        else if (events.type == ALLEGRO_EVENT_KEY_UP) {
            int tecla = events.keyboard.keycode;
            switch (tecla) {
                case ALLEGRO_KEY_UP:    keys[UP] = false;    break;
                case ALLEGRO_KEY_DOWN:  keys[DOWN] = false;  break;
                case ALLEGRO_KEY_LEFT:  keys[LEFT] = false;  break;
                case ALLEGRO_KEY_RIGHT: keys[RIGHT] = false; break;

                case ALLEGRO_KEY_W: keys2[W] = false; break;
                case ALLEGRO_KEY_S: keys2[S] = false; break;
                case ALLEGRO_KEY_A: keys2[A] = false; break;
                case ALLEGRO_KEY_D: keys2[D] = false; break;

                case ALLEGRO_KEY_ESCAPE: done = true; break; // Tecla ESC para sair do jogo
            }
        }

        // --- Desenho da Tela ---
        // Só redesenha se a flag 'redesenha' estiver verdadeira e a fila de eventos vazia
        if (redesenha && al_is_event_queue_empty(event_queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela com preto

            DrawMap(); // Desenha o mapa
            DrawItems(); // Desenha os itens não coletados

            al_draw_bitmap(player, posX, posY, 0); // Desenha o Player 1
            al_draw_bitmap(player2, posX2, posY2, 0); // Desenha o Player 2

            DrawCollectedItems(); // Desenha os itens coletados na área de inventário

            al_flip_display(); // Atualiza a tela
            redesenha = false; // Reseta a flag de redesenho
        }
    }

    // --- Liberação de Recursos ---
    // Libera a memória alocada para os bitmaps dos itens
    // **CORREÇÃO 2: Liberação correta da imagem única da moeda.**
    // Como a imagem da moeda é carregada apenas uma vez e atribuída a todos os itens,
    // precisamos liberá-la apenas uma vez. Verificar se `items[0].image` (ou qualquer outro item)
    // não é NULL antes de destruí-lo.
    if (items[0].image) {
        al_destroy_bitmap(items[0].image);
    }

    // Libera a memória alocada para os outros bitmaps
    al_destroy_bitmap(player);
    al_destroy_bitmap(player2);
    al_destroy_bitmap(bloco1);
    al_destroy_bitmap(bloco2);

    // Libera a fonte
    if (game_font) al_destroy_font(game_font);

    // Libera os outros recursos do Allegro
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0; // Indica que o programa terminou com sucesso
}
