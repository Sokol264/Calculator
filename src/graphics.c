#include "calculation.h"
#include "stackManager.h"
#include "polishManager.h"
#include <gtk/gtk.h>
#include <cairo.h>

typedef struct {
    stack *notat;
    GObject *entry_x;
    GObject *window_x;
    GObject *entry_exp;
} transportX;

typedef struct {
    GObject *entry_x;
    GObject *entry_y;
    GObject *window_choose_border;
    GObject *entry_exp;
} transportGraph;

int checkOnlyNumericAndOneDot(char *ptr) {
    int res = 1, dotCount = 0;
    while (*ptr != '\0' && res) {
        if (!checkNum(*ptr) && !findDot(*ptr, &dotCount)) res = 0;
        ptr++;
    }
    if (dotCount > 1) res = 0;
    return res;
}

void getBorder(GObject *entry_x, GObject *entry_y, double *x_bord, double *y_bord) {
    GtkEntryBuffer *buf_x = gtk_entry_get_buffer(GTK_ENTRY(entry_x));
    char *xBor = (char*)gtk_entry_buffer_get_text(buf_x);
    if (checkOnlyNumericAndOneDot(xBor)) {
        sscanf(xBor, "%lf", x_bord);
    } else {
        *x_bord = 0;
    }
    GtkEntryBuffer *buf_y = gtk_entry_get_buffer(GTK_ENTRY(entry_y));
    char *yBor = (char*)gtk_entry_buffer_get_text(buf_y);
    if (checkOnlyNumericAndOneDot(yBor)) {
        sscanf(yBor, "%lf", y_bord);
    } else {
        *y_bord = 0;
    }
}

static void draw_function(GtkDrawingArea* drawing_area, cairo_t* cr,
                          int width, int height, gpointer user_data) {
    transportGraph tr = *(transportGraph*)user_data;
    char *expression = (char*)gtk_entry_buffer_get_text(gtk_entry_get_buffer((GtkEntry*)tr.entry_exp));
    double y_border, x_border;
    getBorder(tr.entry_x, tr.entry_y, &x_border, &y_border);
    cairo_move_to(cr, 0, height / 2);
    cairo_line_to(cr, width, height / 2);
    cairo_move_to(cr, width / 2, height);
    cairo_line_to(cr, width / 2, 0);
    cairo_stroke(cr);
    cairo_translate(cr, width / 2, height / 2);
    cairo_set_line_width(cr, 1 / ((1 / (x_border * 0.02)) + (1 / (y_border * 0.02))));
    cairo_scale(cr, width / (x_border * 2), -height / (y_border * 2));
    cairo_set_source_rgb(cr, 1, 0, 0);
    double step = 2 * x_border / 10000;
    for (double x = -x_border, y; x < x_border; x += step) {
        stack *notation = polishConverter(expression);
        if (checkX(notation)) {
            replaceXWithValue(&notation, &x);
        }
        if (notation != NULL)
            y = calculate(&notation);
        else
            y = 0 / 0.0;
        if (y == y)
            cairo_line_to(cr, x, y);
        else
            cairo_new_sub_path(cr);
    }
    cairo_stroke(cr);
}

static void buildGraph(GtkWidget *widget, gpointer data) {
    transportGraph tr = *(transportGraph*)data;
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder_graph.glade", NULL);
    GObject *window_graph = gtk_builder_get_object(builder, "window_graph");
    GObject *area = gtk_builder_get_object(builder, "drawing_area");
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, data, NULL);
    gtk_widget_show(GTK_WIDGET(window_graph));
    gtk_window_close(GTK_WINDOW(tr.window_choose_border));
}

static void startDraw(GtkWidget *widget, gpointer data) {
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder_borders.glade", NULL);
    GObject *window_border = gtk_builder_get_object(builder, "window_border");
    GObject *entry_x = gtk_builder_get_object(builder, "entry_max_x");
    GObject *entry_y = gtk_builder_get_object(builder, "entry_max_y");
    GObject *button_done = gtk_builder_get_object(builder, "button_apply_border");
    static transportGraph tr;
    tr.entry_x = entry_x;
    tr.entry_y = entry_y;
    tr.window_choose_border = window_border;
    tr.entry_exp = data;
    g_signal_connect(button_done, "clicked", G_CALLBACK(buildGraph), &tr);
    gtk_widget_show(GTK_WIDGET(window_border));
}

void resultIntoEntry(stack *notation, GtkEntryBuffer* expression) {
    if (notation != NULL) {
        double result = calculate(&notation);
        char *res = (char*)malloc(sizeof(char) * 100);
        gtk_entry_buffer_delete_text(expression, 0, gtk_entry_buffer_get_length(expression));
        snprintf(res, sizeof(char) * 100, "%lf", result);
        gtk_entry_buffer_insert_text(expression, 0, res, strlen(res));
        free(res);
    } else {
        gtk_entry_buffer_delete_text(expression, 0, gtk_entry_buffer_get_length(expression));
        gtk_entry_buffer_insert_text(expression, 0, "nan", 3);
    }
}

static void clear(GtkWidget *widget, gpointer data) {
    GtkEntryBuffer *expression = gtk_entry_get_buffer((GtkEntry*)data);
    gtk_entry_buffer_delete_text(expression, 0, gtk_entry_buffer_get_length(expression));
    gtk_entry_buffer_insert_text(expression, 0, "", 0);
}

