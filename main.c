/* nuklear - v1.09 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>

#include <SDL/SDL.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_SDL_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear_sdl_12.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the defines */
/*#define INCLUDE_ALL */
/*#define INCLUDE_STYLE */
/*#define INCLUDE_CALCULATOR */
/*#define INCLUDE_OVERVIEW */
/*#define INCLUDE_NODE_EDITOR */

#ifdef INCLUDE_ALL
  #define INCLUDE_STYLE
  #define INCLUDE_CALCULATOR
  #define INCLUDE_OVERVIEW
  #define INCLUDE_NODE_EDITOR
#endif

#ifdef INCLUDE_STYLE
  #include "nuklear/demo/style.c"
#endif
#ifdef INCLUDE_CALCULATOR
  #include "nuklear/demo/calculator.c"
#endif
#ifdef INCLUDE_OVERVIEW
  #include "nuklear/demo/overview.c"
#endif
#ifdef INCLUDE_NODE_EDITOR
  #include "nuklear/demo/node_editor.c"
#endif

/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/
int main(int argc, char **argv)
{
    static SDL_Surface *screen_surface;
    struct nk_color background;
    struct nk_colorf newBackground;
    int running = 1;
    struct nk_context *ctx;
    float bg[4];

    /* SDL setup */
    if( SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
        return 1;
    }
    screen_surface = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_SWSURFACE);
    if(screen_surface == NULL) {
        printf( "Can't set video mode: %s\n", SDL_GetError( ) );
        return 1;
    }

    ctx = nk_sdl_init(screen_surface);
    background = nk_rgb(28, 48, 62);

    #ifdef INCLUDE_STYLE
    /*set_style(ctx, THEME_WHITE); */
    /*set_style(ctx, THEME_RED); */
    /*set_style(ctx, THEME_BLUE);*/
    /*set_style(ctx, THEME_DARK);*/
    #endif

    while (running)
    {
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        /* GUI */
        {
            struct nk_panel layout;
            if (nk_begin(ctx, "Demo", nk_rect(50, 50, 210, 250),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
            {
                enum {EASY, HARD};
                static int op = EASY;
                static int property = 20;
                static char buffer[64];
                static int len;
                struct nk_vec2 vecSize;

                nk_layout_row_static(ctx, 30, 80, 1);
                if (nk_button_label(ctx, "button"))
                    fprintf(stdout, "button pressed\n");
                nk_layout_row_dynamic(ctx, 30, 2);
                if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
                if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
                nk_layout_row_dynamic(ctx, 22, 1);
                nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
                /* nk_edit_string(ctx, NK_EDIT_SIMPLE, buffer, &len, 64, 0); */

                {
                    struct nk_panel combo;
                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_label(ctx, "background:", NK_TEXT_LEFT);
                    nk_layout_row_dynamic(ctx, 25, 1);
                    vecSize.x = nk_widget_width(ctx);
                    vecSize.y = 400;
                    if (nk_combo_begin_color(ctx, background, vecSize)) {
                        nk_layout_row_dynamic(ctx, 120, 1);
                        newBackground = nk_color_picker(ctx, nk_color_cf(background), NK_RGBA);
                        nk_layout_row_dynamic(ctx, 25, 1);
                        background.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, newBackground.r * 255.0f, 255, 1,1);
                        background.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, newBackground.g * 255.0f, 255, 1,1);
                        background.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, newBackground.b * 255.0f, 255, 1,1);
                        background.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, newBackground.a * 255.0f, 255, 1,1);
                        nk_combo_end(ctx);
                    }
                }
            }
            nk_end(ctx);
        }

        /* -------------- EXAMPLES ---------------- */
        #ifdef INCLUDE_CALCULATOR
          calculator(ctx);
        #endif
        #ifdef INCLUDE_OVERVIEW
          overview(ctx);
        #endif
        #ifdef INCLUDE_NODE_EDITOR
          node_editor(ctx);
        #endif
        /* ----------------------------------------- */

        /* Draw */
        nk_color_fv(bg, background);
        nk_sdl_render(nk_rgb(background.r, background.g, background.b));
    }

cleanup:
    nk_sdl_shutdown();
    SDL_Quit();
    return 0;
}

