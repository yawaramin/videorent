#include "video.h"

video*
videorent_video_new(
  const gchar* const title,
  const gchar* const url,
  const gchar* const summary,
  const gfloat rental_amt
) {
  video* v = g_new(video, 1);
  
  if (v) {
    v->v_id = 0;
    v->v_title = g_strdup(title);
    v->v_url = g_strdup(url);
    v->v_summary = g_strdup(summary);
    v->v_rental_amt = rental_amt;
    v->v_count_in = 0;
    v->v_count_rented = 0;
    v->v_count_overdue = 0;
  }
  
  return v;
}

void
videorent_video_free(gpointer mem) {
  video* v = (video*)mem;
  
  g_free(v->v_title);
  g_free(v->v_url);
  g_free(v->v_summary);
  g_free(mem);
}

gint
videorent_video_id_equals(gconstpointer item, gconstpointer v_id) {
  video* v = (video*)item;
  guint* v_id_int = (guint*)v_id;
  return v->v_id == *v_id_int ? 0 : 1;
}
