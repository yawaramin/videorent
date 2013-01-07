#include "videorent.h"

const gchar* const title = "Once Upon a Time in the West";
const gchar* const url = "http://www.imdb.com/title/tt0064116/";
const gchar* const summary = "Epic story of a mysterious stranger with a harmonica who joins forces with a notorious desperado to protect a beautiful widow from a ruthless assassin working for the railroad.";
const gfloat rental_amt = 3.0;

void
setup(gpointer fixture, gconstpointer user_data) {
  videorent_add_video(title, url, summary, rental_amt);
}

void
teardown(gpointer fixture, gconstpointer user_data) {
  videorent_exit();
}

void test_videorent_add_video(gpointer fixture, gconstpointer user_data) {
  g_assert(videorent_get_video_title(0));
}

void test_videorent_video_title(gpointer fixture, gconstpointer user_data) {
  g_assert_cmpstr(title, ==, videorent_get_video_title(0));
}

void test_videorent_video_url(gpointer fixture, gconstpointer user_data) {
  g_assert_cmpstr(url, ==, videorent_get_video_url(0));
}

void test_videorent_video_summary(gpointer fixture, gconstpointer user_data) {
  g_assert_cmpstr(summary, ==, videorent_get_video_summary(0));
}

void test_videorent_video_rental_amt(gpointer fixture, gconstpointer user_data) {
  g_assert_cmpfloat(rental_amt, ==, videorent_get_video_rental_amt(0));
}

/*
Test that a video that's just added to the catalogue--and therefore
which should have a default of 3 copies in stock--was successfully
rented
*/
void test_videorent_rent(gpointer fixture, gconstpointer user_data) {
  g_assert(videorent_rent(0, 0));
}

/*
Test that a video that's just added to the catalogue--and therefore
which should have a default of 3 copies in stock--should not rent more
than 3 times
*/
void test_videorent_not_rent(gpointer fixture, gconstpointer user_data) {
  videorent_rent(0, 0);
  videorent_rent(0, 0);
  videorent_rent(0, 0);
  g_assert(!videorent_rent(0, 0));
}

void test_videorent_return(gpointer fixture, gconstpointer user_data) {
  videorent_rent(0, 0);
  g_assert(videorent_return(0));
}

/*
If we create a video but don't rent it, we shouldn't be able to return
it
*/
void test_videorent_not_return(gpointer fixture, gconstpointer user_data) {
  g_assert(!videorent_return(0));
}

/*
Shouldn't be able to return a video twice after renting it
*/
void test_videorent_not_return_twice(gpointer fixture, gconstpointer user_data) {
  videorent_rent(0, 0);
  videorent_return(0);
  g_assert(!videorent_return(0));
}

/*
Should be able to rent a video after returning it so that at least one
copy is in stock
*/
void test_videorent_rent_after_returning(gpointer fixture, gconstpointer user_data) {
  videorent_rent(0, 0);
  videorent_rent(0, 0);
  videorent_rent(0, 0);
  videorent_return(0);
  g_assert(videorent_rent(0, 0));
}

int main(int argc, char* argv[]) {
  g_test_init(&argc, &argv, NULL);

  g_test_add("/videorent/video_add", void, NULL, setup, test_videorent_add_video, teardown);
  g_test_add("/videorent/video_title", void, NULL, setup, test_videorent_video_title, teardown);
  g_test_add("/videorent/video_url", void, NULL, setup, test_videorent_video_url, teardown);
  g_test_add("/videorent/video_summary", void, NULL, setup, test_videorent_video_summary, teardown);
  g_test_add("/videorent/video_rental_amt", void, NULL, setup, test_videorent_video_rental_amt, teardown);
  g_test_add("/videorent/rent", void, NULL, setup, test_videorent_rent, teardown);
  g_test_add("/videorent/not_rent", void, NULL, setup, test_videorent_not_rent, teardown);
  g_test_add("/videorent/return", void, NULL, setup, test_videorent_return, teardown);
  g_test_add("/videorent/not_return", void, NULL, setup, test_videorent_not_return, teardown);
  g_test_add("/videorent/rent_after_returning", void, NULL, setup, test_videorent_rent_after_returning, teardown);

  return g_test_run();
}
