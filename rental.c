#include "rental.h"

rental*
rental_new(
  const guint video_id,
  const guint member_id,
  gfloat amount,
  gfloat discount,
  rental_status status
) {
  rental* r = g_new(rental, 1);
  
  if (r) {
    r->r_id = 0;
    r->r_date = g_date_time_new_now_local();
    r->r_id = video_id;
    r->r_amt = amount;
    r->r_discount = discount;
    r->r_status = status;
  }
  
  return r;
}

void
rental_free(gpointer mem) {
  rental* r = (rental*)mem;
  
  g_date_time_unref(r->r_date);
  g_free(mem);
}
