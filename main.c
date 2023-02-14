/* Non-GUI Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
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
#define GUI_WINDOW_BACKGROUND nk_rgba(21, 29, 40, 255)
#define GUI_TITLE_COLOR nk_rgba(47, 72, 92, 255)

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
    style->window.header.close_button.normal = nk_style_item_color(header_color);

    ctx->style = *style;
}

json_t* search_recipe(json_t* recipe_root, char search_string[64]) {
    json_t* result = json_array();
    size_t array_size = json_array_size(recipe_root);

    for (size_t i = 0; i < array_size; i++) {
        json_t *obj = json_array_get(recipe_root, i);
        if (json_is_object(obj)) {
            const char *key;
            json_t *value;
            json_object_foreach(obj, key, value) {
                // Access object fields here
                json_t *name = json_object_get(value, "name");
                json_t *id = json_object_get(value, "id");
                if (json_is_string(name) && json_is_string(id)) {
                    if (strstr(json_string_value(id), search_string)
                    || strstr(json_string_value(name), search_string)) {
                        fprintf(stdout, "ID (%s) or name (%s) match search string: %s\n",
                                json_string_value(id), json_string_value(name), search_string);
                        json_array_append(result, obj);
                    }
                }
            }
        }
    }
    return result;
}

int main(void)
{
    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    struct nk_colorf bg;

    /* Load JSON Files */

    json_t *reagent_root, *recipe_root;
    json_error_t error;

    reagent_root = json_load_file("../reagents.json", 0, &error);
    if (!reagent_root) {
        printf("Reagents failed to load: %s\n", error.text);
        exit(1);
    }

    recipe_root = json_load_file("../recipes.json", 0, &error);
    if (!recipe_root) {
        printf("Recipes failed to load: %s\n", error.text);
        exit(1);
    }

    /* GUI Variables */
    static char search_box[64];
    int search_box_size = 0;
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
        if (nk_begin(ctx, "Chem Helper", nk_rect(50, 50, 220, 220),
                     NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE|NK_WINDOW_SCALABLE|NK_WINDOW_TITLE))
        {

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, search_box, &search_box_size, 64, nk_filter_default);
            nk_layout_row_dynamic(ctx, 30, 1);
            struct nk_input *input = &ctx->input;
            if (nk_input_is_key_pressed(input, NK_KEY_ENTER) && search_box_size)
            {
                json_t* results_json = search_recipe(recipe_root, search_box);
                if (json_array_size(results_json) > 0) {
                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "Select a recipe:", NK_TEXT_LEFT);
            }
                else {
                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "No Recipes Found", NK_TEXT_ALIGN_CENTERED);
                }
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