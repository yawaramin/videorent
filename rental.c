#include "rental.h"

rental*
rental_new(
  const guint video_id,
  const guint member_id,
  gfloat amount,
  gfloat discount
) {
  rental* r = g_new(rental, 1);
  
  if (r) {
    r->r_id = 0;
    r->r_date = g_date_time_new_now_local();
    r->v_id = video_id;
    r->m_id = member_id;
    r->r_amt = amount;
    r->r_discount = discount;
    r->r_open = TRUE;
  }
  
  return r;
}

void
rental_free(gpointer mem) {
  rental* r = (rental*)mem;
  
  g_date_time_unref(r->r_date);
  g_free(mem);
}

gint
videorent_rental_id_equals(gconstpointer item, gconstpointer r_id) {
  rental* r = (rental*)item;
  guint* r_id_int = (guint*)r_id;
  return (r->r_id == *r_id_int ? 0 : 1);
}
