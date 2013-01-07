#include "videorent.h"
#include "video.h"
#include "rental.h"

const GTimeSpan rent_days = 7 * G_TIME_SPAN_DAY;
// Member rental discount is 20%:
const gfloat member_discount_factor = 0.8;
const gint default_stock = 3;

GList* videos = NULL;
GList* rentals = NULL;

void
videorent_add_video(
  const gchar* const title,
  const gchar* const url,
  const gchar* const summary,
  const gfloat rental_amt
) {
  video* v = videorent_video_new(title, url, summary, rental_amt);
  
  if (v) {
    if (videos)
      v->v_id = ((video*)g_list_last(videos)->data)->v_id + 1;
    v->v_count_in = default_stock;
    videos = g_list_append(videos, v);
  }
}

video* const
get_video_from_id(guint video_id) {
  GList* video_item = g_list_find_custom(videos, &video_id, videorent_video_id_equals);
  return (video_item ? (video*)video_item->data : NULL);
}

const gchar* const
videorent_get_video_title(guint video_id) {
  video* v = get_video_from_id(video_id);
  return (v ? v->v_title : NULL);
}

void
videorent_set_video_title(guint video_id, const gchar* const title) {
  video* v = get_video_from_id(video_id);
  if (v) v->v_title = g_strdup(title);
}

const gchar* const
videorent_get_video_url(guint video_id) {
  video* v = get_video_from_id(video_id);
  return (v ? v->v_url : NULL);
}

void
videorent_set_video_url(guint video_id, const gchar* const url) {
  video* v = get_video_from_id(video_id);
  if (v) v->v_url = g_strdup(url);
}

const gchar* const
videorent_get_video_summary(guint video_id) {
  video* v = get_video_from_id(video_id);
  return (v ? v->v_summary : NULL);
}

void
videorent_set_video_summary(guint video_id, const gchar* const summary) {
  video* v = get_video_from_id(video_id);
  if (v) v->v_summary = g_strdup(summary);
}

const gfloat
videorent_get_video_rental_amt(guint video_id) {
  video* v = get_video_from_id(video_id);
  return (v ? v->v_rental_amt : -1.0);
}

void
videorent_set_video_rental_amt(guint video_id, const gfloat rental_amt) {
  video* v = get_video_from_id(video_id);
  if (v) v->v_rental_amt = rental_amt;
}

rental* const
get_rental_from_id(guint rental_id) {
  GList* rental_item = g_list_find_custom(rentals, &rental_id, videorent_rental_id_equals);
  return (rental_item ? (rental*)rental_item->data : NULL);
}

/*
videorent_get_rental_overdue - whether, as of right now, this rental is
overdue
*/
gboolean
videorent_get_rental_overdue(const guint rental_id) {
  gboolean overdue = FALSE;
  rental* r = get_rental_from_id(rental_id);
  if (!r) goto videorent_get_rental_overdue_end;
  GDateTime* now = g_date_time_new_now_local();
  overdue = g_date_time_difference(now, r->r_date) > rent_days;

  g_date_time_unref(now);
videorent_get_rental_overdue_end:
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
  video* v = get_video_from_id(video_id);
  if (!v) goto videorent_rent_end;
  // Do some bounds-checking on the IDs
  if (v->v_count_in == 0 || video_id < 0 || member_id < 0)
    goto videorent_rent_end;
  
  gfloat amount = v->v_rental_amt;
  gfloat discount = 0.0;
  if (member_id > 0) discount = amount * member_discount_factor;  

  rental* r = videorent_rental_new(video_id, member_id, amount, discount);
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
  rental* r = get_rental_from_id(rental_id); if (!r) goto videorent_return_end;
  video* v = get_video_from_id(r->v_id); if (!v) goto videorent_return_end;

  if (v->v_count_rented > 0 || v->v_count_overdue > 0) {
    r->r_open = FALSE;
    if (videorent_get_rental_overdue(r->r_id)) v->v_count_overdue--;
    else v->v_count_rented--;
    v->v_count_in++;
    returned = TRUE;
  }

videorent_return_end:
  return returned;
}

void videorent_exit() {
  g_list_free_full(videos, videorent_video_free); videos = NULL;
  g_list_free_full(rentals, videorent_rental_free); rentals = NULL;
}