static void replaceAndCalc(GtkWidget *widget, gpointer data) {
    transportX trans = *(transportX*)data;
    GtkEntryBuffer *buf = gtk_entry_get_buffer(GTK_ENTRY(trans.entry_x));
    GtkEntryBuffer *buf_exp = gtk_entry_get_buffer(GTK_ENTRY(trans.entry_exp));
    char *newVal = (char*)gtk_entry_buffer_get_text(buf);
    double x;
    if (checkOnlyNumericAndOneDot(newVal)) {
        sscanf(newVal, "%lf", &x);
    } else {
        clearStack(&trans.notat);
    }
    replaceXWithValue(&trans.notat, &x);
    resultIntoEntry(trans.notat, buf_exp);
    gtk_window_close(GTK_WINDOW(trans.window_x));
}

void changeX(GObject *entry_exp, stack *notation) {
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder_x.glade", NULL);
    GObject *window_x = gtk_builder_get_object(builder, "window_x");
    GObject *entry_x = gtk_builder_get_object(builder, "entry_x");
    GObject *button_update_x = gtk_builder_get_object(builder, "button_update_x");
    static transportX trans;
    trans.window_x = window_x;
    trans.notat = notation;
    trans.entry_x = entry_x;
    trans.entry_exp = entry_exp;
    g_signal_connect(button_update_x, "clicked", G_CALLBACK(replaceAndCalc), &trans);
    gtk_widget_show(GTK_WIDGET(window_x));
}

static void calc(GtkWidget *widget, gpointer data) {
    GtkEntryBuffer *expression = gtk_entry_get_buffer((GtkEntry*)data);
    stack *notation = polishConverter((char*)gtk_entry_buffer_get_text(expression));
    if (checkX(notation)) {
        changeX(data, notation);
    } else {
        resultIntoEntry(notation, expression);
    }
}

static void add_into_entry(GtkWidget *widget, gpointer data) {
    char *text = (char*)gtk_button_get_label((GtkButton*)widget);
    GtkEntryBuffer *expression = gtk_entry_get_buffer((GtkEntry*)data);
    gtk_entry_buffer_insert_text(expression, gtk_entry_buffer_get_length(expression), text, strlen(text));
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "builder.glade", NULL);
    GObject *main_window = gtk_builder_get_object(builder, "main_window");
    gtk_window_set_application(GTK_WINDOW(main_window), app);
    GObject *entry_expression = gtk_builder_get_object(builder, "entry_expression");
    gtk_entry_set_alignment((GtkEntry*)entry_expression, 1);
    GObject *button_ln = gtk_builder_get_object(builder, "button_ln");
    GObject *button_open = gtk_builder_get_object(builder, "button_open");
    GObject *button_close = gtk_builder_get_object(builder, "button_close");
    GObject *button_ac = gtk_builder_get_object(builder, "button_ac");
    GObject *button_div = gtk_builder_get_object(builder, "button_div");
    GObject *button_7 = gtk_builder_get_object(builder, "button_7");
    GObject *button_8 = gtk_builder_get_object(builder, "button_8");
    GObject *button_9 = gtk_builder_get_object(builder, "button_9");
    GObject *button_log = gtk_builder_get_object(builder, "button_log");
    GObject *button_sin = gtk_builder_get_object(builder, "button_sin");
    GObject *button_asin = gtk_builder_get_object(builder, "button_asin");
    GObject *button_pow = gtk_builder_get_object(builder, "button_pow");
    GObject *button_mul = gtk_builder_get_object(builder, "button_mul");
    GObject *button_4 = gtk_builder_get_object(builder, "button_4");
    GObject *button_5 = gtk_builder_get_object(builder, "button_5");
    GObject *button_6 = gtk_builder_get_object(builder, "button_6");
    GObject *button_sqrt = gtk_builder_get_object(builder, "button_sqrt");
    GObject *button_cos = gtk_builder_get_object(builder, "button_cos");
    GObject *button_acos = gtk_builder_get_object(builder, "button_acos");
    GObject *button_mod = gtk_builder_get_object(builder, "button_mod");
    GObject *button_sub = gtk_builder_get_object(builder, "button_sub");
    GObject *button_1 = gtk_builder_get_object(builder, "button_1");
    GObject *button_2 = gtk_builder_get_object(builder, "button_2");
    GObject *button_3 = gtk_builder_get_object(builder, "button_3");
    GObject *button_draw = gtk_builder_get_object(builder, "button_draw");
    GObject *button_tan = gtk_builder_get_object(builder, "button_tan");
    GObject *button_atan = gtk_builder_get_object(builder, "button_atan");
    GObject *button_res = gtk_builder_get_object(builder, "button_res");
    GObject *button_add = gtk_builder_get_object(builder, "button_add");
    GObject *button_x = gtk_builder_get_object(builder, "button_x");
    GObject *button_0 = gtk_builder_get_object(builder, "button_0");
    GObject *button_dot = gtk_builder_get_object(builder, "button_dot");
    g_signal_connect(button_ln, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_open, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_close, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_ac, "clicked", G_CALLBACK(clear), entry_expression);
    g_signal_connect(button_div, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_7, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_8, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_9, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_log, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_sin, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_asin, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_pow, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_mul, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_4, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_5, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_6, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_sqrt, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_cos, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_acos, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_mod, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_sub, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_1, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_2, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_3, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_draw, "clicked", G_CALLBACK(startDraw), entry_expression);
    g_signal_connect(button_tan, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_atan, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_res, "clicked", G_CALLBACK(calc), entry_expression);
    g_signal_connect(button_add, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_x, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_0, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    g_signal_connect(button_dot, "clicked", G_CALLBACK(add_into_entry), entry_expression);
    gtk_widget_show(GTK_WIDGET(main_window));
    g_object_unref(builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return 0;
}
