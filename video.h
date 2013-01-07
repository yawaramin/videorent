#ifndef VIDEO_H
#define VIDEO_H
#include <glib.h>

typedef struct {
  guint v_id;
  gchar* v_title;
  gchar* v_url;
  gchar* v_summary;
  gint v_count_rented; // Number of videos rented out
  /*
  Number overdue (exclusive of rented, i.e. we don't count overdue
  videos as a subset of rented videos--business logic decision,
  sorry :-)
  */
  gint v_count_overdue;
  gint v_count_in; // Number in stock right now
  gfloat v_rental_amt;
} video;

video*
videorent_video_new(
  const gchar* const title,
  const gchar* const url,
  const gchar* const summary,
  const gfloat rental_amt
);

void
videorent_video_free(gpointer mem);

gint
videorent_video_id_equals(gconstpointer item, gconstpointer v_id);

#endif // VIDEO_H
