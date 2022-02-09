/*gcc gtk.c smartcalc.c -o s21_calc `pkg-config --cflags --libs gtk4`*/

#include <gtk/gtk.h>
#include <cairo.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include "smartcalc.h"

#define SCALE 800
#define K 50

static GtkWidget *entry;
GtkEntryBuffer *buffer;
GtkWidget *button;
GtkWidget *drawing;
GtkWidget *cairo_window;
gchar string[SIZE];
struct type_double stdouble;
struct flags errors;
char polish[SIZE] = "\0";

double osx(double x) {
  return SCALE / 2 + K * x;
}

double osy(double y) {
  return SCALE / 2 - K * y;
}

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // white
    cairo_paint(cr);
    cairo_set_line_width(cr, 1.5);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);  // black
// drawing cross
    cairo_move_to(cr, 0, SCALE / 2);
    cairo_line_to(cr, SCALE, SCALE / 2);
    cairo_move_to(cr, SCALE / 2, 0);
    cairo_line_to(cr, SCALE / 2, SCALE);
    cairo_stroke(cr);
// drawing horizontal lines
    int i = 0;
    cairo_move_to(cr, 0, SCALE / 2);
    cairo_set_line_width(cr, 0.25);
    for (; i < SCALE; i += K) {
      cairo_move_to(cr, i, 0);
      cairo_line_to(cr, i, SCALE);
    }
// drawing vertical lines
    cairo_move_to(cr, SCALE / 2, 0);
    for (i = 0; i < SCALE; i += K) {
      cairo_move_to(cr, 0, i);
      cairo_line_to(cr, SCALE, i);
    }

// print scale numbers
    int count = SCALE / -2 / K;
    char cnum[10] = {};
    for (i = 0; i < SCALE; i += K, count++) {
      cairo_move_to(cr, i, SCALE / 2 + 10);
      snprintf(cnum, sizeof(cnum), "%d", count);
      cairo_show_text(cr, cnum);
    }
    count = SCALE / -2 / K;
    for (i = SCALE; i > 0; i -= K, count++) {
      cairo_move_to(cr, SCALE / 2 + 5, i);
      if (count == 0) continue;
      snprintf(cnum, sizeof(cnum), "%d", count);
      cairo_show_text(cr, cnum);
    }
    cairo_stroke(cr);

// drawing function
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    create_double(&stdouble);
    for (double x = -100; x < 100; x += 0.005) {
      int len = strlen(polish);
      char *buf = (char*)malloc(len * sizeof(char));
      snprintf(buf, SIZE, "%s", polish);
      double y = parser(x, buf, &stdouble);
      double test = fabs(fabs(atan(y)) - M_PI_2);
      if (test > 0.005 && !isnan(y)) {
        cairo_line_to(cr, osx(x), osy(y));
      } else {
        cairo_new_sub_path(cr);
      }
      free(buf);
    }
    destroy_double(&stdouble);
    cairo_stroke(cr);
}

static void activate_cairo() {
  cairo_window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(cairo_window), "graphic");
  gtk_window_set_resizable(GTK_WINDOW(cairo_window), FALSE);
  gtk_widget_set_size_request(cairo_window, SCALE, SCALE);
  drawing = gtk_drawing_area_new();
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing), draw_function, NULL, NULL);
  gtk_window_set_child(GTK_WINDOW(cairo_window), drawing);
  gtk_widget_show(cairo_window);
}

void click_on_num(GtkButton *button, gpointer data) {
  const gchar *num = NULL;
  num = gtk_button_get_label(GTK_BUTTON(button));
  g_strlcat(string, num, SIZE);
  gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer), string, -1);
}

void clear() {
  gint len = strlen(string);
  for (gint i = 0; i < len; i++)
    string[i] = '\0';
  gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer), "", -1);
}

void click_on_ce(GtkButton *button, gpointer data) {
  clear();
}

