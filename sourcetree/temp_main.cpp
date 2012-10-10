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

#include <iostream>
#include <moctree/MOctree.h>
#include <moctree/classic/ClassicOctree.h>
#include <moctree/classic/ClassicOctreeCell.h>

int main() {
  using namespace std;

  int types[2];
  types[0] = 1;
  types[1] = 2;

  moctree::MOctree<int> *t0;
  t0 = new moctree::ClassicOctree<int>(4);

  t0->InsertCell(0, 0, 0, types + 0);
  t0->InsertCell(2, 3, 1, types + 1);

  cout << t0->ToString() << endl;

  cout << "Hello" << endl;

  cout << "T0: " << types + 0 << endl << "T1: " << types + 1 << endl;

  cout << t0->GetData(0, 0, 0) << endl;
  cout << t0->GetData(2, 3, 1) << endl;

  cout << "World!" << endl;

  delete t0;

  return 0;
}
