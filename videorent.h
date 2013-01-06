#ifndef VIDEORENT_H
#define VIDEORENT_H
#include <glib.h>

void
videorent_video_add(
  const gchar* const title,
  const gchar* const url,
  const gchar* const summary,
  const gfloat rental_amt
);

const gchar* const
videorent_video_get_title(guint video_id);

void
videorent_video_set_title(guint video_id, const gchar* const title);

const gchar* const
videorent_video_get_url(guint video_id);

void
videorent_video_set_url(guint video_id, const gchar* const url);

const gchar* const
videorent_video_get_summary(guint video_id);

void
videorent_video_set_summary(guint video_id, const gchar* const summary);

const gfloat
videorent_video_get_rental_amt(guint video_id);

void
videorent_video_set_rental_amt(guint video_id, const gfloat rental_amt);

gboolean
videorent_rent(guint video_id, guint member_id);

gboolean
videorent_return(guint rental_id);

#endif // VIDEORENT_H