void click_on_eq(GtkButton *button, gpointer data) {
  polish[0] = '\0';
  gchar result[SIZE] = "\0";
  struct type_stack stack;
  init_stack(&stack);
  init_flags(&errors);
  gchar *buf = (char*)gtk_entry_buffer_get_text(buffer);
  sort(buf, polish, &stack, &errors);
  if (errors.ops >= errors.nums) {
    snprintf(result, SIZE, "%s", "ERROR");
  } else if (errors.nums > 1 && !errors.funcs && !errors.ops) {
    snprintf(result, SIZE, "%s", "ERROR");
  } else if (stack.error) {
    snprintf(result, SIZE, "%s", "ERROR");
  } else if (!stack.error && !stack.flag_x) {
    create_double(&stdouble);
    parser(0.0, polish, &stdouble);
    if (fmod(fabs(stdouble.array[0]), 1.0) > 0.0) {
      snprintf(result, SIZE, "%.7f", stdouble.array[0]);
    } else {
      snprintf(result, SIZE, "%.f", stdouble.array[0]);
    }
    destroy_double(&stdouble);
  } else if (!stack.error && stack.flag_x) {
    activate_cairo();
    clear();
  }
  gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer), result, -1);
  destroy_stack(&stack);
}

static void on_activate(GtkApplication *app, gpointer user_data) {
  // Create a new window
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "calculator");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  // create grid
  GtkWidget *grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), grid);

  // create Entry with buffer
  entry = gtk_entry_new();
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 5, 1);

  // Create buttons
  button = gtk_button_new_with_label("C");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_ce), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 5, 0, 1, 1);

  button = gtk_button_new_with_label("cos(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

  button = gtk_button_new_with_label("sin(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);

  button = gtk_button_new_with_label("(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 1, 1);

  button = gtk_button_new_with_label(")");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 3, 1, 1, 1);

  button = gtk_button_new_with_label("%");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 4, 1, 1, 1);

  button = gtk_button_new_with_label("^");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 5, 1, 1, 1);

  button = gtk_button_new_with_label("tan(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

  button = gtk_button_new_with_label("acos(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 2, 1, 1);

  button = gtk_button_new_with_label("7");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 2, 2, 1, 1);

  button = gtk_button_new_with_label("8");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 3, 2, 1, 1);

  button = gtk_button_new_with_label("9");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 4, 2, 1, 1);

  button = gtk_button_new_with_label("/");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 5, 2, 1, 1);

  button = gtk_button_new_with_label("asin(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 1, 1);

  button = gtk_button_new_with_label("atan(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);

  button = gtk_button_new_with_label("4");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 2, 3, 1, 1);

  button = gtk_button_new_with_label("5");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 3, 3, 1, 1);

  button = gtk_button_new_with_label("6");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 4, 3, 1, 1);

  button = gtk_button_new_with_label("*");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 5, 3, 1, 1);

  button = gtk_button_new_with_label("sqrt(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 4, 1, 1);

  button = gtk_button_new_with_label("ln(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 4, 1, 1);

  button = gtk_button_new_with_label("1");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 2, 4, 1, 1);

  button = gtk_button_new_with_label("2");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 3, 4, 1, 1);

  button = gtk_button_new_with_label("3");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 4, 4, 1, 1);

  button = gtk_button_new_with_label("-");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 5, 4, 1, 1);

  button = gtk_button_new_with_label("log(");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);

  button = gtk_button_new_with_label("x");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 5, 1, 1);

  button = gtk_button_new_with_label("0");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 2, 5, 1, 1);

  button = gtk_button_new_with_label(".");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 3, 5, 1, 1);

  button = gtk_button_new_with_label("=");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_eq), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 4, 5, 1, 1);

  button = gtk_button_new_with_label("+");
  g_signal_connect(button, "clicked", G_CALLBACK(click_on_num), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 5, 5, 1, 1);

  gtk_widget_show(window);
}

int main(int argc, char *argv[]) {
  // Create a new application
  GtkApplication *app1 = gtk_application_new("com.cojess.GtkApplication", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app1, "activate", G_CALLBACK(on_activate), NULL);
  int buf = g_application_run(G_APPLICATION(app1), argc, argv);
  g_object_unref(app1);
  return buf;
}
