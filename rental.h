#ifndef RENTAL_H
#define RENTAL_H
#include <glib.h>

typedef enum {
  rental_open,
  rental_overdue,
  rental_closed
} rental_status;

typedef struct {
  guint r_id;
  GDateTime* r_date;
  guint v_id;
  gfloat r_amt;
  gfloat r_discount;
  rental_status r_status;
} rental;

rental*
rental_new(
  const guint video_id,
  const guint member_id,
  gfloat amount,
  gfloat discount,
  rental_status status
);

void
rental_free(gpointer mem);

#endif /* RENTAL_H */
