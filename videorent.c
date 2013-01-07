#include "videorent.h"
#include "rental.h"

const GTimeSpan rent_days = 7 * G_TIME_SPAN_DAY;
// Member rental discount is 20%:
const gfloat member_discount_factor = 0.8;
const gint default_stock = 3;

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

GList* videos = NULL;
GList* rentals = NULL;

video*
video_new(
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
    v->v_count_in = default_stock;
    v->v_count_rented = 0;
    v->v_count_overdue = 0;
  }
  
  return v;
}

void
videorent_video_add(
  const gchar* const title,
  const gchar* const url,
  const gchar* const summary,
  const gfloat rental_amt
) {
  video* v = video_new(title, url, summary, rental_amt);
  
  if (v) {
    if (videos)
      v->v_id = ((video*)g_list_last(videos)->data)->v_id + 1;
    videos = g_list_append(videos, v);
  }
}

void
video_free(gpointer mem) {
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

video* const
video_get_from_id(guint video_id) {
  GList* video_item = g_list_find_custom(videos, &video_id, videorent_video_id_equals);
  return (video_item ? (video*)video_item->data : NULL);
}

const gchar* const
videorent_video_get_title(guint video_id) {
  video* v = video_get_from_id(video_id);
  return (v ? v->v_title : NULL);
}

void
videorent_video_set_title(guint video_id, const gchar* const title) {
  video* v = video_get_from_id(video_id);
  if (v) v->v_title = g_strdup(title);
}

const gchar* const
videorent_video_get_url(guint video_id) {
  video* v = video_get_from_id(video_id);
  return (v ? v->v_url : NULL);
}

void
videorent_video_set_url(guint video_id, const gchar* const url) {
  video* v = video_get_from_id(video_id);
  if (v) v->v_url = g_strdup(url);
}

const gchar* const
videorent_video_get_summary(guint video_id) {
  video* v = video_get_from_id(video_id);
  return (v ? v->v_summary : NULL);
}

void
videorent_video_set_summary(guint video_id, const gchar* const summary) {
  video* v = video_get_from_id(video_id);
  if (v) v->v_summary = g_strdup(summary);
}

const gfloat
videorent_video_get_rental_amt(guint video_id) {
  video* v = video_get_from_id(video_id);
  return (v ? v->v_rental_amt : -1.0);
}

void
videorent_video_set_rental_amt(guint video_id, const gfloat rental_amt) {
  video* v = video_get_from_id(video_id);
  if (v) v->v_rental_amt = rental_amt;
}

rental* const
rental_get_from_id(guint rental_id) {
  GList* rental_item = g_list_find_custom(rentals, &rental_id, videorent_rental_id_equals);
  return (rental_item ? (rental*)rental_item->data : NULL);
}

/*
videorent_rental_get_overdue - whether, as of right now, this rental is
overdue
*/
gboolean
rental_get_overdue(const rental* const r) {
  GDateTime* now = g_date_time_new_now_local();
  gboolean overdue = g_date_time_difference(now, r->r_date) > rent_days;

  g_date_time_unref(now);
  return overdue;
}

/*
videorent_rent - Rent a video

member_id - if 0 (zero), video is understood to be rented to a
non-member at full rental rate
*/
gboolean
videorent_rent(guint video_id, guint member_id) {
  gboolean rented = FALSE;
  video* v = video_get_from_id(video_id);
  if (!v) goto videorent_rent_end;
  // Do some bounds-checking on the IDs
  if (v->v_count_in == 0 || video_id < 0 || member_id < 0)
    goto videorent_rent_end;
  
  gfloat amount = v->v_rental_amt;
  gfloat discount = 0.0;
  if (member_id > 0) discount = amount * member_discount_factor;  

  rental* r = rental_new(video_id, member_id, amount, discount);
  if (r) {
    if (rentals)
      r->r_id = ((rental*)(g_list_last(rentals)->data))->r_id + 1;
    rentals = g_list_append(rentals, r);
    v->v_count_in--;
    v->v_count_rented++;
    rented = TRUE;
  }
  
videorent_rent_end:
  return rented;
}

gboolean
videorent_return(guint rental_id) {
  gboolean returned = FALSE;
  rental* r = rental_get_from_id(rental_id); if (!r) goto videorent_return_end;
  video* v = video_get_from_id(r->v_id); if (!v) goto videorent_return_end;

  if (v->v_count_rented > 0 || v->v_count_overdue > 0) {
    r->r_open = FALSE;
    if (rental_get_overdue(r)) v->v_count_overdue--;
    else v->v_count_rented--;
    returned = TRUE;
  }

videorent_return_end:
  return returned;
}

void videorent_exit() {
  g_list_free_full(videos, video_free); videos = NULL;
  g_list_free_full(rentals, rental_free); rentals = NULL;
}
