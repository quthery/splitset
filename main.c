#include <cairo-deprecated.h>
#include <cairo.h>
#include <glib.h>
#include <gtk/gtk.h>


static void draw_clock_hand(cairo_t *cr, int width, int height) {
    double cx = width / 2.0;
    double cy = height / 2.0;

    double size = MIN(width, height);
    double radius = size * 0.4;
    double hand_len = radius * 0.9;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (t == NULL) {
        return;
    }

    double angle = (t->tm_sec / 60.0) * 2.0 * G_PI - G_PI / 2.0;

    double x2 = cx + cos(angle) * hand_len;
    double y2 = cy + sin(angle) * hand_len;

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 3.0);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

    cairo_move_to(cr, cx, cy);
    cairo_line_to(cr, x2,y2);
    cairo_stroke(cr);
}


static void draw_cr(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    cairo_set_source_rgb(cr, 54.0 / 255.0, 69.0 / 255.0, 79.0 / 255.0);
    double size = MIN(width, height);
    double radius = size * 0.4;

    double x = width / 2.0;
    double y = height / 2.0;

    cairo_arc(cr, x, y, radius, 0, 2 * G_PI);
    cairo_fill(cr);

    // hands
    draw_clock_hand(cr, width, height);
}

static gboolean tick(gpointer data) {
    gtk_widget_queue_draw(GTK_WIDGET(data));
    return G_SOURCE_CONTINUE;
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  GtkWidget *area = gtk_drawing_area_new();

  gtk_window_set_title (GTK_WINDOW (window), "splitset");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

  gtk_window_set_child(GTK_WINDOW(window), area);

  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_cr, NULL, NULL);
  g_timeout_add_seconds(1, tick, area);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("xyz.quthery.splitset", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
