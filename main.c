#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>

#define BLOCKSIZE 32
#define WMAPA 50
#define HMAPA 30
#define SCREENWIDTH BLOCKSIZE * WMAPA
#define PLAYABLE_SCREENHEIGHT (BLOCKSIZE * HMAPA)
#define INVENTORY_AREA_HEIGHT 96
#define TOTAL_SCREENHEIGHT (PLAYABLE_SCREENHEIGHT + INVENTORY_AREA_HEIGHT)

enum KEYS { UP, DOWN, LEFT, RIGHT };
enum KEYS2 { W, S, A, D };

ALLEGRO_BITMAP *bloco2 = NULL;
ALLEGRO_BITMAP *bloco1 = NULL;

typedef struct {
    int x;
    int y;
    bool collected;
    int owner;
    ALLEGRO_BITMAP *image;
} Item;

#define MAX_ITEMS 10
Item items[MAX_ITEMS];

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

ALLEGRO_FONT *game_font = NULL;

void DrawMap() {
    for (int i = 0; i < HMAPA; i++) {
        for (int j = 0; j < WMAPA; j++) {
            if (mapa[i][j] == 0) {
                al_draw_bitmap(bloco2, j * BLOCKSIZE, i * BLOCKSIZE, 0);
            } else {
                al_draw_bitmap(bloco1, j * BLOCKSIZE, i * BLOCKSIZE, 0);
            }
        }
    }
}

void InitItems() {
    ALLEGRO_BITMAP *item_img1 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img2 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img3 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img4 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img5 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img6 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img7 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img8 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img9 = al_load_bitmap("coin.png");
    ALLEGRO_BITMAP *item_img10 = al_load_bitmap("coin.png");

    if (!item_img1 || !item_img2 || !item_img3 || !item_img4 || !item_img5 ||
        !item_img6 || !item_img7 || !item_img8 || !item_img9 || !item_img10) {
        fprintf(stderr, "Erro ao carregar uma ou mais imagens de itens. Verifique os caminhos.\n");
    }

    items[0] = (Item){.x = 2 * BLOCKSIZE, .y = 1 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img1};
    items[1] = (Item){.x = 5 * BLOCKSIZE, .y = 3 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img2};
    items[2] = (Item){.x = 8 * BLOCKSIZE, .y = 6 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img3};
    items[3] = (Item){.x = 12 * BLOCKSIZE, .y = 9 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img4};
    items[4] = (Item){.x = 1 * BLOCKSIZE, .y = 10 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img5};
    items[5] = (Item){.x = 15 * BLOCKSIZE, .y = 1 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img6};
    items[6] = (Item){.x = 18 * BLOCKSIZE, .y = 4 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img7};
    items[7] = (Item){.x = 22 * BLOCKSIZE, .y = 7 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img8};
    items[8] = (Item){.x = 25 * BLOCKSIZE, .y = 10 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img9};
    items[9] = (Item){.x = 28 * BLOCKSIZE, .y = 13 * BLOCKSIZE, .collected = false, .owner = 0, .image = item_img10};
}

void DrawItems() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].collected) {
            al_draw_bitmap(items[i].image, items[i].x, items[i].y, 0);
        }
    }
}

void DrawCollectedItems() {
    int pacman_item_count = 0;
    int ghost_item_count = 0;

    al_draw_filled_rectangle(0, PLAYABLE_SCREENHEIGHT, SCREENWIDTH, TOTAL_SCREENHEIGHT, al_map_rgb(50, 50, 50));

    if (game_font) {
        al_draw_text(game_font, al_map_rgb(255, 255, 255), 10, PLAYABLE_SCREENHEIGHT + 5, 0, "Itens Pac-Man:");
        al_draw_text(game_font, al_map_rgb(255, 255, 255), SCREENWIDTH / 2 + 10, PLAYABLE_SCREENHEIGHT + 5, 0, "Itens Fantasma:");
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].collected) {
            if (items[i].owner == 1) {
                al_draw_bitmap(items[i].image, 10 + (pacman_item_count * (BLOCKSIZE + 5)), PLAYABLE_SCREENHEIGHT + 30, 0);
                pacman_item_count++;
            } else if (items[i].owner == 2) {
                al_draw_bitmap(items[i].image, SCREENWIDTH / 2 + 10 + (ghost_item_count * (BLOCKSIZE + 5)), PLAYABLE_SCREENHEIGHT + 30, 0);
                ghost_item_count++;
            }
        }
    }
}

