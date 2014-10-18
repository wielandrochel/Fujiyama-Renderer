// Copyright (c) 2011-2014 Hiroshi Tsubokawa
// See LICENSE and README

#ifndef FRAMEBUFFER_VIEWER_H
#define FRAMEBUFFER_VIEWER_H

#include "fj_framebuffer.h"
#include "fj_rectangle.h"
#include "fj_socket.h"
#include "image_card.h"

#include <string>
#include <vector>

namespace fj {

enum MouseButton {
  MOUSE_BUTTON_NONE = 0,
  MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_RIGHT
};

class FrameBufferViewer {
public:
  FrameBufferViewer();
  ~FrameBufferViewer();

  void Draw() const;
  void Resize(int width, int height);

  void PressButton(MouseButton button, int x, int y);
  void ReleaseButton(MouseButton button, int x, int y);
  void MoveMouse(int x, int y);
  void PressKey(unsigned char key, int mouse_x, int mouse_y);

  void StartListening();
  void StopListening();
  bool IsListening() const;
  void Listen();

  int LoadImage(const std::string &filename);

  void GetImageSize(int databox[4], int viewbox[4], int *nchannels) const;

private:
  void set_to_home_position();
  void setup_image_card();
  void draw_viewbox() const;

  FrameBuffer fb_;
  ImageCard image_;

  std::string filename_;
  bool is_listening_;

  int win_width_;
  int win_height_;
  int diplay_channel_;

  MouseButton pressbutton_;
  int xpresspos_;
  int ypresspos_;

  float scale_;
  float exponent_;
  float lockexponent_;
  float dist_per_pixel_;
  int xoffset_;
  int yoffset_;
  int xlockoffset_;
  int ylockoffset_;

  int databox_[4];
  int viewbox_[4];

  int tilesize_;
  int draw_tile_;

  //TODO TEST
  Socket server_;
  int state_;
  enum {
    STATE_NONE = 0,
    STATE_READY,
    STATE_RENDERING,
    STATE_DONE
  };

  class TileStatus {
  public:
    TileStatus() :
        state(STATE_READY), region()
    {}
    ~TileStatus() {}

    int state;
    Rectangle region;
  };
  std::vector<TileStatus> tiles;
};

} // namespace xxx

#endif // XXX_H
