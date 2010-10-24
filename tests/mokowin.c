#include <glib.h>

#include <mokosuite/utils/utils.h>
#include <mokosuite/ui/gui.h>

int main(int argc, char* argv[])
{
    mokosuite_utils_init();
    mokosuite_ui_init(argc, argv);

    MokoWin* w = mokowin_new("test-mokowin", TRUE);
    mokowin_create_vbox(w, TRUE);
    mokowin_set_title(w, "Test app");

    Evas_Object* btn = elm_button_add(w->win);
    elm_button_label_set(btn, "Hello world!");
    evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
    elm_box_pack_end(w->vbox, btn);
    evas_object_show(btn);

    Evas_Object* list = elm_genlist_add(w->win);
    evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(w->vbox, list);
    evas_object_show(list);

    evas_object_resize(w->win, 480, 640);
    mokowin_activate(w);

    elm_run();
    elm_shutdown();

    return 0;
}