bool colide_mapa(int x, int y, int player_width, int player_height) {
    int left_tile = x / BLOCKSIZE;
    int top_tile = y / BLOCKSIZE;
    int right_tile = (x + player_width - 1) / BLOCKSIZE;
    int bottom_tile = (y + player_height - 1) / BLOCKSIZE;

    if (left_tile < 0) left_tile = 0;
    if (top_tile < 0) top_tile = 0;
    if (right_tile >= WMAPA) right_tile = WMAPA - 1;
    if (bottom_tile >= HMAPA) bottom_tile = HMAPA - 1;

    if (mapa[top_tile][left_tile] != 0 ||
        mapa[top_tile][right_tile] != 0 ||
        mapa[bottom_tile][left_tile] != 0 ||
        mapa[bottom_tile][right_tile] != 0) {
        return true;
    }
    return false;
}

void CheckItemCollision(int playerX, int playerY, int playerWidth, int playerHeight, int playerOwner) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].collected) {
            if (playerX < items[i].x + BLOCKSIZE &&
                playerX + playerWidth > items[i].x &&
                playerY < items[i].y + BLOCKSIZE &&
                playerY + playerHeight > items[i].y) {
                items[i].collected = true;
                items[i].owner = playerOwner;
            }
        }
    }
}

int main() {
    ALLEGRO_DISPLAY *display;

    const float FPS = 60.0;
    bool done = false;
    bool redesenha = true;
    int posX = 32;
    int posY = 32;
    int posX2 = 257;
    int posY2 = 257;

    bool keys[4] = {false, false, false, false};
    bool keys2[4] = {false, false, false, false};

    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_font_addon();

    display = al_create_display(SCREENWIDTH, TOTAL_SCREENHEIGHT);
    if (!display) {
        fprintf(stderr, "Falha ao criar o display.\n");
        return -1;
    }
    al_set_window_position(display, 300, 200);

    game_font = al_create_builtin_font();
    if (!game_font) {
        fprintf(stderr, "Erro ao criar a fonte embutida do Allegro.\n");
        al_destroy_display(display);
        return -1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Falha ao criar o timer.\n");
        al_destroy_display(display);
        if (game_font) al_destroy_font(game_font);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        if (game_font) al_destroy_font(game_font);
        return -1;
    }

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_BITMAP *player2 = al_load_bitmap("Player1.png");
    ALLEGRO_BITMAP *player = al_load_bitmap("Player2.png");

    bloco2 = al_load_bitmap("wallInte.jpeg");
    bloco1 = al_load_bitmap("wall.jpeg");

    if (!player || !player2 || !bloco1 || !bloco2) {
        fprintf(stderr, "Erro ao carregar uma ou mais imagens de jogo (players ou blocos).\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        if (game_font) al_destroy_font(game_font);
        return -1;
    }

    InitItems();

    al_start_timer(timer);

    int player_width = al_get_bitmap_width(player);
    int player_height = al_get_bitmap_height(player);

    while (!done) {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if (events.type == ALLEGRO_EVENT_TIMER) {
            int new_posX = posX;
            int new_posY = posY;

            if (keys[UP]) new_posY -= 2;
            if (keys[DOWN]) new_posY += 2;
            if (keys[LEFT]) new_posX -= 2;
            if (keys[RIGHT]) new_posX += 2;

            if (!colide_mapa(new_posX, new_posY, player_width, player_height)) {
                posX = new_posX;
                posY = new_posY;
            }
            CheckItemCollision(posX, posY, player_width, player_height, 1);

            int new_posX2 = posX2;
            int new_posY2 = posY2;

            if (keys2[W]) new_posY2 -= 2;
            if (keys2[S]) new_posY2 += 2;
            if (keys2[A]) new_posX2 -= 2;
            if (keys2[D]) new_posX2 += 2;

            if (!colide_mapa(new_posX2, new_posY2, player_width, player_height)) {
                posX2 = new_posX2;
                posY2 = new_posY2;
            }
            CheckItemCollision(posX2, posY2, player_width, player_height, 2);

            redesenha = true;
        } else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        } else if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
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
        } else if (events.type == ALLEGRO_EVENT_KEY_UP) {
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

                case ALLEGRO_KEY_ESCAPE: done = true; break;
            }
        }

        if (redesenha && al_is_event_queue_empty(event_queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            DrawMap();
            DrawItems();

            al_draw_bitmap(player, posX, posY, 0);
            al_draw_bitmap(player2, posX2, posY2, 0);

            DrawCollectedItems();

            al_flip_display();
            redesenha = false;
        }
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].image) {
            al_destroy_bitmap(items[i].image);
        }
    }
    al_destroy_bitmap(player);
    al_destroy_bitmap(player2);
    al_destroy_bitmap(bloco1);
    al_destroy_bitmap(bloco2);
    if (game_font) al_destroy_font(game_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
