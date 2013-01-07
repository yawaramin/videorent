#ifndef VIDEORENT_H
#define VIDEORENT_H
#include <glib.h>

void
videorent_add_video(
  const gchar* const title,
  const gchar* const url,
  const gchar* const summary,
  const gfloat rental_amt
);

const gchar* const
videorent_get_video_title(guint video_id);

void
videorent_set_video_title(guint video_id, const gchar* const title);

const gchar* const
videorent_get_video_url(guint video_id);

void
videorent_set_video_url(guint video_id, const gchar* const url);

const gchar* const
videorent_get_video_summary(guint video_id);

void
videorent_set_video_summary(guint video_id, const gchar* const summary);

const gfloat
videorent_get_video_rental_amt(guint video_id);

void
videorent_set_video_rental_amt(guint video_id, const gfloat rental_amt);

gboolean
videorent_rent(guint video_id, guint member_id);

gboolean
videorent_get_rental_overdue(const guint rental_id);

gboolean
videorent_return(guint rental_id);

void
videorent_exit();

#endif // VIDEORENT_H
