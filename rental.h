#ifndef RENTAL_H
#define RENTAL_H
#include <glib.h>

typedef struct {
  guint r_id;
  GDateTime* r_date;
  guint v_id;
  guint m_id;
  gfloat r_amt;
  gfloat r_discount;
  gboolean r_open; // Rental status is open (i.e. rented out) or closed
} rental;

rental*
videorent_rental_new(
  const guint video_id,
  const guint member_id,
  gfloat amount,
  gfloat discount
);

void
videorent_rental_free(gpointer mem);

gint
videorent_rental_id_equals(gconstpointer item, gconstpointer v_id);

#endif // RENTAL_H
