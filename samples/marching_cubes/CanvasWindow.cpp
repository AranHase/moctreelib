/*
 * Copyright (c) 2012 Allan Yoshio Hasegawa
 * 
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "CanvasWindow.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <stdio.h>

CanvasWindow::CanvasWindow(wxFrame *parent)
    : pending_setup_(true),
      wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE) {
  int argc = 1;
  char* argv[1] = { wxString((wxTheApp->argv)[0]).char_str()};

  gl_context_ = new wxGLContext(this);

  camera_position_.SetMovementSpeed(0.1, 0.1, 0.1);
  camera_position_.SetAngleSensitivity(0.1, 0.1, 0.1);

  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

CanvasWindow::~CanvasWindow() {
  delete gl_context_;
}

void CanvasWindow::OnPaintit(wxPaintEvent& WXUNUSED(event)) {
  wxGLCanvas::SetCurrent(*gl_context_);
  wxPaintDC(this);
  if (pending_setup_) {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    SetupOpenGLProjection();

    rot_ = 0;
    pending_setup_ = false;
  } else {
    Render();
    SwapBuffers();
  }
}

void CanvasWindow::Update(const double& delta_time) {
  camera_position_.Update(delta_time);
  Refresh(false);

  rot_ += 0.1 * delta_time;
}

void CanvasWindow::Render() {


  const float pi_over_180 = 0.0174532925f;

  GLfloat camera_x_trans = -camera_position_.position().x_;  // Used For Player Translation On The X Axis
  GLfloat camera_z_trans = -camera_position_.position().z_;  // Used For Player Translation On The Z Axis
  GLfloat camera_y_trans = -camera_position_.position().y_;    // Used For Bouncing Motion Up And Down

  GLfloat camera_x_rot = 360.0f - camera_position_.angle().x_;
  GLfloat camera_y_rot = 360.0f - camera_position_.angle().y_;  // 360 Degree Angle For Player Direction

  glLoadIdentity();

  glRotatef(camera_x_rot, 1.0f, 0, 0);  // Rotate Up And Down To Look Up And Down
  glRotatef(camera_y_rot, 0, 1.0f, 0);  // Rotate Depending On Direction Player Is Facing

  glTranslatef(camera_x_trans, camera_y_trans, camera_z_trans);  // Translate The Scene Based On Player

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glTranslatef(1.5f, 0.0f, -10.0f);            // Move Right And Into The Screen

  glRotatef(rot_, 1.0f, 1.0f, 1.0f);            // Rotate The Cube On X, Y & Z

  glBegin(GL_QUADS);                  // Start Drawing The Cube

  glColor3f(0.0f, 1.0f, 0.0f);          // Set The Color To Green
  glVertex3f(1.0f, 1.0f, -1.0f);          // Top Right Of The Quad (Top)
  glVertex3f(-1.0f, 1.0f, -1.0f);          // Top Left Of The Quad (Top)
  glVertex3f(-1.0f, 1.0f, 1.0f);          // Bottom Left Of The Quad (Top)
  glVertex3f(1.0f, 1.0f, 1.0f);          // Bottom Right Of The Quad (Top)
  glColor3f(1.0f, 0.5f, 0.0f);          // Set The Color To Orange
  glVertex3f(1.0f, -1.0f, 1.0f);          // Top Right Of The Quad (Bottom)
  glVertex3f(-1.0f, -1.0f, 1.0f);          // Top Left Of The Quad (Bottom)
  glVertex3f(-1.0f, -1.0f, -1.0f);          // Bottom Left Of The Quad (Bottom)
  glVertex3f(1.0f, -1.0f, -1.0f);          // Bottom Right Of The Quad (Bottom)
  glColor3f(1.0f, 0.0f, 0.0f);          // Set The Color To Red
  glVertex3f(1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Front)
  glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Front)
  glVertex3f(-1.0f, -1.0f, 1.0f);          // Bottom Left Of The Quad (Front)
  glVertex3f(1.0f, -1.0f, 1.0f);          // Bottom Right Of The Quad (Front)
  glColor3f(1.0f, 1.0f, 0.0f);          // Set The Color To Yellow
  glVertex3f(1.0f, -1.0f, -1.0f);          // Bottom Left Of The Quad (Back)
  glVertex3f(-1.0f, -1.0f, -1.0f);          // Bottom Right Of The Quad (Back)
  glVertex3f(-1.0f, 1.0f, -1.0f);          // Top Right Of The Quad (Back)
  glVertex3f(1.0f, 1.0f, -1.0f);  // Top Left Of The Quad (Back)glColor3f(0.0f,0.0f,1.0f);          // Set The Color To Blue
  glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Left)
  glVertex3f(-1.0f, 1.0f, -1.0f);          // Top Left Of The Quad (Left)
  glVertex3f(-1.0f, -1.0f, -1.0f);          // Bottom Left Of The Quad (Left)
  glVertex3f(-1.0f, -1.0f, 1.0f);          // Bottom Right Of The Quad (Left)
  glColor3f(1.0f, 0.0f, 1.0f);          // Set The Color To Violet
  glVertex3f(1.0f, 1.0f, -1.0f);          // Top Right Of The Quad (Right)
  glVertex3f(1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Right)
  glVertex3f(1.0f, -1.0f, 1.0f);          // Bottom Left Of The Quad (Right)
  glVertex3f(1.0f, -1.0f, -1.0f);          // Bottom Right Of The Quad (Right)
  glEnd();                        // Done Drawing The Quad

  glFlush();
}

void CanvasWindow::SetupOpenGLProjection() {

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(0, 0, (GLint) GetSize().x, (GLint) GetSize().y);

  gluPerspective(45.0f, (GLfloat) GetSize().x / (GLfloat) GetSize().y, 0.1f,
                 100.0f);

  glMatrixMode(GL_MODELVIEW);
}

void CanvasWindow::OnResized(wxSizeEvent& evt) {

  SetupOpenGLProjection();

  Refresh();

}

void CanvasWindow::OnIdle(wxIdleEvent &event) {
  double delta_time = 1.f;
  Update(delta_time);
}

void CanvasWindow::OnMouseMoved(wxMouseEvent& event) {
  const int px = event.GetPosition().x;
  const int py = event.GetPosition().y;

  if (mouse_right_down_) {
    const int lpx = mouse_last_posistion_.x_;
    const int lpy = mouse_last_posistion_.y_;

    camera_position_.SetAngleDisplacement(-(py - lpy), px - lpx, 0);
  }

  mouse_last_posistion_.x_ = px;
  mouse_last_posistion_.y_ = py;

}

void CanvasWindow::OnMouseWheelMoved(wxMouseEvent& event) {

}
void CanvasWindow::OnMouseLeftDown(wxMouseEvent& event) {
  SetFocus();
}
void CanvasWindow::OnMouseLeftUp(wxMouseEvent& event) {
  SetFocus();
}
void CanvasWindow::OnMouseRightDown(wxMouseEvent& event) {
  SetFocus();
  mouse_right_down_ = true;
}
void CanvasWindow::OnMouseRightUp(wxMouseEvent& event) {
  SetFocus();
  mouse_right_down_ = false;
}
void CanvasWindow::OnMouseLeftWindow(wxMouseEvent& event) {

}

void CanvasWindow::OnKeyPressed(wxKeyEvent& event) {
  const int kc = event.GetKeyCode();

  switch (kc) {
    case WXK_PAGEUP: {
      camera_position_.SetYMovement(FORWARD);
      break;
    }
    case WXK_PAGEDOWN: {
      camera_position_.SetYMovement(BACKWARD);
      break;
    }
    case WXK_LEFT: {
      //camera_position_.SetXMovement(BACKWARD);
      break;
    }
    case WXK_RIGHT: {
      //camera_position_.SetXMovement(FORWARD);
      break;
    }
    case WXK_UP: {
      camera_position_.SetZMovement(BACKWARD);
      camera_position_.SetXMovement(BACKWARD);
      break;
    }
    case WXK_DOWN: {
      camera_position_.SetZMovement(FORWARD);
      camera_position_.SetXMovement(FORWARD);
      break;
    }
    case WXK_BACK: {
      camera_position_.Reset();
      break;
    }
  }
  printf("KD: %d\n", event.GetKeyCode());
}

void CanvasWindow::OnKeyReleased(wxKeyEvent& event) {
  const int kc = event.GetKeyCode();

  switch (kc) {
    case WXK_LEFT:
    case WXK_RIGHT: {
      camera_position_.SetXMovement(NONE);
      camera_position_.SetZMovement(NONE);
      break;
    }
    case WXK_UP:
    case WXK_DOWN: {
      camera_position_.SetXMovement(NONE);
      camera_position_.SetZMovement(NONE);
      break;
    }
  }
  camera_position_.SetYMovement(NONE);
  printf("KU: %d\n", event.GetKeyCode());
}
/*
 * wxWidget events link =X
 */                        //
BEGIN_EVENT_TABLE(CanvasWindow, wxGLCanvas)   //
EVT_MOUSEWHEEL(CanvasWindow::OnMouseWheelMoved)  //
EVT_MOTION(CanvasWindow::OnMouseMoved)//
EVT_LEFT_DOWN(CanvasWindow::OnMouseLeftDown)//
EVT_LEFT_UP(CanvasWindow::OnMouseLeftUp)//
EVT_RIGHT_DOWN(CanvasWindow::OnMouseRightDown)//
EVT_RIGHT_UP(CanvasWindow::OnMouseRightUp)//
EVT_LEAVE_WINDOW(CanvasWindow::OnMouseLeftWindow)//
EVT_KEY_DOWN(CanvasWindow::OnKeyPressed)//
EVT_KEY_UP(CanvasWindow::OnKeyReleased)//
EVT_PAINT (CanvasWindow::OnPaintit)//
EVT_SIZE(CanvasWindow::OnResized)//
EVT_IDLE(CanvasWindow::OnIdle)//
END_EVENT_TABLE()//
