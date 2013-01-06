#include "videorent.h"

const gchar* const title = "Once Upon a Time in the West";
const gchar* const url = "http://www.imdb.com/title/tt0064116/";
const gchar* const summary = "Epic story of a mysterious stranger with a harmonica who joins forces with a notorious desperado to protect a beautiful widow from a ruthless assassin working for the railroad.";
const gfloat rental_amt = 3.0;

void test_videorent_video_add(void) {
  videorent_video_add(title, url, summary, rental_amt);
  g_assert(videorent_video_get_title(0));
}

void test_videorent_video_title(void) {
  videorent_video_add(title, url, summary, rental_amt);
  g_assert_cmpstr(title, ==, videorent_video_get_title(0));
}

void test_videorent_video_url(void) {
  videorent_video_add(title, url, summary, rental_amt);
  g_assert_cmpstr(url, ==, videorent_video_get_url(0));
}

void test_videorent_video_summary(void) {
  videorent_video_add(title, url, summary, rental_amt);
  g_assert_cmpstr(summary, ==, videorent_video_get_summary(0));
}

void test_videorent_video_rental_amt(void) {
  videorent_video_add(title, url, summary, rental_amt);
  g_assert_cmpfloat(rental_amt, ==, videorent_video_get_rental_amt(0));
}

/*
Test that a video that's just added to the catalogue--and therefore
which should have a default of 3 copies in stock--was successfully
rented
*/
void test_videorent_rent(void) {
  videorent_video_add(title, url, summary, rental_amt);
  g_assert(videorent_rent(0, 0));
}

/*
Test that a video that's just added to the catalogue--and therefore
which should have a default of 3 copies in stock--should not rent more
than 3 times
*/
void test_videorent_not_rent(void) {
  videorent_video_add(title, url, summary, rental_amt);
  videorent_rent(0, 0);
  videorent_rent(0, 0);
  videorent_rent(0, 0);
  g_assert(!videorent_rent(0, 0));
}

int main(int argc, char* argv[]) {
  g_test_init(&argc, &argv, NULL);
  g_test_add_func("/videorent/video_add", test_videorent_video_add);
  g_test_add_func("/videorent/video_title", test_videorent_video_title);
  g_test_add_func("/videorent/video_url", test_videorent_video_url);
  g_test_add_func("/videorent/video_summary", test_videorent_video_summary);
  g_test_add_func("/videorent/video_rental_amt", test_videorent_video_rental_amt);
  g_test_add_func("/videorent/rent", test_videorent_rent);
  g_test_add_func("/videorent/not_rent", test_videorent_not_rent);
  return g_test_run();
}
