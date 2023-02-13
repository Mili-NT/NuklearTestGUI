/* Non-GUI Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
/* GLEW and GLFW 3 Includes */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/* Nuklear Directives */
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear.h>
#include <nuklear_glfw_gl4.h>
/* Defines the GLFW Window Attributes */
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "demo"
/* Define maximum buffer size for use in nk_glfw3_init() */
#define MAX_VERTEX_BUFFER (512 * 1024)
#define MAX_ELEMENT_BUFFER (128 * 1024)
/* Style Directives */
#define GUI_WINDOW_BACKGROUND nk_rgba(51, 51, 51, 255)
#define GUI_TITLE_COLOR nk_rgba(229, 4, 49, 255)

// Callback function used to report GLFW errors
void error_callback(int error, const char* description) {
    printf("GLFW Error: %d: %s\n", error, description);
}
// Sets the style
void set_style(struct nk_context *ctx) {
    struct nk_color bg_color = GUI_WINDOW_BACKGROUND;
    struct nk_color header_color = GUI_TITLE_COLOR;
    struct nk_style *style = &ctx->style;

    style->window.fixed_background = nk_style_item_color(bg_color);
    style->window.header.normal = nk_style_item_color(header_color);
    style->window.header.active = nk_style_item_color(header_color);

    ctx->style = *style;
}

int main(void)
{
    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    struct nk_colorf bg;

    char input_text[256] = "";
    int input_size = 0; // array size is updated by nk_edit_string()
    char output_text[256] = "";
    int output_size = 0; // array size is updated by nk_edit_string()

    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* OpenGL */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        nk_glfw3_font_stash_end();
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        /*nk_style_set_font(ctx, &droid->handle);*/}
    set_style(ctx);
    // Viewport Background
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (!glfwWindowShouldClose(win))
    {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        /* GUI */
        if (nk_begin(ctx, "Cryptography", nk_rect(50, 50, 220, 220),
                     NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_SCALABLE|NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, "Input:", NK_TEXT_LEFT);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, input_text, &input_size, 256, nk_filter_default);

            nk_label(ctx, "Output:", NK_TEXT_LEFT);
            nk_edit_string(ctx, NK_EDIT_DEACTIVATED, output_text, &output_size, 256, nk_filter_default);

            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_button_label(ctx, "Decrypt")) {

            }
        }
        nk_end(ctx);

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}