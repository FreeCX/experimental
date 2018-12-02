#include "graphic.h"

SDL_Rect wnd, pos;

int ani_init(SDL_Renderer *r, char *config, anim_file_t **a) {
    SDL_Texture *tex = NULL;
    anim_file_t *obj = A_NULL_OBJECT;
    anim_t *p = NULL;
    char buffer[256], tex_name[64];
    int i = -1, dy = 0;
    FILE *f;

    f = fopen(config, "r");
    if (f == NULL) {
        return A_ERROR_OPEN_FILE;
    }
    obj = (anim_file_t *)calloc(1, sizeof(anim_file_t));
    while (feof(f) == 0) {
        int ch = fgetc(f);
        if (ch == '#') {
            fgets(buffer, 256, f);
            printf("#%s", buffer);
            continue;
        } else {
            ungetc(ch, f);
        }
        if (i == -1) {
            fscanf(f, "%63s %d %d", tex_name, &(obj->frm_size), &(obj->anim_count));
            printf("[load]: %s %d %d\n", tex_name, obj->frm_size, obj->anim_count);
            tex = IMG_LoadTexture(r, tex_name);
            if (tex == NULL) {
                fclose(f);
                return A_ERROR_LOAD_TEXTURE;
            }
            obj->a = (anim_t *)calloc(obj->anim_count, sizeof(anim_t));
            obj->tex = tex;
            i++;
        } else {
            p = &(obj->a[i]);
            fscanf(f, "%255s %d %d", buffer, &(p->frm_count), &(p->frm_rate));
            printf("%s %d %d\n", buffer, p->frm_count, p->frm_rate);
            p->anim_name = (char *)calloc(strlen(buffer), sizeof(anim_t));
            strcpy(p->anim_name, buffer);
            p->frm_select = 0;
            p->counter = 0;
            p->dy = dy;
            p->id = i;
            dy += obj->frm_size;
            i++;
        }
    }
    fclose(f);
    wnd.x = 0;
    wnd.y = 0;
    wnd.w = obj->frm_size;
    wnd.h = obj->frm_size;
    pos.x = 0;
    pos.y = 0;
    pos.w = obj->frm_size;
    pos.h = obj->frm_size;
    *a = obj;
    return A_SUCCESS;
}

int ani_draw(SDL_Renderer *r, anim_file_t *obj, int id, int x, int y) {
    anim_t *p;
    int i = -1;

    if (!obj) {
        return A_NULL_OBJECT;
    }
    while (id != obj->a[++i].id)
        ;
    p = &(obj->a[i]);
    wnd.x = p->frm_select * obj->frm_size;
    wnd.y = p->dy;
    pos.x = x;
    pos.y = y;
    if (wnd.x > obj->frm_size * (p->frm_count - 1)) {
        wnd.x -= obj->frm_size * p->frm_count;
    }
    SDL_RenderCopy(r, obj->tex, &wnd, &pos);
    p->counter++;
    if (p->counter > p->frm_rate) {
        if (p->frm_select == p->frm_count - 1) {
            p->frm_select = 0;
        } else {
            p->frm_select++;
        }
        p->counter = 0;
    }
    return A_SUCCESS;
}

int ani_destroy(anim_file_t *obj) {
    if (!obj) {
        return A_NULL_OBJECT;
    }
    free(obj->a);
    SDL_DestroyTexture(obj->tex);
    free(obj);
    return A_SUCCESS;
}

void tile_draw(SDL_Renderer *r, SDL_Texture *t, short id, int x, int y) {
    static SDL_Rect wnd = {0, 0, 16, 16}, pos = {0, 0, 16, 16};
    int w, h, dy = 0;

    if (id != 0) {
        SDL_QueryTexture(t, NULL, NULL, &w, &h);
        while (id * 16 > w) {
            id -= w / 16;
            dy++;
        }
        wnd.x = (id - 1) * 16;
        wnd.y = dy * 16;
        pos.x = x;
        pos.y = y;
        SDL_RenderCopy(r, t, &wnd, &pos);
    }
}
